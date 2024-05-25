

#include "ordenar_paralelo.h"
#define ELEMENTS_N_BY_THREAD (Nlong/Thilos)
#define OFFSET_N_ID_BY_THREAD (id*Nlong/Thilos)
#define BLOCKSIZE_PORTION_BY_THREAD ((Nlong* porciones)/(Thilos*2))


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
    int id = *((int*)arg);

    mergeParalelo(id,Vec1,VecTemp);
    mergeParalelo(id,Vec2,VecTemp);

// todos los hilos comparan un pedazo de los vectores, y guardan resultado en flag_diferencia=1
    compararVec(Vec1,Vec2, OFFSET_N_ID_BY_THREAD, ELEMENTS_N_BY_THREAD,flagDif); 

    pthread_exit(NULL);
}

void mergeParalelo(int id, int* vec, int* vecTemp){

// Etapa 1 - cada Hilo ordena 1 segmento del vector
    iterativeSort(vec + OFFSET_N_ID_BY_THREAD, vecTemp + OFFSET_N_ID_BY_THREAD, ELEMENTS_N_BY_THREAD );

// Etapa 2 - embudo de ordenamiento
    pthread_barrier_wait(&barrera);

//Se combinan vectores contiguos
    for (int porciones=2; porciones <= Thilos; porciones *= 2){
        
    //Si el id mod porciones es 0, el hilo trabaja
        if (id % porciones == 0){
            mergeBlocks(vec, vecTemp,OFFSET_N_ID_BY_THREAD, BLOCKSIZE_PORTION_BY_THREAD);
        }
    
    //Se espera a que todos los hilos terminen de trabajar para pasar al siguiente nivel
	    pthread_barrier_wait(&barrera);
    }

}

