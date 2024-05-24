

#include "ordenar_paralelo.h"
#define ELEMENTS_N_ID_BY_THREADACTIVE (id*Nlong/TActivos)
#define ELEMENTS_N_ID_BY_THREAD (id*Nlong/Thilos)
#define OFFSET_N_BY_THREAD (Nlong/Thilos)
#define OFFSET_N_BY_THREADACTIVE (Nlong/(TActivos*2))


int *Vec1,*Vec2,*VecTemp; // copia de las variables del main
int Nlong,Thilos;
int *flagDif;
pthread_barrier_t barrera;

void *taskThread(void *arg); // prototipo de funcion principal de cada hilo
void mergeParalelo(int id, int* vec, int* vecTemp);


void ordenar_paralelo(int *Vec_1, int *Vec_2, int *Vec_Temp, int N_long, int T_hilos, int* flag_diferencia){
    // copio los valores al contexto de la libreria, con memoria compartida
    Vec1=Vec_1;  Vec2=Vec_2;  VecTemp=Vec_Temp;    Nlong=N_long;    Thilos=T_hilos; flagDif=flag_diferencia;

    int ids[Thilos];
    pthread_t hilos[Thilos];
    pthread_barrier_init(&barrera, NULL, Thilos); // inicializa la barrera global

    for (int i = 0; i<Thilos ; i++) {   // Crear los hilos
        ids[i] = i;
        pthread_create(&hilos[i], NULL, taskThread, &ids[i]); // manda a ejecutar tarea
    }

    for (int i = 0; i < Thilos; i++) {    // Esperar a que todos los hilos terminen
        pthread_join(hilos[i], NULL);
    }
    pthread_barrier_destroy(&barrera);
}

void *taskThread(void *arg) {
    int indice;
    int id = *((int*)arg);

    mergeParalelo(id,Vec1,VecTemp);
    mergeParalelo(id,Vec2,VecTemp);

    compararVec(Vec1,Vec2, ELEMENTS_N_ID_BY_THREAD, OFFSET_N_BY_THREAD,flagDif); // todos los hilos comparan un pedazo de los vectores, y guardan resultado en flag_diferencia=1

    pthread_exit(NULL);
}

void mergeParalelo(int id, int* vec, int* vecTemp){

// Etapa 1 - cada Hilo ordena 1 segmento del vector
    iterativeSort(vec+ELEMENTS_N_ID_BY_THREAD, vecTemp+ELEMENTS_N_ID_BY_THREAD,OFFSET_N_BY_THREAD );

// Etapa 2 - enbudo de ordenamiento
    pthread_barrier_wait(&barrera);
    for(int TActivos=Thilos/2 ; TActivos>=1 ; TActivos/=2){ // indice es cantidad de hilos con trabajo activo
        if(id < TActivos){ //los nucleos activos (id<4) - ejecuta: T0,T1,T2,T3
       //     printf(" id:%d/%d -  offset:%d,blockZ:%d", id, TActivos,N*id/TActivos, N/(2*TActivos));
            mergeBlocks(vec, vecTemp,OFFSET_N_BY_THREADACTIVE, ELEMENTS_N_ID_BY_THREADACTIVE);
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

