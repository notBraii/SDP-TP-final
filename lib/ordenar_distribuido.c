#include "ordenar_distribuido.h"

#define BLOCK_SIZE (N/cantProcesos)

void master(int N, int K, int cantProcesos){
    int *V1;                        // Arreglo 1
    int *V2;                        // Arreglo 2
    int *Vtemp;                     // Arreglo temporal para ordenar
    const int BlockSize=BLOCK_SIZE; // constante con el tamaño del bloque asignado a cada proceso

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));

    inicializarVectors(V1,V2,N,K);

    MPI_Barrier(MPI_COMM_WORLD);

    double t0 = dwalltime();

    // Master envía bloques a todos los procesos, incluido él mismo
    MPI_Scatter(V1, BlockSize, MPI_INT, V1, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    // Ordena de manera cooperativa con los otros procesos
    ordenar_distribuido(&V1, &Vtemp, BlockSize, N, MASTER_ID, cantProcesos);

    // Se repite para el vector 2:
    MPI_Scatter(V2, BlockSize, MPI_INT, V2, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    ordenar_distribuido(&V2, &Vtemp, BlockSize, N, MASTER_ID, cantProcesos);

    // Procede a enviar los subvectores a cada proceso para la comparación, incluido él mismo
    MPI_Scatter(V1, BlockSize, MPI_INT, V1, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(V2, BlockSize, MPI_INT, V2, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    // Compara localmente ambas porciones de vectores
    int diferencia = 0;
    int diferenciaGlobal;
    for (int i=0; (i < BlockSize); i++){
        if(V1[i] != V2[i]){
            diferencia++;
            break;
        }
    }

    // Se envía a través de una operación reductora si se enconcuentra alguna diferencia en los procesos
    // Como es el MASTER, recibirá el resultado en diferenciaGlobal
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

void slave(int N, int cantProcesos, int miID){
    int *V1;                        // Arreglo 1 con valores
    int *Vtemp;                     // Arreglo temporal para ordenar
    const int BlockSize=BLOCK_SIZE; // constante con el tamaño del bloque asignado a cada proceso
    int enviarVector = 0;
    
    // Reserva de memoria. Para simplificar, se le asigna la cantidad máxima de valores que puede tener un proceso slave, que es de N/2
    V1 = (int*) malloc(sizeof(int) * N/2);
    Vtemp = (int*) malloc(sizeof(int) * N/2);


    MPI_Request request;
    MPI_Status status;

    MPI_Barrier(MPI_COMM_WORLD);

    // Recibe la porción del vector 1 
    MPI_Scatter(NULL, 0, MPI_INT, V1, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    // Ordena de manera cooperativa con los otros procesos
    ordenar_distribuido(&V1, &Vtemp, BlockSize, N, miID, cantProcesos);

    // Se repite para el Vector 2
    MPI_Scatter(NULL, 0, MPI_INT,V1, BlockSize, MPI_INT,MASTER_ID, MPI_COMM_WORLD);
    ordenar_distribuido(&V1, &Vtemp, BlockSize, N, miID, cantProcesos);

    // Recibe la porción de los vectores a comparar
    MPI_Scatter(NULL, 0, MPI_INT, V1, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
    MPI_Scatter(NULL, 0, MPI_INT, Vtemp, BlockSize, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

    // Compara localmente
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

void ordenar_distribuido(int **vec, int **vecTemp, int BlockSize, int N, int id, int cantProcesos)
{
    // Ordena su porción de vector (recordar que para todos los procesos, el pedazo de vector que le toca arranca en 0)
    iterativeSortSwap(vec, vecTemp, 0, BlockSize);

    // La forma de resolución optada es la de trabajar con el proceso contiguo a la derecha, respetando el intervalo de procesos activos
    for (int intervalProcesoActivo = 2; intervalProcesoActivo <= cantProcesos; intervalProcesoActivo *= 2)
    {
        int offset = BlockSize * (intervalProcesoActivo / 2);

        if ((id % intervalProcesoActivo) == 0)
        {
            // Espera a recibir el subvector del proceso contiguo a la derecha en la posición correspondiente del vector
            MPI_Recv(*vec + offset, offset, MPI_INT, id + (intervalProcesoActivo / 2), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Hace el merge con su porción
            mergeBlocksToOut(*vec, *vecTemp, 0, offset);

            // Se invierten los punteros para tener en V1 el vector actualizado
            int *vect = *vec;
            *vec = *vecTemp;
            *vecTemp = vect;
        }
        else
        {
            // Envía su porción al proceso de la izquierda correspondiente y termina
            MPI_Send(*vec, offset, MPI_INT, id - (intervalProcesoActivo / 2), 1, MPI_COMM_WORLD);
            break;
        }
    }
}
