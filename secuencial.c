
// ORDENAR 2 VECTORES Y COMPARAR SI CONTIENEN LOS MISMOS ELEMENTOS
// compilar con gcc -pthread secuencial.c -o secuencial -lm

// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/time.h>
#include <stddef.h>
#include <math.h> 
#include <time.h>       // random seed

#define MIN(A, B) ((A) < (B) ? (A) : (B)) // macro numero minimo

// Constantes
// #define DEBUG
// #define DEBUG2

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar();
void iterativeSort(int* vec, int Length);
void mergeBlocks(int* vec, int indice, int blockSize, int* tempvec);
void comparar();
void check_ordenamiento();
void verVector(int* v,int lengt);

// Variables compartidas
int N; //Tamaño del vector
int K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int diferencia=0; // flag deteccion de diferencias

double dwalltime(){
    double sec;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}


// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
	  Vtemp = (int*) malloc(N * sizeof(int));
    
    inicializar();

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();


    iterativeSort(V1,N);
    iterativeSort(V2,N);
    comparar();
    
    printf("Para N=%d, tiempo de ejecucion %f segundos\n", N, dwalltime() - t0);

    check_ordenamiento();

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


void iterativeSort(int* vec, int Length) {

  int indice;
    // ordena los elementos de a pares
    for (indice = 0; indice < Length - 1; indice += 2) {
        if (vec[indice] > vec[indice + 1]) {
            // Swap elements if the pair is not sorted
            int temp = vec[indice];
            vec[indice] = vec[indice + 1];
            vec[indice + 1] = temp;
        }
    }

    // Merge increasingly larger blocks of elements
    for (int blockSize = 2; blockSize <= Length / 2; blockSize *= 2) {
        for (indice = 0; indice < Length - 1; indice += (blockSize * 2)) {
            mergeBlocks(vec, indice, blockSize, Vtemp);
        }
    }

}

void mergeBlocks(int* vec, int offset, int blockSize, int* tempvec) {
    // punteros
    int* firstBlockPtr = vec + offset;
    int* secondBlockPtr = vec + offset + blockSize;

    tempvec = tempvec + offset; // muevo los punteros locales para trabajar en sona del vector correcta
    vec = vec + offset;

    // Indices para recorrer vectores
    int firstBlockIndex = 0;
    int secondBlockIndex = 0;
    int tempIndex = 0;

    // mientras ningun indice llegue al final / compara y guarda el menor
    while (firstBlockIndex < blockSize && secondBlockIndex < blockSize) {
        if (firstBlockPtr[firstBlockIndex] <= secondBlockPtr[secondBlockIndex]) {
            tempvec[tempIndex++] = firstBlockPtr[firstBlockIndex++];
        } else {
            tempvec[tempIndex++] = secondBlockPtr[secondBlockIndex++];
        }
    }
    while (firstBlockIndex < blockSize) {    // copia todos los elementos restantes del primer vector
      tempvec[tempIndex++] = firstBlockPtr[firstBlockIndex++];
    }
    while (secondBlockIndex < blockSize) {    // copia todos los elementos restantes del segundo vector
      tempvec[tempIndex++] = secondBlockPtr[secondBlockIndex++];
    }

    // copia los elementos ordenados al vector original
    for (int i = 0; i < 2*blockSize; i++) {
        vec[i] = tempvec[i];
    }

}

void comparar(){
    //Comparar los 2 vectores
  	for (int i=0; i<N; i++){
  		if(V1[i] != V2[i]){
        #ifdef DEBUG
          printf("Diferencia Encontrada: V1[%d] = %d es distinto de V2[%d] = %d \n", i, V1[i], i, V2[i]);
        #endif
        diferencia=1;
        break;
      }
    }
}

void inicializar(){
    int i;

    srand(time(NULL));
    for (i=0; i<N; i++) {
    	V1[i] = rand() % 10000;
    	V2[i] = V1[i];
    }
    for(int j=0;j<K;j++){// inserta errores aleatorios en V2
      	V2[rand()%N] = rand() % 10000;
    }
}

void check_ordenamiento(){
    // verifica si V1 y V2 esta ordenado de menor a mayor
    for (int i=0; i<N-1; i++){
        if (V1[i] > V1[i+1]){
           printf("Error: V1[%d] = %d es menor que V1[%d] = %d \n", i+1, V1[i+1], i, V1[i]);
           break;
        }
        if (V2[i] > V2[i+1]){
           printf("Error: V2[%d] = %d es menor que V2[%d] = %d \n", i+1, V2[i+1], i, V2[i]);
           break;
        }
    }
	
}

void extraerParams(int argc, char* argv[]){
    if (argc < 2) {
        printf("Debe especificar la potencia NN (entre 1..31) \n");
        printf("Debe especificar errores K \n");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N <= 1) {
        printf("N debe ser positivo\n");
        exit(2);
    }else
    	if( N>=32){
        printf("debe ingresar la potencia de N menor a 32");
        exit(3);
      }
  	N = pow(2,N);
    printf("\t - Tamaño del vector ingresado %d \n",N);
  	
  	if(argc==3){
      K = atoi(argv[2]);
      if(K<0){
        printf("K debe ser positivo\n");
        exit(4);
      }
      printf("\t - %d errores insertados en vector 2 \n \n",K);
    }
}

void verVector(int* v,int length){
  for(int u=0;u<length;u++){
    printf("v[%d]:%d, \t",u,v[u]);
  }
  printf("\n\n");
}
