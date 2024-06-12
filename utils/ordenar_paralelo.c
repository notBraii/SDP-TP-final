#include "ordenar_paralelo.h"

#define ELEMENTS_N_PER_THREAD (Nlong/Thilos)
#define OFFSET_N_ID_PER_THREAD (id*(Nlong/Thilos))
#define BLOCKSIZE_PORTION_PER_THREAD (intervalTActivo*(Nlong/(Thilos*2)))

int *Vec1, *Vec2, *VecTemp; // copia de las variables del main
int Nlong, Thilos;
int *flagDif;
pthread_barrier_t barrera;

void ordenar_paralelo(int **Vec_1, int **Vec_2, int **Vec_Temp, int N_long, int T_hilos, int* flag_diferencia){
    // Copia los valores al contexto de la libreria
    Vec1=*Vec_1;  Vec2=*Vec_2;  VecTemp=*Vec_Temp;    Nlong=N_long;    Thilos=T_hilos; flagDif=flag_diferencia;

    int ids[Thilos];
    pthread_t hilos[Thilos];
    pthread_barrier_init(&barrera, NULL, Thilos); // inicializa la barrera global

    for (int i = 0; i < Thilos; i++) {   // Crear los hilos
        ids[i] = i;
        pthread_create(&hilos[i], NULL, taskThread, &ids[i]); // manda a ejecutar tarea
    }

    for (int i = 0; i < Thilos; i++) {    // Esperar a que todos los hilos terminen
        pthread_join(hilos[i], NULL);
    }
    pthread_barrier_destroy(&barrera);

    // Se guardan los punteros ordenados en los vectores originales 
    *Vec_1 = Vec1;  
    *Vec_2 = Vec2;
    *Vec_Temp = VecTemp;

}

void *taskThread(void *arg) {
    int id = *((int*)arg);

    mergeParalelo(id,&Vec1,&VecTemp);
    mergeParalelo(id,&Vec2,&VecTemp);

    // Cada hilo compara una parte de los vectores según su id, y guardan resultado en el flag
    compararVec(Vec1,Vec2, OFFSET_N_ID_PER_THREAD, ELEMENTS_N_PER_THREAD,flagDif); 

    pthread_exit(NULL);
}

void mergeParalelo(int id, int** ptrVec, int** ptrVecTemp){
    int offsetID=OFFSET_N_ID_PER_THREAD;

    // Cada Hilo ordena 1 segmento del vector
    iterativeSortSwap(ptrVec,ptrVecTemp,offsetID , ELEMENTS_N_PER_THREAD );

    // Espera a que todos los hilos terminen su trabajo local
    pthread_barrier_wait(&barrera);

    // Vectores auxiliares
    int* vec= *ptrVec;
    int* vecOut= *ptrVecTemp;

    // Se combinan vectores contiguos
    for (int intervalTActivo = 2; intervalTActivo <= Thilos; intervalTActivo *= 2){
        // Si el id mod intervalTActivo es 0, el hilo trabaja
        if (id % intervalTActivo == 0){
            mergeBlocksToOut(vec, vecOut,offsetID, BLOCKSIZE_PORTION_PER_THREAD); // ordena 1 nivel
            // Invierte los vectores de trabajo
            int* vect=vecOut;
            vecOut=vec;
            vec=vect;
        }
        // Se espera a que todos los hilos terminen de trabajar para pasar al siguiente nivel
	    pthread_barrier_wait(&barrera);
    }
    if(id == 0){
        *ptrVec = vec; // Guarda los vectores ordenados
        *ptrVecTemp = vecOut;
    }
    pthread_barrier_wait(&barrera);
}

