
#include "ordenar_secuencial.h"
#define WO_FUNCTION

void iterativeSortSwap(int** prtVec, int** ptrTempvec,int offset, int length) {
    int temp;
    int *vec=  (*prtVec) + offset;
    int *vecOut = (*ptrTempvec) + offset;// guardo sub-referencia
    int *vect;

    // ordena los elementos de a pares
    for (int indice = 0; indice < length - 1; indice += 2) {
        if (vec[indice] > vec[indice + 1]) {
            // permuta los numeros que no estan ordenados de a pares
            temp = vec[indice];
            vec[indice] = vec[indice + 1];
            vec[indice + 1] = temp;
        }
    }
    // Mezcla incrementando la longitud de los bloques
    for (int blockSize = 2; blockSize <= length / 2; blockSize *= 2) {
        for (int indice = 0; indice < length - 1; indice += (blockSize * 2)) {
   
            int *firstBlockPtr = vec + indice;
            int *secondBlockPtr = firstBlockPtr + blockSize;

            int *vecOutPtr = vecOut + indice;
            
            int *firstBlockPtr_end = secondBlockPtr;
            int *secondBlockPtr_end = secondBlockPtr + blockSize;
    
            // mientras ningun indice llegue al final compara y guarda el menor
            while (firstBlockPtr < firstBlockPtr_end && secondBlockPtr < secondBlockPtr_end) {
                if (*firstBlockPtr <= *secondBlockPtr) {
                    *vecOutPtr++ = *firstBlockPtr ++;
                } else {
                    *vecOutPtr++ = *secondBlockPtr ++;
                }
            }
            while (firstBlockPtr < firstBlockPtr_end) {
                *vecOutPtr++ = *firstBlockPtr ++;
            }
            while (secondBlockPtr < secondBlockPtr_end) {
                *vecOutPtr++ = *secondBlockPtr ++;
            }
        }
        
        // por cada nivel de ordenacion permuta los punteros
        vect = vec;
        vec = vecOut;
        vecOut = vect;
    }
    // al finalizar el algoritmo, guarda el estado de los punteros de trabajo en los punteros de entrada
    if(offset == 0){
        *prtVec = vec;
        *ptrTempvec = vecOut;
    }
}

void mergeBlocksToOut(int* vec, int* vecOut, int offset, int blockSize) { 
    int *firstBlockPtr = vec + offset;
    int *secondBlockPtr = firstBlockPtr + blockSize;
    int *vecOutPtr = vecOut + offset;
    int *firstBlockPtr_end = secondBlockPtr;
    int *secondBlockPtr_end = secondBlockPtr + blockSize;

    // mientras ningun indice llegue al final / compara y guarda el menor
    while (firstBlockPtr < firstBlockPtr_end && secondBlockPtr < secondBlockPtr_end) {
        if (*firstBlockPtr <= *secondBlockPtr) {
            *vecOutPtr++ = *firstBlockPtr ++;
        } else {
            *vecOutPtr++ = *secondBlockPtr ++;
        }
    }
    while (firstBlockPtr < firstBlockPtr_end) {
        *vecOutPtr++ = *firstBlockPtr ++;
    }
    while (secondBlockPtr < secondBlockPtr_end) {
        *vecOutPtr++ = *secondBlockPtr ++;
    }
}
