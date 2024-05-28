#include "ordenar_distribuido.h"

#define BLOCK_SIZE (N/cantProcesos)

// copia de las variables del main
//int Nglobal,cantProcesosGlobal;

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

    MPI_Request request;
    MPI_Status status;

    MPI_Barrier(MPI_COMM_WORLD);

    int* V1Original = V1;
    int* V2Original = V2;
    int* VtempOriginal = Vtemp;

    double t0 = dwalltime();

    //mergeParaleloMPI(&V1, &Vtemp, MASTER_ID, tamañoBloque, cantProcesos);

    // Master envia bloques a cada proceso incluido el mismo
    MPI_Scatter(V1, tamañoBloque, MPI_INT, //Pointer to data, length, type
              V1, tamañoBloque, MPI_INT, //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);

    // Ordena su porción de vector (recordar que en todos los procesos, el pedazo de vector que le toca arranca en 0)
    iterativeSortSwap(&V1, &Vtemp, 0, tamañoBloque);

    // El master siempre trabaja
    //Para este caso se trabaja con el proceso contiguo a la derecha, respetando el intervalo de procesos activos
    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *=2){
    
        //Espera a recibir el subvector de la derecha en la posición correspondiente
        MPI_Recv(V1 + BLOCK_SIZE * (intervalProcesoActivo/2), tamañoBloque, MPI_INT, intervalProcesoActivo/2, 1, MPI_COMM_WORLD);

        //Hace el merge con su porción        
        mergeBlocksToOut(V1,Vtemp,0, BLOCK_SIZE * (intervalProcesoActivo/2));
        
        //Se invierten los punteros para tener en V1 el vector actualizado
        int* vect=V1;
        V1=Vtemp;
        Vtemp=vect;
    }
    
    //Se repite para el vector 2
    // Master envia bloques a cada proceso incluido el mismo
    MPI_Scatter(V2, tamañoBloque, MPI_INT, //Pointer to data, length, type
              V2, tamañoBloque, MPI_INT, //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);

    // Ordena su porción de vector (recordar que en todos los procesos, el pedazo de vector que le toca arranca en 0)
    iterativeSortSwap(&V2, &Vtemp, 0, tamañoBloque);

    // El master siempre trabaja
    //Para este caso se trabaja con el proceso contiguo a la derecha, respetando el intervalo de procesos activos
    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *=2){
    
        //Espera a recibir el subvector de la derecha en la posición correspondiente
        MPI_Recv(V2 + BLOCK_SIZE * (intervalProcesoActivo/2), tamañoBloque, MPI_INT, intervalProcesoActivo/2, 1, MPI_COMM_WORLD);

        //Hace el merge con su porción        
        mergeBlocksToOut(V2,Vtemp,0, BLOCK_SIZE * (intervalProcesoActivo/2));

        //Se invierten los punteros para tener en V1 el vector actualizado
        int* vect=V2;
        V2=Vtemp;
        Vtemp=vect;
    }

    //Procede a enviar los subvectores a cada proceso para la comparación, incluido el mismo

    MPI_Scatter(V1, tamañoBloque, MPI_INT, V1, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(V2, tamañoBloque, MPI_INT, V2, tamañoBloque, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    for (int i=0; (i < tamañoBloque)&&(!()); i++){
        if(vec1[i] != vec2[i]){
            //*flag_diference = 1;
            break;
        }
    }

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

    int tamañoBloque = N/cantProcesos;
    
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

    MPI_Barrier(MPI_COMM_WORLD);
    //For por niveles 
    for(){
        if(){
            //ESpera a que le llegue el pedazo de vector contiguo
            //trabaja (hace merge)
        }
        else{
            //envia su pedazo de vector al proceso correspondiente
            //MPI_Send (V + posContigua);
            //break;
        }
        //finaliza
    }
    MPI_Scatter(NULL, 0, MPI_INT,           //Pointer to data, length, type
              V1, tamañoBloque, MPI_INT,    //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);
    
    iterativeSortSwap(&V2, &Vtemp, 0, tamañoBloque);
}


void compararVecMPI(int* vec1, int* vec2, int offset, int NLength, int* flag_diference){
    vec1+=offset;
    vec2+=offset;
    for (int i=0; (i<NLength)&&(!(*flag_diference)); i++){
        if(vec1[i] != vec2[i]){
            *flag_diference = 1;
            break;
        }
    }
}

/*
void mergeParaleloMPI(int** ptrVec, int** ptrVecTemp, int id, int tamañoBloque){
    int offsetID=OFFSET_N_ID_BY_THREAD;

// Etapa 1 - cada Hilo ordena 1 segmento del vector
    iterativeSortSwap(ptrVec,ptrVecTemp,offsetID , tamañoBloque );

// Etapa 2 - embudo de ordenamiento
    //pthread_barrier_wait(&barrera); SE BORRA PARA MPI

    int* vec= *ptrVec;// los vectores de trabajo privados
    int* vecOut= *ptrVecTemp;

    //Se combinan vectores contiguos
    for (int porciones=2; porciones <= cantProcesos; porciones *= 2){
        //Si el id mod porciones es 0, el hilo trabaja
        if (id % porciones == 0){
            mergeBlocksToOut(vec, vecOut,offsetID, porciones *(tamañoBloque/2)); //ordeno 1 nivel
            int* vect=vecOut;// invierto los vectores de trabajo
            vecOut=vec;
            vec=vect;
        }
            //Se espera a que todos los hilos terminen de trabajar para pasar al siguiente nivel
	    pthread_barrier_wait(&barrera);
    }
    if(id==0){
        *ptrVec=vec;// guardo los vectores ordenados
        *ptrVecTemp=vecOut;
    }
    pthread_barrier_wait(&barrera);
}

*/