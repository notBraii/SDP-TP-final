#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>      // Usado para:  printf()
#include <stdbool.h>    // Usado para:  variable bool
/**
 *  Imprime cada valor del vector en consola separados por un tab
 * @param v         Vector 
 * @param length    Longitud del vector
 * @return none
*/

void verVector(int *v, int length);

/**
 * Revisa el orden de los valores de un vector, de menor a mayor, devolviendo true si se comprueba su orden o false caso contrario
 * @param vec       Vector
 * @param offset    Posición inicial del vector
 * @param length    Longitud del vector que se quiere verificar
 * @return bool
*/

bool orderCheck(int *vec, int offset, int length);

/**
 * @brief Compara que los vectores pasados sean iguales campo a campo.
 * 
 * En caso de encontrar diferencia, setea el flag en 1. Caso contrario 0.
 * Para la siguiente función se asume que el offset válido se encuentra dentro del rango de los arreglos y la longitud no excede los rangos de los mismos.
 * @param vec1      Vector 1
 * @param vec2      Vector 2
 * @param offset    Posición de inicio en ambos vectores desde la cual empieza a comparar
 * @param length   Tamaño de los subvectores que se compara
 * @param flag      Puntero del Flag en el que se guarda el resultado de la comparación
 * @return none
*/
void compararVec(int *vec1, int *vec2, int offset, int length, int *flag);


#endif