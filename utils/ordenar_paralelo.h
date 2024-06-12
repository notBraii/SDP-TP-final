
#ifndef ORDENAR_PARALELO_H
#define ORDENAR_PARALELO_H


#include <pthread.h>                // Manejo de hilos
#include "check.h"                  // orderCheck(), compararVec()
#include "ordenar_secuencial.h"     // iterativeSortSwap() , mergeBlocksToOut()
#include "simple_init.h"
/**
 * Inicializa, crea, ejecuta y espera a que finalicen los hilos y elementos necesarios para su 
 * manipulación. 
 * Esta función trabaja con una copia de los vectores para aprovechar la memoria compartida, copias 
 * de las variables N y T que se manejan de manera global y una variable global del puntero al flag.
 * 
 * @param Vec_1             Vector 1
 * @param Vec_2             Vector 2
 * @param Vec_Temp          Vector temporal para operaciones intermedias
 * @param N                 Tamaño del vector
 * @param T                 Cantidad de hilos
 * @param flag_diferencia   Valor resultante de la comparación entre vectores, siendo 1 si encuentra 
 *                          diferencia o 0 en el caso contrario
 * @return none
*/
void ordenar_paralelo(int **Vec_1, int **Vec_2, int **Vec_Temp, int N, int T, int* flag_diferencia);

/**
 * Tareas que realiza el hilo:
 * -    Ordena vector 1
 * -    Ordena vector 2
 * -    Compara los vectores campo a campo
 * 
 * @param arg               Puntero que contiene el id del hilo
 * @return none
*/
void *taskThread(void *arg);

/**
 * @brief Ordena el vector trabajando con hilos en memoria compartida.
 * 
 * La siguiente función se basa en el principio del mergesort: cada hilo ordena de manera local el
 * bloque del vector asignado para luego unir ambas partes, según corresponda. La union ordenada de
 * distintas porciones del vector se realiza por niveles, en la cual los hilos van trabajando con la 
 * porción del vector del hilo contiguo a la derecha, duplicando el tamaño de la porción del vector a 
 * medida que avanza el nivel. De esta manera se paraleliza la unión de vectores entre distintos hilos.
 * 
 * Realiza el merge pasando por referencia los vectores, swapeando los punteros para evitar múltiples 
 * reasignaciones de valor.
 * 
 * @param id             
 * @param ptrVec            Puntero al vector
 * @param ptrVecTemp        Puntero al vector temporal para operaciones intermedias
 * @return none
*/
void mergeParalelo(int id, int** ptrVec, int** ptrVecTemp);

#endif