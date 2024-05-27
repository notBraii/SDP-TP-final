
#ifndef ORDENAR_PARALELO_H
#define ORDENAR_PARALELO_H


#include <pthread.h> //manejo de threads
#include "check.h"                        // verVector(), orderCheck(), compararVec()
#include "ordenar_secuencial.h"           // iterativeSort() , mergeBlocks()

void ordenar_paralelo(int **Vec_1, int **Vec_2, int **Vec_Temp, int N_long, int T_hilos, int* flag_diferencia);
void *taskThread(void *arg); // prototipo de funcion principal de cada hilo
void mergeParalelo(int id, int** ptrVec, int** ptrVecTemp);

#endif