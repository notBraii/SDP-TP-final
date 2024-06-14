// consigna : 
//          verificar si 2 vectores contienen los mismos números
// Estrategia de resolución: 
//          ordenar ambos vectores y comparar posición a posición si estan todos los mismos números
//          a cada proceso le toca ordenar un segmento del vector.
//          luego algunos procesos van mezclando los resultados ordenados para mayor paralelismo.
//          Para la comparación se sigue la misma idea: se reparten porciones de V1 y V2 a cada proceso y compara localmente
//          Para obtener el resultado al final de cada comparación del proceso se hace una reducción buscando el valor máximo de las diferencias encontradas.

// compilar con :
//          mpicc -Iinclude -o distribuido distribuido.c lib/simple_init.c lib/ordenar_secuencial.c lib/check.c lib/ordenar_distribuido.c -lm
// ejecutar (2^20 datos, 2 procesos, 0 errores insertados) con :
//          mpirun -np 2 distribuido 20 0

// bibliotecas
#include "simple_init.h"              // extraerParamsMPI()
#include "ordenar_distribuido.h"      // master(), slave(), llamado a mpi.h 

#define MASTER_ID 0

int main(int argc, char* argv[]){
    int miID,
        cantProcesos,
        N,
        K;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &miID);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    int error_parametros = extraerParamsMPI(argc, argv, &N, &K, miID);
    if(error_parametros){
        MPI_Finalize();
        return (error_parametros);
    }

    if(miID == MASTER_ID){
        master(N, K, cantProcesos);
    }
    else{
        slave(N, cantProcesos, miID);
    }

    MPI_Finalize();
    return (0);
}

