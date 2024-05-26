// bibliotecas
#include "utils/simple_init.h"                  // extraerParamsNTK(), inicializarVectors(), dwalltime()
#include "utils/ordenar_distribuido.h"

// Variables compartidas
int N;                   // Tamaño del vector
int T;                   // Cantidad de hilos
int K;                   // Cantidad de errores
int flag_diferencia = 0; // flag deteccion de diferencias
int *V1;                 // Arreglo 1 con valores
int *V2;                 // Arreglo 2 con valores
int *Vtemp;              // Arreglo temporal para ordenar

int main(int argc, char* argv[]){

    extraerParamsNTK(argc, argv,&N,&T,&K);

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));


    inicializarVectors(V1,V2,N,K);
    double t0 = dwalltime();

    ordenar_paralelo(&V1,&V2,&Vtemp,N,T,&flag_diferencia);

    // obtiene el tiempo de ejecucion de la tarea, y la imprime
    printf("Para N:%d, T:%d , tardo %f segundos\n", N,T, dwalltime()- t0);

    // imprime el resultado de si los vectores son iguales o distintos
    if (flag_diferencia) printf("\n \t - Hay diferencia entre los vectores \n");
    else  printf("\n \t - Los vectores son iguales \n");

    // chequear ambos vectores 
    printf("el Vector1: %s\n", orderCheck(V1, 0, N) ? "esta ordenado correctamente": " ");
    printf("el Vector2: %s\n", orderCheck(V2, 0, N) ? "esta ordenado correctamente": " ");

    free(V1);
    free(V2);
    free(Vtemp);
    return 0;
}