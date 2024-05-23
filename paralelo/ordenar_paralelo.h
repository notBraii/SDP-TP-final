
#ifndef ORDENAR_PARALELO_H
#define ORDENAR_PARALELO_H


#include <pthread.h> //manejo de threads
#include "check.h"                        // verVector(), orderCheck(), compararVec()
#include "ordenar_secuencial.h"           // iterativeSort() , mergeBlocks()

// Variables compartidas
int N;                   // Tamaño del vector
int T;                   // Cantidad de hilos
int K;                   // Cantidad de errores
int flag_diferencia = 0; // flag deteccion de diferencias
int *V1;    // Arreglo 1 con valores
int *V2;    // Arreglo 2 con valores
int *Vtemp; // Arreglo temporal para ordenar
pthread_barrier_t barrera;

void *taskThread(void *arg);                        // programa principal de cada Hilo
void mergeParalelo(int id, int *vec, int *vecTemp); // funcion principal de ordenar paralelo

#endif