#ifndef ORDENAR_DISTRIBUIDO_H
#define ORDENAR_DISTRIBUIDO_H

#define MASTER_ID 0

#include <mpi.h>                    // Definición y comunicación de procesos
#include "simple_init.h"            // inicializarVectores(), dwalltime(), llamado a stdlib.h
#include "check.h"                  // orderCheck()
#include "ordenar_secuencial.h"     // iterativeSortSwap(), mergeBlocksToOut()

/**
 * Operaciones del master
 * 
 * @param N             Tamaño del vector
 * @param K             Indica la cantidad de valores distintos del vector 2
 * @param cantProcesos  Número de procesos activos
 * @return none
*/
void master(int N, int K, int cantProcesos);

/**
 * Operaciones del worker/slave
 * 
 * @param N             Tamaño del vector
 * @param cantProcesos  Número de procesos activos
 * @param miID          ID del proceso que utiliza la función
 * @return none
*/
void slave(int N, int cantProcesos, int miID);

/**
 * @brief Ordena el vector aprovechando el pasaje de mensajes.
 * 
 * La siguiente función se basa en el principio del mergesort: se ordena de manera local la parte de 
 * un vector global asignada al proceso para luego enviar su parte al proceso contiguo o recibir la 
 * parte del proceso contiguo y unir ambas partes, según corresponda. De esta manera se distribuyen 
 * las operaciones de unión entre los distintos procesos.
 * 
 * @param vec           Puntero al bloque del arreglo asignado al proceso
 * @param vectTemp      Puntero al vector auxiliar utilizado
 * @param BlockSize     Tamaño del arreglo vec pasado por parámetro
 * @param N             Tamaño del arreglo original
 * @param id            ID del proceso que utiliza la función
 * @param cantProcesos  Número de procesos activos
 * @return none
*/
void ordenar_distribuido(int **vec, int **vecTemp, int BlockSize, int N, int id, int cantProcesos);

#endif