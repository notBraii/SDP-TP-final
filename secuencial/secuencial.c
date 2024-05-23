
// ORDENAR 2 VECTORES Y COMPARAR SI CONTIENEN LOS MISMOS ELEMENTOS
// compilar con
//      gcc -pthread -o secuencial secuencial.c simple_init.c check.c ordenar_secuencial.c -lm
// ejecutar (2^20 datos, 0 errores insertados) con
//      ./secuencial 20 0

#include "simple_init.h" // extraerParams(), inicializarVectors(), dwalltime()
#include "check.h" // verVector(), orderCheck(), compararVec()
#include "ordenar_secuencial.h" // iterativeSort() , mergeBlocks()
#define MIN(A, B) ((A) < (B) ? (A) : (B)) // macro numero minimo

// Prototipos de funcion

// Variables compartidas
int NLength; // Tamaño del vector
int Kdif; //Cantidad de diferencias insertadas
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int diferencia=0; // flag deteccion de diferencias


// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv, &NLength, &Kdif);

    V1 = (int *)malloc(NLength * sizeof(int));
    V2 = (int *)malloc(NLength * sizeof(int));
    Vtemp = (int *)malloc(NLength * sizeof(int));

    inicializarVectors(V1, V2, NLength, Kdif); // inicializa V1,V2 de largo N, con los K diferencias aleatorias

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    iterativeSort(V1,Vtemp,NLength);
    iterativeSort(V2,Vtemp,NLength);
    compararVec(V1,V2,0,NLength,&diferencia);

    printf("Para N=%d, tiempo de ejecucion %f segundos\n", NLength, dwalltime() - t0);

    printf("\n check V1: "); orderCheck(V1, 0, NLength);
    printf("\n check V2: "); orderCheck(V2, 0, NLength);

    if (diferencia)
      printf("\t - Hay diferencia entre los vectores \n");
    else
		  printf("\t - Los vectores son iguales \n");
  
  
    // liberar memoria
    free(V1);
    free(V2);
    free(Vtemp);

    return 0;
}
