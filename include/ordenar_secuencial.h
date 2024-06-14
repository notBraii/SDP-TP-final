#ifndef ORDENAR_SECUENCIAL_H
#define ORDENAR_SECUENCIAL_H

#include <stdio.h>
// #define CHECK        // bandera para agregar los chequeos internos del short

/**
 * Recibe un vector con 2 bloques a los cuales se les realiza la mezcla ordenada de sus campos. No se tiene en cuenta la validación de parámetros con respecto al vector
 * 

*/
void mergeBlocks(int *vec, int *tempvec, int offset, int blockSize);

/**
 * @brief Implementación del algoritmo de ordenación MergeSort.
 * 
 * 
 * @param ptrVec        
 * @param ptrTempvec    
 * @param offset        
 * @param length        
 * @return none
*/
void iterativeSortSwap(int** prtVec, int** ptrTempvec, int offset, int length);

/**
 * @brief Realiza merge de vectores.
 * 
 * Recibe 1 vector que posee 2 bloques de vectores con los valores ordenados y los junta de forma ordenada, devolviendo el vector unido y ordenado en vecOut. Se tiene en cuenta 
 * 
 * @param vec           Vector original
 * @param vecOut        Vector resultante ordenado
 * @param offset        Posición inicial del primer bloque
 * @param blocksize     Tamaño de cada bloque
 * @return none
 */ 
void mergeBlocksToOut(int* vec, int* vecOut, int offset, int blockSize);

#endif