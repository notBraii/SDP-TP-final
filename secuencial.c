
// ORDENAR 2 VECTORES Y COMPARAR SI CONTIENEN LOS MISMOS ELEMENTOS
// compilar con
//      gcc -o secuencial secuencial.c utils/simple_init.c utils/check.c utils/ordenar_secuencial.c -lm
// ejecutar (2^20 datos, 0 errores insertados) con
//      ./secuencial 20 0

#include "utils/simple_init.h" // extraerParams(), inicializarVectors(), dwalltime()
#include "utils/check.h" // verVector(), orderCheck(), compararVec()
#include "utils/ordenar_secuencial.h" // iterativeSort() , mergeBlocks()

// Prototipos de funcion

// Variables compartidas
int NLength; // Tamaño del vector
int Kdif; //Cantidad de diferencias insertadas
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int flag_diferencia=0; // flag deteccion de diferencias


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

    iterativeSortSwap(&V1,&Vtemp,0,NLength);
    iterativeSortSwap(&V2,&Vtemp,0,NLength);
    compararVec(V1,V2,0,NLength,&flag_diferencia);

    printf("Para N=%d, tiempo de ejecucion %f segundos \n \n", NLength, dwalltime() - t0);

    if (flag_diferencia)
      printf("\t - Hay diferencia entre los vectores \n");
    else
      printf("\t - Los vectores son iguales \n");

    // chequear ambos vectores
    printf("- el Vector1: %s\n", orderCheck(V1,0,NLength) ? "esta ordenado correctamente" : " ");
    printf("- el Vector2: %s\n", orderCheck(V2,0,NLength) ? "esta ordenado correctamente" : " ");
  
  
    // liberar memoria
    free(V1);
    free(V2);
    free(Vtemp);

    return 0;
}
