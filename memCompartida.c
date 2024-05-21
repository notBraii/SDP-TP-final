// PARALELIZAR LA ORDENACION POR MEZCLA DE 2 VECTORES DE N ELEMENTOS Y COMPARAR SI TIENEN LOS MISMOS ELEMENTOS
//Compilar con:
//gcc -pthread –o memCompartida memCompartida.c

// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    //threads manage
#include <time.h>       // random seed
#include <math.h>       // log2
#include <sys/time.h>   // dwalltime
//#include <stddef.h>     // NULL

// Constantes
// #define DEBUG

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void combinar(int left, int medio, int right);
void* ordenarIterativo(void* arg);
void create_and_join(void *(*start_routine)(void *), int T);
double dwalltime();

// Variables compartidas
pthread_barrier_t* barreras;
int N; //Tamaño del vector
int T; //Cantidad de hilos
int K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int diferencia=0; // flag deteccion de diferencias

// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    // N...
    srand(time(NULL));
    V1 = (int*) malloc(N * sizeof(double));
    V2 = (int*) malloc(N * sizeof(double));
    Vtemp = (int*) malloc(N * sizeof(int));

// Si trabajan T, luego T/2, T/4... 1, son 2^T - 1 barreras    barreras = (pthread_barrier_t* ) malloc((log2(T)+1) * sizeof(pthread_barrier_t));
    for (i=0; i<=log2(T); i++) pthread_barrier_init(&barreras[i], NULL, T / pow(2,i));

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    create_and_join(&ordenarIterativo, T);

    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // Verificar ordenacion (menor a mayor)
    double ant = V[0];
    double act;

    for (i=1; i<N; i++){
        act = V[i];
        if (act < ant) printf("Error: V[%d] = %.2f es menor que V[%d] = %.2f \n", i, act, i-1, ant);
        ant = act;
    }

    // liberar recursos
    for (i=0; i<=log2(T); i++) pthread_barrier_destroy(&barreras[i]);
    free(barreras);
    free(V);

    return 0;
}


//Crear e insertar hilos a
void create_and_join(void *(*start_routine)(void *), int T){
    pthread_t hilos[T];
    int ids[T];
    int i;

    for (i=0; i<T; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, start_routine, &ids[i]);
    }

    for (i=0; i<T; i++){
        pthread_join(hilos[i], NULL);
    }
}

// Implemento funciones
static inline int min(int n1, int n2){
    return (n1 < n2) ? n1 : n2;
}

 // VERSION 1: N = 2*T, y además N es potencia de 2.
void* ordenarIterativo(void* arg){
    const int id = *(int*) arg;

    // N multiplo de T
    const int lenHilo = N/T;
    const int inicio = id * lenHilo;
    int fin = (id+1) * lenHilo - 1;

    // Etapa 1: ordenación de la porción asignada
    combinar(inicio, inicio, fin);
    pthread_barrier_wait(&barreras[0]);

    // Etapa 2: merge con hilo vecino
    int i, medio, porciones;

    for (i=1; i<=log2(T); i++){
        porciones = pow(2,i);
        if (id % porciones != 0) break;

        // Me toca trabajar...
        medio = fin;
        fin = (id+porciones) * lenHilo - 1;

        combinar(inicio, medio, fin);
        pthread_barrier_wait(&barreras[i]);
    }

    pthread_exit(NULL);
}

void combinar(int left, int medio, int right){
    int len1 = (medio - left) + 1;
    int len2 = (right - medio);
    int i = 0, j = 0, k;

    // crear arreglos temporales
    double* L = (double*) malloc(len1 * sizeof(double));
    double* R = (double*) malloc(len2 * sizeof(double));

    // copiar datos
    for (k=0; k<len1; k++) L[k] = V[left+k];
    for (k=0; k<len2; k++) R[k] = V[medio+k+1];

    // realizar ordenacion por cada L[i] y R[j]
    for (k=left; k<=right; k++){
        if (i >= len1) V[k] = R[j++];       // si se acabaron los de L, copiar los de R
        else if (j >= len2) V[k] = L[i++];  // si se acabaron los de R, copiar los de L
        else if (L[i] < R[j]) V[k] = L[i++];    // copiar el menor entre L y R
        else V[k] = R[j++];

        #ifdef DEBUG
            printf("V[%d] = %.2f \n", k, V[k]);
        #endif
    }

    // liberar memoria temporal
    free(L);
    free(R);
}

void extraerParams(int argc, char* argv[]){
    if (argc < 3) {
        printf("Especificar N y T\n");
        exit(1);
    }

    N = atoi(argv[1]);
    T = atoi(argv[2]);

    if (N < 1 || T < 1) {
        printf("N debe ser positivo\n");
        exit(2);
    }

    if (N % T != 0){
        printf("Por favor, utilice N multiplo de T\n");
        exit(3);
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