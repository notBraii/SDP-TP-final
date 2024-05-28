#ifndef ORDENAR_DISTRIBUIDO_H
#define ORDENAR_DISTRIBUIDO_H

#define MASTER_ID 0

#include <mpi.h>
#include "simple_init.h"
#include "ordenar_paralelo.h" //Ante la duda
#include "ordenar_secuencial.h" //Ante la duda

// #define CHECK        // bandera para agregar ver los chequeos internos del short
// #include "check.h"


void extraerParamsMPI(int argc, char* argv[],int *N, int*K);

void master(int N, int K, int cantProcesos);

//Tarea que realizan el resto de las computadoras
void slave(int N, int cantProcesos, int miID); 

#endif