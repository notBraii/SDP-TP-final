
#ifndef SIMPLE_INIT_H
#define SIMPLE_INIT_H

#define MASTER_ID 0

#include <stdio.h> // printf
#include <stdlib.h> // exit
#include <stddef.h> // incluye NULL
#include <sys/time.h> // tiempo
#include <time.h>   // random seed
#include <math.h>   // pow

/**
 * Valida la cantidad de parámetros y modifica variables globales de N, K, delegando la tarea al master de informar en caso de haber error.
 * 
 * @param argc          Cantidad de argumentos
 * @param argv          Arreglo con los parámetros pasados 
 * @param N             Tamaño total del vector
 * @param K             Cantidad de errores a insertar
 * @param miID          ID del proceso que llama a la función
 * @return int el cual indica el valor de retorno: 0 si se realizó con éxito u otro valor distinto en el caso contrario
*/
int extraerParamsMPI(int argc, char *argv[],int *N, int *K, int miID);

/**
 * Valida la cantidad de parámetros y modifica variables globales de N, T, K
 * 
 * @param argc          Cantidad de argumentos
 * @param argv          Arreglo con los parámetros pasados 
 * @param N             Tamaño total del vector
 * @param T             Cantidad de hilos
 * @param K             Cantidad de errores a insertar
 * @return none
*/
void extraerParamsNTK(int argc, char *argv[], int *N, int *T, int *K);

/**
 * Valida la cantidad de parámetros y modifica variables globales de N, K
 * 
 * @param argc          Cantidad de argumentos
 * @param argv          Arreglo con los parámetros pasados 
 * @param N             Tamaño total del vector
 * @param K             Cantidad de errores a insertar
 * @return none
*/
void extraerParams(int argc, char *argv[],int *N, int*K);

/**
 * Inicializa los vectores globales con valores aleatorios iguales en ambos vectores, agregando K valores diferentes en posiciones aleatorias del vector 2. Considera que ambos vectores son del mismo tamaño.
 * 
 * @param vec1          vector 1
 * @param vec2          vector 2
 * @param length        longitud de ambos vectores
 * @param Kdiference    Cantidad de diferencias insertadas
 * @return none
*/
void inicializarVectors(int *vec1, int *vec2, int length, int Kdiference);

/**
 * Función que retorna la hora del día en segundos
 * 
 * @return double
*/
double dwalltime();


#endif