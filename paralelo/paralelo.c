// consigna : 
//          verificar si 2 vectores contien los mismos numeros
// Estrategia de resolucion: 
//          ordenar ambos vectores y comparar posicion a posicion si estan todos los mismos numeros
//          a cada hilo T le toca ordenar un segmento del vector.
//          luego algunos hilos van mesclando los resultados ordenados.
// compilar con :
//          gcc -pthread -o paralelo paralelo.c simple_init.c check.c ordenar_secuencial.c -lm
// ejecutar (2^20 datos,4 hilos, 0 errores insertados) con :
//          ./secuencial 20 4 0

// bibliotecas
#include <pthread.h>    //manejo de threads
/*#include <stdlib.h>     // exit
#include <stdio.h>      // printf
#include <sys/time.h>   // dwalltime
#include <stddef.h>     // NULL
#include <math.h>       // log2
#include <time.h>       // random seed
*/

#include "simple_init.h"                  // extraerParamsNTK(), inicializarVectors(), dwalltime()
#include "check.h"                        // verVector(), orderCheck(), compararVec()
#include "ordenar_secuencial.h"           // iterativeSort() , mergeBlocks()

#define MIN(A, B) ((A) < (B) ? (A) : (B)) // macro numero minimo


// #define DEBUG
// #define CHECK

// Macros
#define MIN(A, B) ((A) < (B) ? (A) : (B)) //Devuelve el mínimo

// Prototipos de funcion
void *taskThread(void *arg);                        // programa principal de cada Hilo
void mergeParalelo(int id, int* vec, int* vecTemp); // funcion principal de ordenar paralelo


// Variables compartidas
pthread_barrier_t barrera;
int N; //Tamaño del vector
int T; //Cantidad de hilos
int K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int flag_diferencia = 0; // flag deteccion de diferencias

int main(int argc, char* argv[]){

    extraerParamsNTK(argc, argv,&N,&T,&K);

    int i;
    pthread_t hilos[T];
    int ids[T];

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));

    pthread_barrier_init(&barrera, NULL, T);//inicializa la barrera global

    inicializarVectors(V1,V2,N,K);

    double t0 = dwalltime();

    // Crear los hilos
    for (i = 0; i < T; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, taskThread, &ids[i]); //manda a ejecutar tarea

    }

    // Esperar a que todos los hilos terminen
    for (i = 0; i < T; i++) {
        pthread_join(hilos[i], NULL);
    }


    printf("Para N:%d, T:%d , tardo %f segundos\n", N,T, dwalltime()- t0);

    if (flag_diferencia)
        printf("\t - Hay diferencia entre los vectores \n");
    else
        printf("\t - Los vectores son iguales \n");

    #ifdef CHECK
        printf("\n check V1: "); orderCheck(V1, 0, N);
        printf("\n check V2: "); orderCheck(V2, 0, N);
    #endif

    pthread_barrier_destroy(&barrera);
    free(V1);
    free(V2);
    free(Vtemp);
    return 0;
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

