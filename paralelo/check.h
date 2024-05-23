#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>

void verVector(int *v, int lengt);// imprime el vector por consola
void orderCheck(int *vec, int offset, int length); // escribe un error cuando, encuentra numeros no ordenados de menor a mayor
void compararVec(int *vec1, int *vec2, int offset, int NLength, int *flag_diference); // setea el flag si encuentra una diferencia


#endif