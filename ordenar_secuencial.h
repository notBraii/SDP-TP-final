#ifndef ORDENAR_SECUENCIAL_H
#define ORDENAR_SECUENCIAL_H

#include <stdio.h>
// #define CHECK

void iterativeSort(int *vec, int *tempvec, int Length); // ordena el vector con logica de merge-short iterativo
void mergeBlocks(int *vec, int *tempvec, int offset, int blockSize); //junta 2 vectores ordenados, los guarda contiguo al primero


#endif