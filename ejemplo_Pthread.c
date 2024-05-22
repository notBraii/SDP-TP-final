// Ejemplo, se ejecutan T tareas, y cada tarea ordena un pedazo del cector V1 y V2 


// bibliotecas
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    //manejo de threads
#include <sys/time.h>   // dwalltime
#include <stddef.h>     // NULL
#include <math.h>       // log2
#include <time.h>       // random seed


// Macros
#define MIN(A, B) ((A) < (B) ? (A) : (B)) //Devuelve el mínimo

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar();
void iterativeSort(int* vec, int Length);
void mergeBlocks(int* vec, int offset, int blockSize, int* tempvec);
void comparar();
void verVector(int* v, int length); //funcion auxiliar para DEBUG
double dwalltime(); //tiempo de ejecucion
void *taskThread(void *arg);


// Variables compartidas
pthread_barrier_t *barreras;
int N; //Tamaño del vector
int T; //Cantidad de hilos
int K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int diferencia = 0; // flag deteccion de diferencias


int main(int argc, char* argv[]){
    
    extraerParams(argc, argv);

    int i;
    pthread_t hilos[T];
    int ids[T];

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));
    barreras = (pthread_barrier_t*) malloc(T * sizeof(pthread_barrier_t));

    inicializar();

    double t0 = dwalltime();

    // Crear los hilos
    for (i = 0; i < T; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, taskThread, &ids[i]); //manda a ejecutar tarea

    }

    // Esperar a que todos los hilos terminen
    for (i = 0; i < T; i++) {
        pthread_join(hilos[i], NULL);
    }


    printf("Para N:%d, T:%d , tardo %f segundos\n", N,T, dwalltime()- t0);
    

    free(V1);
    free(V2);
    free(Vtemp);
    free(barreras);
    return 0;
}


void *taskThread(void *arg) {
    int id = *((int*)arg);


    iterativeSort(V1 + (id*N/T), N/T);

    printf("id:%d vector segmento %d, con cantidad de elementos %d \n ",id ,(id*N/T) , (N/T));

    iterativeSort(V2 + (id*N/T), N/T);
        

    pthread_exit(NULL);
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
    // Comparar los 2 vectores
    for (int i = 0; i < N; i++){
        if (V1[i] != V2[i]){
        #ifdef DEBUG
            printf("Diferencia Encontrada: V1[%d] = %d es distinto de V2[%d] = %d \n", i, V1[i], i, V2[i]);
        #endif
        diferencia = 1;
        break;
        }
    }
    
}

// Carga las variables globales con valores pasados por parámetros
void extraerParams(int argc, char* argv[]){
    K = 0;
    if (argc < 3) {
        printf("Especificar al menos N y T\n");
        exit(1);
    }
    
    N = atoi(argv[1]);
    T = atoi(argv[2]);

    if (N <= 1) {
        printf("N debe ser positivo\n");
        exit(2);
    }else
        if( N>=32){
            printf("Debe ingresar la potencia de N menor a 32\n");
            exit(3);
        }
    
    
    N = pow(2,N);

    if (T < 1) {
        printf("T debe ser positivo\n");
        exit(2);
    }
    if (N % T != 0){
        printf(" N debe ser multiplo de T\n");
        exit(4);
    }

    printf("\t - Tamaño del vector ingresado %d \n",N);
    printf("\t - Cantidad de hilos: %d \n",T);
    
    if(argc == 4){
        K = atoi(argv[3]);
        if(K < 0){
            printf("K debe ser positivo\n");
            exit(2);
        }

        printf("\t - %d errores insertados en vector 2 \n \n",K);
    }
    
}

void check_ordenamiento(){
    // verifica si V1 y V2 esta ordenado de menor a mayor
    for (int i = 0; i < (N/T)-1; i++){
        if (V1[i] > V1[i + 1]){
            printf("Error: V1[%d] = %d es menor que V1[%d] = %d \n", i + 1, V1[i + 1], i, V1[i]);
            break;
        }
        if (V2[i] > V2[i + 1]){
            printf("Error: V2[%d] = %d es menor que V2[%d] = %d \n", i + 1, V2[i + 1], i, V2[i]);
            break;
        }
    }

    if (diferencia)
        printf("\t - Hay diferencia entre los vectores \n");
    else
        printf("\t - Los vectores son iguales \n");
}

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

void verVector(int* v,int length){
  for(int u=0;u<length;u++){
    printf("v[%d]:%d, \t",u,v[u]);
  }
  printf("\n\n");
}
//Carga con valores aleatorios los vectores V1 y V2
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

