

#include "ordenar_paralelo.h"
#define ELEMENTS_N_BY_THREAD (Nlong/Thilos)
#define OFFSET_N_ID_BY_THREAD ((Nlong/Thilos)*id)
#define BLOCKSIZE_PORTION_BY_THREAD ((Nlong/(Thilos*2))* porciones)


int *Vec1,*Vec2,*VecTemp; // copia de las variables del main
int Nlong,Thilos;
int *flagDif;
pthread_barrier_t barrera;

void *taskThread(void *arg); // prototipo de funcion principal de cada hilo
void mergeParalelo(int id, int** ptrVec, int** ptrVecTemp);


void ordenar_paralelo(int **Vec_1, int **Vec_2, int **Vec_Temp, int N_long, int T_hilos, int* flag_diferencia){
    // copio los valores al contexto de la libreria, con memoria compartida
    Vec1=*Vec_1;  Vec2=*Vec_2;  VecTemp=*Vec_Temp;    Nlong=N_long;    Thilos=T_hilos; flagDif=flag_diferencia;

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

    //Se guardan los punteros ordenados en los vectores originales 
    *Vec_1=Vec1;  
    *Vec_2=Vec2;
    *Vec_Temp=VecTemp;

}

void *taskThread(void *arg) {
    int id = *((int*)arg);

    mergeParalelo(id,&Vec1,&VecTemp);
    mergeParalelo(id,&Vec2,&VecTemp);

// todos los hilos comparan un pedazo de los vectores, y guardan resultado en flag_diferencia=1
    compararVec(Vec1,Vec2, OFFSET_N_ID_BY_THREAD, ELEMENTS_N_BY_THREAD,flagDif); 

    pthread_exit(NULL);
}

void mergeParalelo(int id, int** ptrVec, int** ptrVecTemp){
    int offsetID=OFFSET_N_ID_BY_THREAD;

// Etapa 1 - cada Hilo ordena 1 segmento del vector
    iterativeSortSwap(ptrVec,ptrVecTemp,offsetID , ELEMENTS_N_BY_THREAD );

// Etapa 2 - embudo de ordenamiento
    pthread_barrier_wait(&barrera);

    int* vec= *ptrVec;// los vectores de trabajo privados
    int* vecOut= *ptrVecTemp;

    //Se combinan vectores contiguos
    for (int porciones=2; porciones <= Thilos; porciones *= 2){
        //Si el id mod porciones es 0, el hilo trabaja
        if (id % porciones == 0){
            mergeBlocksToOut(vec, vecOut,offsetID, BLOCKSIZE_PORTION_BY_THREAD); //ordeno 1 nivel
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

