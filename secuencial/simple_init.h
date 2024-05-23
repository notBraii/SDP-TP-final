
#ifndef SIMPLE_INIT_H
#define SIMPLE_INIT_H

#include <stdio.h> // printf
#include <stdlib.h> // exit
#include <stddef.h> // incluye NULL
#include <sys/time.h> // tiempo
#include <time.h>   // random seed
#include <math.h>   // pow

void extraerParams(int argc, char* argv[],int *N, int*K); //logica para obtener parametros y chequeos
void inicializarVectors(int *vec1, int *vec2, int length, int Kdiference); // inicializa vectores
double dwalltime(); // retorna el tiempo de ejecucion


#endif