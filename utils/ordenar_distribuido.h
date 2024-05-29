#ifndef ORDENAR_DISTRIBUIDO_H
#define ORDENAR_DISTRIBUIDO_H

#define MASTER_ID 0

#include <mpi.h>
#include "check.h"
#include "simple_init.h"
#include "ordenar_secuencial.h"

void master(int N, int K, int cantProcesos);
void slave(int N, int cantProcesos, int miID);

// proceso de ordenamiento colaborativo, cada proceso con su ID, con pasaje de mensajes
void ordenar_distribuido(int **vec, int **vecTemp, int BlockSize, int N, int id, int cantProcesos);

#endif