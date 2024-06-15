#ifndef ORDENAR_SECUENCIAL_H
#define ORDENAR_SECUENCIAL_H

#include <stdio.h>
// #define CHECK        // bandera para agregar los chequeos internos del short

/**
 * @brief Implementación del algoritmo de ordenación MergeSort.
 * 
 * En esta versión se utiliza un vector temporal pasado por parámetro para operaciones intermedias, 
 * la mezcla de los subvectores se realiza de manera iterativa y se trabaja con punteros para 
 * utilizar operaciones de bajo nivel y optimizar el tiempo de las operaciones realizadas. Arranca 
 * ordenando primero de a pares, permutando los valores que no están ordenados. Luego, se incrementa 
 * el tamaño de los bloques de comparación para ir mezclando ordenadamente. El tamaño nuevo del 
 * bloque será del doble del tamaño anterior, ya que los valores utilizados serán los valores de los 
 * subarreglos que fueron ordenados en la iteración previa. Por cada tamaño de bloque se van 
 * mezclando ordenadamente los bloques contiguos guardando el resultado en un vector temporal. Al 
 * finalizar con todos los bloques del mismo tamaño, se intercambian los punteros del vector original 
 * y el temporal para proceder con la siguiente iteración con bloques más grandes. Esto se repite 
 * hasta que el tamaño del bloque sea la mitad del tamaño del vector pasado por parámetro. 
 * 
 * @param ptrVec        Puntero al vector que se quiere ordenar
 * @param ptrTempvec    Puntero al vector temporal para operaciones intermedias
 * @param offset        Posición de inicio en ambos vectores
 * @param length        Tamaño del vector vec
 * @return none
*/
void iterativeSortSwap(int** prtVec, int** ptrTempvec, int offset, int length);

/**
 * @brief Realiza mezcla ordenada de vectores.
 * 
 * Recibe 1 vector que posee 2 bloques con los valores ordenados y los mezcla de forma ordenada, 
 * devolviendo el vector ordenado resultante en vecOut. Los bloques del vector deben estar contiguos, 
 * el vector original y temporal deben tener la misma longitud, en la cual ambos empiezan a trabajar 
 * desde la posición indicada en el offset. No se realizan las validaciones de parámetros 
 * correspondientes.
 * 
 * @param vec           Vector original
 * @param vecOut        Vector resultante ordenado
 * @param offset        Posición inicial del primer bloque
 * @param blocksize     Tamaño de cada bloque
 * @return none
 */ 
void mergeBlocksToOut(int* vec, int* vecOut, int offset, int blockSize);

#endif