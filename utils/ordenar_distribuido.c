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
    const int BlockSize=BLOCK_SIZE;// constante del tamaño de bloque

        // Reserva de memoria
    V1 = (int *)malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));

    inicializarVectors(V1,V2,N,K);

  //  MPI_Request request;
  //  MPI_Status status;

    MPI_Barrier(MPI_COMM_WORLD);

    double t0 = dwalltime();

    // Master envia bloques a cada proceso incluido el mismo
    MPI_Scatter(V1, BlockSize, MPI_INT, //Pointer to data, length, type
              V1, BlockSize, MPI_INT, //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);

    ordenar_distribuido(&V1,&Vtemp,BlockSize,N,MASTER_ID,cantProcesos);

    //Se repite para el vector 2
    // Master envia bloques a cada proceso incluido el mismo
    MPI_Scatter(V2, BlockSize, MPI_INT, V2, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    ordenar_distribuido(&V2,&Vtemp,BlockSize,N,MASTER_ID,cantProcesos);

    //Procede a enviar los subvectores a cada proceso para la comparación, incluido el mismo
    MPI_Scatter(V1, BlockSize, MPI_INT, V1, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(V2, BlockSize, MPI_INT, V2, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);


    int diferencia = 0;
    int diferenciaGlobal;
    for (int i=0; (i < BlockSize); i++){
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
    int *Vtemp;              // Arreglo temporal para ordenar
    const int BlockSize = BLOCK_SIZE; // constante del tamaño de bloque
    int enviarVector = 0;
    
    // Reserva de memoria. Contempla para el momento de hacer el merge, la cantidad máxima de valores que puede tener es de N/2
    V1 = (int*) malloc(sizeof(int) * N/2);
    Vtemp = (int*) malloc(sizeof(int) * N/2);


    MPI_Request request;
    MPI_Status status;

    MPI_Barrier(MPI_COMM_WORLD);

    //ordenar coperativo para Vector1
    MPI_Scatter(NULL, 0, MPI_INT,           //Pointer to data, length, type
              V1, BlockSize, MPI_INT,    //Pointer to data received, length, type
              MASTER_ID, MPI_COMM_WORLD);
    ordenar_distribuido(&V1, &Vtemp, BlockSize, N, miID, cantProcesos);

    //Se repite para el Vector 2
    MPI_Scatter(NULL, 0, MPI_INT,V1, BlockSize, MPI_INT,MASTER_ID, MPI_COMM_WORLD);
    ordenar_distribuido(&V1, &Vtemp, BlockSize, N, miID, cantProcesos);

    //Recibe su porción de vector
    MPI_Scatter(NULL, 0, MPI_INT, V1, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(NULL, 0, MPI_INT, Vtemp, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    int flag_diferencia = 0;
    for (int i = 0; i < BlockSize; i++)
    {
        if (V1[i] != Vtemp[i])
        {
            flag_diferencia++;
            break;
        }
    }

    MPI_Reduce(&flag_diferencia, NULL, 1, MPI_INT, MPI_MAX, MASTER_ID, MPI_COMM_WORLD);

    free(V1);
    free(Vtemp);
}

// proceso de ordenamiento colaborativo, cada proceso con su ID, con pasaje de mensajes
void ordenar_distribuido(int **vec, int **vecTemp, int BlockSize, int N, int id, int cantProcesos)
{
    // Ordena su porción de vector (recordar que en todos los procesos, el pedazo de vector que le toca arranca en 0)
    iterativeSortSwap(vec, vecTemp, 0, BlockSize);

    // El master siempre trabaja
    // Para este caso se trabaja con el proceso contiguo a la derecha, respetando el intervalo de procesos activos
    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *= 2)
    {
        int offset = BlockSize * (intervalProcesoActivo / 2);

        if ((id % intervalProcesoActivo) == 0)
        {
            // Espera a recibir el subvector de la derecha en la posición correspondiente
            MPI_Recv(*vec + offset, offset, MPI_INT, id + (intervalProcesoActivo / 2), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Hace el merge con su porción
            mergeBlocksToOut(*vec, *vecTemp, 0, offset);

            // Se invierten los punteros para tener en V1 el vector actualizado
            int *vect = *vec;
            *vec = *vecTemp;
            *vecTemp = vect;
        }
        else // Si no es el que trabaja, envía su porción al proceso de la izquierda correspondiente y termina
        {
            MPI_Send(*vec, offset, MPI_INT, id - (intervalProcesoActivo / 2), 1, MPI_COMM_WORLD);
            break;
        }
    }
}
