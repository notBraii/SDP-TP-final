// PARALELIZAR LA ORDENACION POR MEZCLA DE 2 VECTORES DE N ELEMENTOS Y COMPARAR SI TIENEN LOS MISMOS ELEMENTOS

//En pthreads tenes que esperar
//En MPI se esperan entre ellos (?
//Utilizar GDB para debugger
/*Compilar con:

gcc -pthread -g -o memCompartida memCompartida.c -lm

*/
// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    //manejo de threads
#include <time.h>       // random seed
#include <math.h>       // log2
#include <sys/time.h>   // dwalltime
#include <stddef.h>     // NULL

// Macros
#define MIN(A, B) ((A) < (B) ? (A) : (B)) //Devuelve el mínimo

// Constantes
#define LONGITUD_SUBVECTOR(N,T) (N/T)
#define DEBUG

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar();

void create_and_join(void *(*start_routine)(void *), int T);
void* threadTask(void* arg);
void ordenarVector(int * vec, int * vtemp, int length, int id );
void ordenar (int * vec, int * vtemp, int length);
void combinar(int * vec, int offset, int blockSize, int * tempvec);

void comparar(int offset, int length);

//Verificaciones
void verificarOrden(int* vec, int offset,int length);
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
pthread_barrier_t barrera;

// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    pthread_t hilos[T];
    int ids[T];

    // Reserva de memoria
    V1 = (int*) malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
    Vtemp = (int*) malloc(N * sizeof(int));
    
	//Inicializa barrera   
    pthread_barrier_init(&barrera, NULL, T );
     
	//Inicializa vectores V1 y V2
    inicializar();

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    for (i = 0; i < T; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, threadTask, &ids[i]);
    }

    for (i=0; i<T; i++){
        pthread_join(hilos[i], NULL);
    }

//    create_and_join(&threadTask, T);

    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // Verificar ordenacion (menor a mayor)
    
    //Imprimir resultado
    if (diferencia)
        printf("\t - Hay diferencia entre los vectores \n");
    else
		printf("\t - Los vectores son iguales \n");
    

    // liberar recursos
    pthread_barrier_destroy(&barrera);
    
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
    const int offset = id * LONGITUD_SUBVECTOR(N, T);
    int length = LONGITUD_SUBVECTOR(N, T);

    #ifdef DEBUG
        printf("Hilo %d, size %d, offset %d \n", id, length, offset);
    #endif
    //Se ordena vector 1
    ordenarVector(V1, Vtemp, length, id);

    //Se ordena vector 2
    ordenarVector(V2, Vtemp, length, id);

	//Compara los vectores V1 y V2 ordenados
	comparar(offset, length);
	
	//Finaliza hilo
    pthread_exit(NULL);
}

//Ordena el vector pasado por parámetro
void ordenarVector(int * vec, int * vtemp, int length, int id ){
    
    int i, porciones;
    const int offset = id * LONGITUD_SUBVECTOR(N, T);

    // Etapa 1: ordenación de la porción asignada
    ordenar(vec + offset, vtemp + offset , length);
    #ifdef DEBUG
        verVector(vec + offset, length);
    #endif
    pthread_barrier_wait(&barrera);
    
    // Etapa 2: combinar de a pares
    for (i=1; i<=log2(T); i++){
        porciones = pow(2,i);
        
    	// Si mi id mod porciones es 0, trabajo
        if (id % porciones == 0){
        	combinar(vec, offset, length * porciones/2, vtemp);
            #ifdef DEBUG
                printf("Hilo %d, size %d, offset %d \n", id, length*porciones, offset);
                verVector(vec + offset, length*porciones);
            #endif  
        }
    
	    pthread_barrier_wait(&barrera);
    }
    


}

//Mergesort iterativo
void ordenar(int * vec, int * vtemp, int length){
    //vec = vec + 
    int indice;
    // ordena los elementos de a pares
    for (indice = 0; indice < length - 1; indice += 2) {
        if (vec[indice] > vec[indice + 1]) {
            // Intercambia elementos si el par no está ordenado
            int temp = vec[indice];
            vec[indice] = vec[indice + 1];
            vec[indice + 1] = temp;
        }
    }

    // Combina incrementando en cada iteración la longitud del subvector
    for (int blockSize = 2; blockSize <= length / 2; blockSize *= 2) {
        for (indice = 0; indice < length - 1; indice += (blockSize * 2)) {
            combinar(vec, indice, blockSize, vtemp);
        }
    }
}

//Combina los subvectores que posee cada hilo (vectores temporal vTemp)
void combinar(int * vec, int offset, int blockSize, int * tempvec){
    
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
    
    /*int len1 = (medio - left) + 1;
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
    */
}

//Compara los vectores V1 y V2 (secuencial)
/*
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

*/

void comparar(int offset, int length){
	int* vec1 = V1 + offset;
	int* vec2 = V2 + offset;
	for (int i=0; ((i<length) && (!diferencia)); i++){
  		if(vec1[i] != vec2[i]){
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
    	V1[i] = N - i; //rand() % 10000;
    	V2[i] = V1[i];
    }
    for(int j=0;j<K;j++){// inserta errores aleatorios en V2
      	V2[rand()%N] = rand() % 10000;
    }
}

//Ver vector
void verVector(int* v,int length){
  for(int u=0;u<length;u++){
    printf("v[%d]:%d, \t",u,v[u]);
  }
  printf("\n\n");
}