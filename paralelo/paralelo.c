// consigna : 
//          verificar si 2 vectores contien los mismos numeros
// Estrategia de resolucion: 
//          ordenar ambos vectores y comparar posicion a posicion si estan todos los mismos numeros
//          a cada hilo T le toca ordenar un segmento del vector.
//          luego algunos hilos van mesclando los resultados ordenados.
// compilar con :
//          gcc -pthread -o paralelo paralelo.c simple_init.c check.c ordenar_secuencial.c ordenar_paralelo.c -lm
// ejecutar (2^20 datos, 4 hilos, 0 errores insertados) con :
//          ./paralelo 20 4 0


// bibliotecas
#include "simple_init.h"                  // extraerParamsNTK(), inicializarVectors(), dwalltime()
#include "ordenar_paralelo.h"

// variables compartidas dentro de :    ordenar_paralelo.h
//      N, T, K, flag_diferencia
//      V1, V2, Vtemp

int main(int argc, char* argv[]){

    extraerParamsNTK(argc, argv,&N,&T,&K);

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));

    int ids[T];
    pthread_t hilos[T];
    pthread_barrier_init(&barrera, NULL, T);//inicializa la barrera global

    inicializarVectors(V1,V2,N,K);
    double t0 = dwalltime();

    // Crear los hilos
    for (int i = 0; i < T; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, taskThread, &ids[i]); // manda a ejecutar tarea
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < T; i++) {
        pthread_join(hilos[i], NULL);
    }

    // obtiene el tiempo de ejecucion de la tarea, y la imprime
    printf("Para N:%d, T:%d , tardo %f segundos\n", N,T, dwalltime()- t0);

    // imprime el resultado de si los vectores son iguales o distintos
    if (flag_diferencia) printf("\n \t - Hay diferencia entre los vectores \n");
    else  printf("\n \t - Los vectores son iguales \n");

    // chequear ambos vectores 
    printf("el Vector1: %s\n", orderCheck(V1, 0, N) ? "esta ordenado correctamente": " ");
    printf("el Vector2: %s\n", orderCheck(V2, 0, N) ? "esta ordenado correctamente": " ");

    pthread_barrier_destroy(&barrera);
    free(V1);
    free(V2);
    free(Vtemp);
    return 0;
}

