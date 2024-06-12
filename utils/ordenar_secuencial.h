#ifndef ORDENAR_SECUENCIAL_H
#define ORDENAR_SECUENCIAL_H

#include <stdio.h>
// #define CHECK        // bandera para agregar los chequeos internos del short

/**
 * Merge-sort iterativo.
 * 
 * @param vec           Vector a ordenar
 * @param tempvec       Vector temporal para operaciones intermedias
 * @param length        Longitud del vector
 * @return none
*/
void iterativeSort(int *vec, int *tempvec, int length);

/**
 * Recibe un vector con 2 bloques a los cuales se les realiza la mezcla ordenada de sus campos. No se tiene en cuenta la validación de parámetros con respecto al vector
 * 
 * @param vec           Vector original
 * @param tempvec       Vector temporal
 * @param offset        Posición inicial del primer bloque
 * @param blocksize     Tamaño de cada bloque
 * @return none
*/
void mergeBlocks(int *vec, int *tempvec, int offset, int blockSize);

/**
 * merge-short con permutación de vectores
 * funcion silimar al ordenar iterativo, pero opimiza en el copiado a memoria
 * cada paso de ordenacion, permuta los vectores ordenando y ordenado, para evitar copiar todos los datos
 * 
 * @param ptrVec        
 * @param ptrTempvec    
 * @param offset        
 * @param length        
 * @return none
*/
void iterativeSortSwap(int** prtVec, int** ptrTempvec, int offset, int length);

/**
 * Recibe 2 bloques de numeros ordenados y los junta de forma ordenada (optimizado)
 * toma los 2 vectores de entrada, los copia a la salida ordenados
 *
 */ 
void mergeBlocksToOut(int* vec, int* vecOut, int offset, int blockSize);

#endif