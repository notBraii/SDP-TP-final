// Ejemplo, se ejecutan T tareas, y cada tarea ordena un pedazo del cector V1 y V2
// compilar con 
//      gcc -pthread ejemplo_Pthread.c -o a -lm

// bibliotecas
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    //manejo de threads
#include <sys/time.h>   // dwalltime
#include <stddef.h>     // NULL
#include <math.h>       // log2
#include <time.h>       // random seed

//#define DEBUG
#define CHECK


// Macros
#define MIN(A, B) ((A) < (B) ? (A) : (B)) //Devuelve el mínimo

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar();
void iterativeSort(int* vec, int Length, int* vecTemp);
void mergeParalelo(int id, int* vec, int* vecTemp); // funcion principal de ordenar paralelo
void mergeBlocks(int* vec, int offset, int blockSize, int* tempvec);
void comparar(int *Vec1, int *Vec2, int offset, int Length); // compara sub-vectores de parametro
void verVector(int* v, int length); //funcion auxiliar para DEBUG
void check_ordenamiento(int* vec, int offset,int length); // funcion auxiliar para DEBUG
double dwalltime(); // tiempo de ejecucion
void *taskThread(void *arg); // programa principal de cada Hilo


// Variables compartidas
pthread_barrier_t barrera;
int N; //Tamaño del vector
int T; //Cantidad de hilos
int K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int flag_diferencia = 0; // flag deteccion de diferencias

int main(int argc, char* argv[]){
    
    extraerParams(argc, argv);

    int i;
    pthread_t hilos[T];
    int ids[T];

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));

    pthread_barrier_init(&barrera, NULL, T);//inicializa la barrera global

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

    if (flag_diferencia)
        printf("\t - Hay diferencia entre los vectores \n");
    else
        printf("\t - Los vectores son iguales \n");

    #ifdef CHECK
        printf("\n check V1: "); check_ordenamiento(V1, 0, N);
        printf("\n check V2: "); check_ordenamiento(V2, 0, N);
    #endif

    pthread_barrier_destroy(&barrera);
    free(V1);
    free(V2);
    free(Vtemp);
    return 0;
}


void *taskThread(void *arg) {
    int indice;
    int id = *((int*)arg);

    mergeParalelo(id,V1,Vtemp);
    mergeParalelo(id,V2,Vtemp);

    comparar(V1,V2, id*N/T, N/T); // todos los hilos comparan un pedazo de los vectores, y guardan resultado en flag_diferencia=1

    pthread_exit(NULL);
}

void mergeParalelo(int id, int* vec, int* vecTemp){

    int offset=(id*N/T);

// Etapa 1 - cada Hilo ordena 1 segmento del vector
    iterativeSort(vec+offset, N/T , vecTemp+offset); // todos los hilos ordenan un segmento del vector
#ifdef CHECK
    printf("check %d/%d: ", id, T);
    check_ordenamiento(vec, offset, N/T); // verifica que su porcion de vector este ordenado
#endif

#ifdef DEBUG
    pthread_barrier_wait(&barrera);
    if(id==0) verVector(vec,N); // vector ordenado de a segmentos
#endif

// Etapa 2 - enbudo de ordenamiento
    pthread_barrier_wait(&barrera);
    for(int TActivos=T/2;TActivos>=1;TActivos/=2){ // indice es cantidad de hilos con trabajo activo
        if(id < TActivos){ //los nucleos activos (id<4) - ejecuta: T0,T1,T2,T3
       //     printf(" id:%d/%d -  offset:%d,blockZ:%d", id, TActivos,N*id/TActivos, N/(2*TActivos));
            offset=(id*N/TActivos);
            mergeBlocks(vec, offset, N/(TActivos*2) , vecTemp);
#ifdef CHECK
            printf("check %d/%d: ", id, TActivos);
            check_ordenamiento(vec, offset, N / TActivos); // verifica que su porcion de vector este ordenado
#endif
        }
#ifdef DEBUG
        pthread_barrier_wait(&barrera);
        if (id == -1)    verVector(vec, N); // vector ordenado de a segmentos dobles
        printf(" \n --- \n");
#endif
        pthread_barrier_wait(&barrera);
    }

}

void iterativeSort(int* vec, int Length, int* vecTemp) {

  int indice;
    // ordena los elementos de a pares
    for (indice = 0; indice < Length - 1; indice += 2) {
        if (vec[indice] > vec[indice + 1]) {
            // Swap elements if the pair is not sorted
            vecTemp[0] = vec[indice];
            vec[indice] = vec[indice + 1];
            vec[indice + 1] = vecTemp[0];
        }
    }

    // Merge increasingly larger blocks of elements
    for (int blockSize = 2; blockSize <= Length / 2; blockSize *= 2) {
        for (indice = 0; indice < Length - 1; indice += (blockSize * 2)) {
            mergeBlocks(vec, indice, blockSize, vecTemp);
            #ifdef CHECK
              printf("check %d'%d - ",indice,2*blockSize);
              check_ordenamiento(vec, indice, 2*blockSize); // chequeo que ese segmento este ordenado
            #endif
      }
      #ifdef CHECK
            printf("\n -\n ");
      #endif
    }

}

void mergeBlocks(int* vec, int offset, int blockSize, int* tempvec) {
    // punteros
    vec = vec + offset;
    int *firstBlockPtr = vec;
    int* secondBlockPtr = vec + blockSize;

    tempvec = tempvec + offset; // muevo los punteros locales para trabajar en sona del vector correcta


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

void check_ordenamiento(int* vec, int offset,int length){ // parametros tipicos (id*N/T , N/T)
    int* vec1=vec+offset;
    for (int i=0; i < length-1; i++){
        if (vec1[i] > vec1[i+1]){
            printf("Error Ordenamiento: V[%d]:%d < V[%d]:%d \n", i+offset+1, vec1[i+1], i+offset, vec1[i]);
            break;
        }
    }
}


void comparar(int* Vec1, int* Vec2, int offset, int Length){
    Vec1+=offset;
    Vec2+=offset;
    for (int i = 0; i < N; i++){
        if (Vec1[i] != Vec2[i]){
            #ifdef CHECK
                printf("\n Diferencia encontrada en v1[%d]:%d != v2[%d]: %d \n ", i,Vec1[i],i,Vec2[i]);
            #endif
            flag_diferencia = 1;
            break;
        }
        if(flag_diferencia!=0) break;
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

