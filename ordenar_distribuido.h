#ifndef ORDENAR_DISTRIBUIDO_H
#define ORDENAR_DISTRIBUIDO_H

#include <mpi.h>
#include "ordenar_paralelo.h" //Ante la duda
#include "ordenar_secuencial.h" //Ante la duda

// #define CHECK        // bandera para agregar ver los chequeos internos del short
// #include "check.h"

//Tarea que realiza el master
void master(int N, int cantProcesos);

//Tarea que realizan el resto de las computadoras
void slave(int N, int cantProcesos, int miID); 

#endif