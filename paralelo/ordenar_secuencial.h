#ifndef ORDENAR_SECUENCIAL_H
#define ORDENAR_SECUENCIAL_H

#include <stdio.h>
// #define CHECK        // bandera para agregar ver los chequeos internos del short
// #include "check.h"

void iterativeSort(int *vec, int *tempvec, int Length); // ordena el vector con logica de merge-short iterativo
void mergeBlocks(int *vec, int *tempvec, int offset, int blockSize); //junta 2 vectores ordenados, los guarda contiguo al primero


#endif