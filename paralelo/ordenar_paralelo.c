

#include "ordenar_paralelo.h"

void ordenar_paralelo(){
    for (int i = 0; i < T; i++) {   // Crear los hilos
        ids[i] = i;
        pthread_create(&hilos[i], NULL, taskThread, &ids[i]); // manda a ejecutar tarea
    }
    for (int i = 0; i < T; i++) {    // Esperar a que todos los hilos terminen
        pthread_join(hilos[i], NULL);
    }
}

void *taskThread(void *arg) {
    int indice;
    int id = *((int*)arg);

    mergeParalelo(id,V1,Vtemp);
    mergeParalelo(id,V2,Vtemp);

    compararVec(V1,V2, id*N/T, N/T,&flag_diferencia); // todos los hilos comparan un pedazo de los vectores, y guardan resultado en flag_diferencia=1

    pthread_exit(NULL);
}

void mergeParalelo(int id, int* vec, int* vecTemp){

    int offset=(id*N/T);

// Etapa 1 - cada Hilo ordena 1 segmento del vector
    iterativeSort(vec+offset, vecTemp+offset,N/T ); // todos los hilos ordenan un segmento del vector
#ifdef CHECK
    printf("check %d/%d: ", id, T);
    orderCheck(vec, offset, N / T); // verifica que su porcion de vector este ordenado
#endif

#ifdef DEBUG
    pthread_barrier_wait(&barrera);
    if(id==0) verVector(vec,N); // vector ordenado de a segmentos
#endif

// Etapa 2 - enbudo de ordenamiento
    pthread_barrier_wait(&barrera);
    for(int TActivos=T/2;TActivos>=1;TActivos/=2){ // indice es cantidad de hilos con trabajo activo
        if(id < TActivos){ //los nucleos activos (id<4) - ejecuta: T0,T1,T2,T3
       //     printf(" id:%d/%d -  offset:%d,blockZ:%d", id, TActivos,N*id/TActivos, N/(2*TActivos));
            offset=(id*N/TActivos);
            mergeBlocks(vec, vecTemp,offset, N/(TActivos*2) );
#ifdef CHECK
            printf("check %d/%d: ", id, TActivos);
            orderCheck(vec, offset, N / TActivos); // verifica que su porcion de vector este ordenado
#endif
        }
#ifdef DEBUG
        pthread_barrier_wait(&barrera);
        if (id == -1)    verVector(vec, N); // vector ordenado de a segmentos dobles
        printf(" \n --- \n");
#endif
        pthread_barrier_wait(&barrera);
    }

}

