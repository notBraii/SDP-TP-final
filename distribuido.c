// bibliotecas
//#include "utils/simple_init.h"                  // extraerParamsNTK(), inicializarVectors(), dwalltime()
//#include "utils/ordenar_distribuido.h"
#include <stdio.h> // printf
#include <stdlib.h> // exit
#include <stddef.h> // incluye NULL
#include <sys/time.h> // tiempo
#include <time.h>   // random seed
#include <math.h>   // pow
#include <mpi.h>

#define MASTER 0
// Variables
/*
int N;                   // Tamaño del vector
int T;                   // Cantidad de hilos
int K;                   // Cantidad de errores
int flag_diferencia = 0; // flag deteccion de diferencias
int *V1;                 // Arreglo 1 con valores
int *V2;                 // Arreglo 2 con valores
int *Vtemp;              // Arreglo temporal para ordenar
*/

//Prototipos

int main(int argc, char* argv[]){
    
    int miID,
        cantProcesos,
        N,
        K;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &miID);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    //Modificar extraer params para que se modifique parámetros por referencia, además de finalizar el MPI en caso de tirar error
    extraerParams(argc, argv, &N, &K);

    if(miID == MASTER){
        //DENTRO DEL MASTER DEBEN IR LAS INICIALIZACIONES DE VECTORES, Y LA LÓGICA DE DISTRIBUCIÓN
        master();
    }
    else{
        //SOLO RECIBE POR PARÁMETROS EN LOS MENSAJES SU PORCIÓN ASIGNADA DEL VECTOR, COMUNICACIÓN CONSTANTE CON EL MASTER
        slave();
    }

    MPI_FINALIZE();
    return (0);
}
/*
Adaptar esto dentro del master, ya que es el que hará de hilo main para lo que es la inicialización, carga y dirección de los vectores
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

*/
//
