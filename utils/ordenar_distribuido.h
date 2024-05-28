#ifndef ORDENAR_DISTRIBUIDO_H
#define ORDENAR_DISTRIBUIDO_H

#define MASTER_ID 0

#include <mpi.h>
#include "check.h"
#include "simple_init.h"
#include "ordenar_secuencial.h"

void master(int N, int K, int cantProcesos);

void slave(int N, int cantProcesos, int miID); 

#endif