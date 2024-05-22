// PARALELIZAR LA ORDENACION POR MEZCLA DE 2 VECTORES DE N ELEMENTOS Y COMPARAR SI TIENEN LOS MISMOS ELEMENTOS

/*Compilar con:

gcc -pthread –o memCompartida memCompartida.c -lm

*/
// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    //manejo de threads
#include <time.h>       // random seed
#include <math.h>       // log2
#include <sys/time.h>   // dwalltime
//#include <stddef.h>     // NULL

// Macros
#define MIN(A, B) ((A) < (B) ? (A) : (B)) //Devuelve el mínimo

// Constantes
#define ELEMENTOS_POR_HILO(N,T) (N/T)
// #define DEBUG

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar();

void create_and_join(void *(*start_routine)(void *), int T);
void* threadTask(void* arg);
void ordenarVector(int * vec, int left, int right, int id );
void ordenar (int * vec, int left, int right);
void combinar(int * vec, int left, int medio, int right);

void comparar();

//Verificaciones
void verificarOrden();
void verVector(int* v, int length);

double dwalltime(); //

// Variables compartidas
int N, //Tamaño del vector
    T, //Cantidad de hilos
    K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar
int diferencia = 0; // flag deteccion de diferencias
pthread_barrier_t* barreras;

// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));
    barreras = (pthread_barrier_t*) malloc(T * sizeof(pthread_barrier_t));
    
    // Si trabajan T, luego T/2, T/4... 1, son 2^T - 1 barreras
    for (i=0; i<=log2(T); i++){
        pthread_barrier_init(&barreras[i], NULL, T / pow(2,i));
    } 

    inicializar();

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    create_and_join(&threadTask, T);

    comparar();
    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // Verificar ordenacion (menor a mayor)
    
    //Imprimir resultado
    if (diferencia)
        printf("\t - Hay diferencia entre los vectores \n");
    else
		printf("\t - Los vectores son iguales \n");
    

    // liberar recursos
    for (i=0; i<=log2(T); i++){
        pthread_barrier_destroy(&barreras[i]);
    }
    free(barreras);
    free(V1);
    free(V2);
    free(Vtemp);
    return 0;
}


//Crea los hilos y espera a que finalicen
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

/*Pasos a realizar:
    Primero se divide el vector en T partes
    Cada hilo ordena completamente su porción asignada (T hilos trabajando)
    Luego se combinan las porciones ordenadas de a pares (T/2 hilos trabajando)
    Se repite lo anterior hasta ordenar el vector completo (1 hilo trabajando)
*/

//Tarea que realiza cada hilo
void* threadTask(void* arg){

    const int id = *(int*) arg;
    const int left = id * ELEMENTOS_POR_HILO(N, T);
    int right = (id+1) * ELEMENTOS_POR_HILO(N, T) - 1;

    #ifdef DEBUG
        printf("Hilo %d, left %d, right %d \n", id, left, right);
    #endif
    //Se ordena vector 1
    ordenarVector(V1, left, right, id);

    //Se ordena vector 2
    ordenarVector(V2, left, right, id);

    pthread_exit(NULL);
}

//Ordena el vector pasado por parámetro
void ordenarVector(int * vec, int left, int right, int id ){
    
    int i, medio, porciones;
    
    // Etapa 1: ordenación de la porción asignada
    ordenar(vec, left, right);
    pthread_barrier_wait(&barreras[0]);

    // Etapa 2: combinar de a pares
    for (i=1; i<=log2(T); i++){
        porciones = pow(2,i);
        if (id % porciones != 0) break;

        // Me toca trabajar...
        right = (id+porciones) * ELEMENTOS_POR_HILO(N, T) - 1;
        medio = left + (right - left) / 2;
        
        combinar(vec, left, medio, right);

        pthread_barrier_wait(&barreras[i]); //por simplicidad no verifico i
    }

}

//Mergesort recursivo
void ordenar(int * vec, int left, int right){
    if (left >= right) return;

    // l = 2, r = 8 -> m = 2 + 3 = 5
    int len = right - left;
    int medio = left + len / 2;

    // ordenar parte izquierda (2 a 5)
    ordenar(vec, left, medio);

    // ordenar parte derecha (6 a 8)
    ordenar(vec, medio+1, right);

    // combinar vectores ordenados
    combinar(vec, left, medio, right);
}

//Combina los subvectores que posee cada hilo (vectores temporales dinámicos)
void combinar(int * vec, int left, int medio, int right){
    int len1 = (medio - left) + 1;
    int len2 = (right - medio);
    int i = 0, j = 0, k;

    // crear arreglos temporales
    double* L = (double*) malloc(len1 * sizeof(double));
    double* R = (double*) malloc(len2 * sizeof(double));

    // copiar datos
    for (k=0; k<len1; k++){
        L[k] = vec[left+k];
    }
    
    for (k=0; k<len2; k++){
        R[k] = vec[medio+k+1];
    }

    // realizar ordenacion por cada L[i] y R[j]
    for (k=left; k<=right; k++){
        if (i >= len1) vec[k] = R[j++];       // si se acabaron los de L, copiar los de R
        else if (j >= len2) vec[k] = L[i++];  // si se acabaron los de R, copiar los de L
        else if (L[i] < R[j]) vec[k] = L[i++];    // copiar el menor entre L y R
        else vec[k] = R[j++];

        #ifdef DEBUG
            printf("vec[%d] = %.2f \n", k, vec[k]);
        #endif
    }

    // liberar memoria temporal
    free(L);
    free(R);
}

//Compara los vectores V1 y V2 (secuencial)
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