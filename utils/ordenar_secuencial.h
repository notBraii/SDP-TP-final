#ifndef ORDENAR_SECUENCIAL_H
#define ORDENAR_SECUENCIAL_H

#include <stdio.h>
// #define CHECK        // bandera para agregar ver los chequeos internos del short
// #include "check.h"

void iterativeSort(int *vec, int *tempvec, int Length); // ordena el vector con logica de merge-short iterativo
void mergeBlocks(int *vec, int *tempvec, int offset, int blockSize); //junta 2 vectores ordenados, los guarda contiguo al primero
void iterativeSortEficence(int** prtVec, int** ptrTempvec, int length); //merge-short con permuta de vectores
void mergeBlocksToOut(int* vec, int* vecOut, int offset, int blockSize);// toma los 2 vectores de entrada, los copia a la salida ordenados

#endif