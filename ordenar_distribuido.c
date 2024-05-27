#include "ordenar_distribuido.h"

void master(int N, int cantProcesos){
    int *V1;                 // Arreglo 1 con valores
    int *V2;                 // Arreglo 2 con valores
    int *Vtemp;              // Arreglo temporal para ordenar

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));


    inicializarVectors(V1,V2,N,K);
    double t0 = dwalltime();
}

void slave(int N, int cantProcesos, int miID){

}
