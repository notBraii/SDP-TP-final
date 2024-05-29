#include "ordenar_distribuido.h"

#define BLOCK_SIZE (N/cantProcesos)
//#define DEBUG
//#define DEBUG2

/**Operaciones del master
 * @param N indica el tamaño del vector
 * @param K indica la cantidad de valores distintos del vector 2
 * @param cantProcesos indica la cantidad de procesos
*/
void master(int N, int K, int cantProcesos){
    int *V1;                 // Arreglo 1 con valores
    int *V2;                 // Arreglo 2 con valores
    int *Vtemp;              // Arreglo temporal para ordenar
    int tamañoBloque = BLOCK_SIZE;

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));

    inicializarVectors(V1,V2,N,K);

    #ifdef DEBUG2
        verVector(V1, N);
        verVector(V2, N);
    #endif

    MPI_Request request;
    MPI_Status status;

    MPI_Barrier(MPI_COMM_WORLD);

    double t0 = dwalltime();

    //mergeParaleloMPI(&V1, &Vtemp, MASTER_ID, tamañoBloque, cantProcesos);

    // Master envia bloques a cada proceso incluido el mismo
    MPI_Scatter(V1, tamañoBloque, MPI_INT, //Pointer to data, length, type
              V1, tamañoBloque, MPI_INT, //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);

    // Ordena su porción de vector (recordar que en todos los procesos, el pedazo de vector que le toca arranca en 0)
    iterativeSortSwap(&V1, &Vtemp, 0, tamañoBloque);
    #ifdef DEBUG
        printf("Proceso: %d termina su parte de mergesort.\n", MASTER_ID);
        verVector(V1, tamañoBloque);
    #endif
    // El master siempre trabaja
    //Para este caso se trabaja con el proceso contiguo a la derecha, respetando el intervalo de procesos activos
    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *=2){
        #ifdef DEBUG
            printf("NIvel de iteración: %d.\n", intervalProcesoActivo);    
            printf("Proceso: %d espera subvector del proceso contiguo: %d para merge.\n", MASTER_ID, intervalProcesoActivo/2);
        #endif
        
        //Espera a recibir el subvector de la derecha en la posición correspondiente
        MPI_Recv(V1 + BLOCK_SIZE * (intervalProcesoActivo/2), tamañoBloque*intervalProcesoActivo/2, MPI_INT, intervalProcesoActivo/2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        #ifdef DEBUG
            printf("Proceso: %d recibe subvector del proceso contiguo: %d para merge.\n", MASTER_ID, intervalProcesoActivo/2);
            verVector(V1, tamañoBloque*intervalProcesoActivo);
        #endif

        //Hace el merge con su porción        
        mergeBlocksToOut(V1,Vtemp,0, BLOCK_SIZE * (intervalProcesoActivo/2));
        
        #ifdef DEBUG
            printf("Proceso: %d termina su parte de merge.\n", MASTER_ID);
            verVector(Vtemp, tamañoBloque*intervalProcesoActivo);
        #endif

        //Se invierten los punteros para tener en V1 el vector actualizado
        int* vect=V1;
        V1=Vtemp;
        Vtemp=vect;
    }
    
    //Se repite para el vector 2
    // Master envia bloques a cada proceso incluido el mismo
    MPI_Scatter(V2, tamañoBloque, MPI_INT, V2, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    iterativeSortSwap(&V2, &Vtemp, 0, tamañoBloque);

    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *=2){
    
        MPI_Recv(V2 + BLOCK_SIZE * (intervalProcesoActivo/2), tamañoBloque*intervalProcesoActivo/2, MPI_INT, intervalProcesoActivo/2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mergeBlocksToOut(V2,Vtemp,0, BLOCK_SIZE * (intervalProcesoActivo/2));

        int* vect=V2;
        V2=Vtemp;
        Vtemp=vect;
    }

    //Procede a enviar los subvectores a cada proceso para la comparación, incluido el mismo
    MPI_Scatter(V1, tamañoBloque, MPI_INT, V1, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(V2, tamañoBloque, MPI_INT, V2, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);


    int diferencia = 0;
    int diferenciaGlobal;
    for (int i=0; (i < tamañoBloque); i++){
        if(V1[i] != V2[i]){
            diferencia++;
            break;
        }
    }

    //Al finalizar su comparación local, envía a través de una operación reductora si se enconcontró alguna diferencia en los procesos
    //Como es el MASTER, recibirá el resultado en diferenciaGlobal
    MPI_Reduce(&diferencia, &diferenciaGlobal, 1, MPI_INT, MPI_MAX, MASTER_ID, MPI_COMM_WORLD);

    // Al obtener el resultado de la comparación, obtiene el tiempo de ejecucion de la tarea y la imprime
    printf("Para N:%d, cantProcesos:%d , tardo %f segundos\n", N,cantProcesos, dwalltime()- t0);

    if (diferenciaGlobal) printf("\n \t - Hay diferencia entre los vectores \n");
    else  printf("\n \t - Los vectores son iguales \n");

    //verVector
    #ifdef DEBUG2
        verVector(V1, N);
        verVector(V2, N);
    #endif

    // chequear ambos vectores 
    printf("el Vector1: %s\n", orderCheck(V1, 0, N) ? "esta ordenado correctamente": " ");
    printf("el Vector2: %s\n", orderCheck(V2, 0, N) ? "esta ordenado correctamente": " ");

    free(V1);
    free(V2);
    free(Vtemp);
}

/**Operaciones del worker/slave
 * @param N indica el tamaño del vector
 * @param cantProcesos indica la cantidad de procesos
 * @param miID indica el valor ID del proceso 
*/
void slave(int N, int cantProcesos, int miID){
    int *V1;                 // Arreglo 1 con valores
    int *V2;                 // Arreglo 2 con valores
    int *Vtemp;              // Arreglo temporal para ordenar
    int enviarVector = 0;
    int tamañoBloque = BLOCK_SIZE;
    
    // Reserva de memoria. Contempla para el momento de hacer el merge, la cantidad máxima de valores que puede tener es de N/2
    V1 = (int*) malloc(sizeof(int) * N/2);
    V2 = (int*) malloc(sizeof(int) * N/2);
    Vtemp = (int*) malloc(sizeof(int) * N/2);


    MPI_Request request;
    MPI_Status status;

    MPI_Barrier(MPI_COMM_WORLD);

    //Para Vector1
    MPI_Scatter(NULL, 0, MPI_INT,           //Pointer to data, length, type
              V1, tamañoBloque, MPI_INT,    //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);
    
    iterativeSortSwap(&V1, &Vtemp, 0, tamañoBloque);

     #ifdef DEBUG
        printf("Proceso: %d termina su parte de mergesort.\n", miID);
        verVector(V1, tamañoBloque);
    #endif
    
    //Realiza su correspondiente trabajo en cada nivel
    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *=2){
        
        #ifdef DEBUG
            printf("NIvel de iteración: %d.\n", intervalProcesoActivo);    
            printf("El proceso: %d espera ver qué hacer:\n", miID);
        #endif

        //Si es el proceso que trabaja recibe el vector contiguo y trabaja
        if((miID % intervalProcesoActivo) == 0){

            #ifdef DEBUG
                printf("Proceso: %d espera subvector del proceso contiguo: %d para merge.\n", miID, miID + intervalProcesoActivo/2);
            #endif
            //Espera a recibir el subvector del proceso de la derecha en la posición correspondiente
            MPI_Recv(V1 + BLOCK_SIZE * (intervalProcesoActivo/2), tamañoBloque*intervalProcesoActivo/2, MPI_INT, miID  + intervalProcesoActivo/2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            #ifdef DEBUG
                printf("Proceso: %d recibe subvector del proceso contiguo: %d para merge.\n", miID, intervalProcesoActivo/2);
                verVector(V1, tamañoBloque*intervalProcesoActivo);
            #endif
            //Hace el merge con su porción        
            mergeBlocksToOut(V1,Vtemp,0, BLOCK_SIZE * (intervalProcesoActivo/2));

            //Se invierten los punteros para tener en V1 el vector actualizado
            int* vect=V1;
            V1=Vtemp;
            Vtemp=vect;
        }
        //Si no es el que trabaja, envía su porción al proceso de la izquierda correspondiente y termina
        else{
             #ifdef DEBUG
                printf("Proceso: %d ENVIA subvector al proceso contiguo: %d para merge.\n", miID, miID  - intervalProcesoActivo/2);
            #endif
            MPI_Send(V1,tamañoBloque * (intervalProcesoActivo/2), MPI_INT, miID - intervalProcesoActivo/2, 1, MPI_COMM_WORLD);
            break;
        }
    }
    
    //Se repite para el vector 2
    MPI_Scatter(NULL, 0, MPI_INT,           //Pointer to data, length, type
              V2, tamañoBloque, MPI_INT,    //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);
    
    iterativeSortSwap(&V2, &Vtemp, 0, tamañoBloque);

     for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *=2){     
        if((miID % intervalProcesoActivo) == 0){

            MPI_Recv(V2 + BLOCK_SIZE * (intervalProcesoActivo/2), tamañoBloque*intervalProcesoActivo/2, MPI_INT, miID  + intervalProcesoActivo/2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            mergeBlocksToOut(V2,Vtemp,0, BLOCK_SIZE * (intervalProcesoActivo/2));

            int* vect=V2;
            V2=Vtemp;
            Vtemp=vect;
        }
        else{
            MPI_Send(V2,tamañoBloque * (intervalProcesoActivo/2), MPI_INT, miID - intervalProcesoActivo/2, 1, MPI_COMM_WORLD);
            break;
        }
    }
    
    //Recibe su porción de vector
    MPI_Scatter(NULL, 0, MPI_INT, V1, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(NULL, 0, MPI_INT, V2, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);


    int diferencia = 0;
    //int diferenciaGlobal;
    for (int i=0; (i < tamañoBloque); i++){
        if(V1[i] != V2[i]){
            diferencia++;
            break;
        }
    }

    //MPI_Reduce(&diferencia, &diferenciaGlobal, 1, MPI_INT, MPI_MAX, MASTER_ID, MPI_COMM_WORLD);
    MPI_Reduce(&diferencia, NULL, 1, MPI_INT, MPI_MAX, MASTER_ID, MPI_COMM_WORLD);

    free(V1);
    free(V2);
    free(Vtemp);
}