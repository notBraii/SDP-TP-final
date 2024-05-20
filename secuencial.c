// PARALELIZAR LA ORDENACION POR MEZCLA DE UN VECTOR DE N ELEMENTOS

// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/time.h>
#include <stddef.h>
#include <math.h>
#include <time.h>       // random seed


// Constantes
// #define DEBUG
// #define DEBUG2
 #define DISTINTO

// Prototipos de funcion
void verVector(int* v,int lengt);
void extraerParams(int argc, char* argv[]);
void inicializar();
void ordenarIterativo(int*);
void ordenarPar(int, int);
//void combinar(int left, int medio, int right);
void subordenar(int* vec,int offset , int L);

// Variables compartidas
int N; //Tamaño del vector
int K; //Cantidad de errores
int *V1; //Arreglo 1 con valores
int *V2; //Arreglo 2 con valores
int *Vtemp; //Arreglo temporal para ordenar

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

    V1 =  malloc(N * sizeof(int));
    V2 = (int*) malloc(N * sizeof(int));
	  Vtemp = (int*) malloc(N * sizeof(int));
    
    inicializar();

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    ordenarIterativo(V1);
    ordenarIterativo(V2);
    //comparar();
    
    printf("Para N=%d, mide %f segundos\n", N, dwalltime() - t0);

    // verifica si V1 esta ordenado de menor a mayor
    for (i=0; i<N-1; i++){
        if (V1[i] > V1[i+1]){
           printf("Error: V1[%d] = %d es menor que V1[%d] = %d \n", i+1, V1[i+1], i, V1[i]);
           break;
        }
        if (V2[i] > V2[i+1]){
           printf("Error: V2[%d] = %d es menor que V2[%d] = %d \n", i+1, V2[i+1], i, V2[i]);
           break;
        }
    }
	
  	//Comparar los 2 vectores
  	for (i=0; i<N; i++){
  		if(V1[i] != V2[i]){
        	printf("Diferencia Encontrada: V1[%d] = %d es distinto de V2[%d] = %d \n", i, V1[i], i, V2[i]);
      		break;
      		
        }
    }
  	if (i == N)
		printf("El vector V1 contiene los mismos elementos de V2 \n");
  	else
      	printf("El vector V1 no contiene los mismos elementos de V2 \n");

  
  
    // liberar memoria
    free(V1);
    free(V2);
    free(Vtemp);

    return 0;
}

// Implemento funciones
static inline int min(int n1, int n2){
    return (n1 < n2) ? n1 : n2;
}

void ordenarIterativo(int* vec){
    int lengBloque, L, M, R;
		int aux;
    #ifdef DEBUG2
            printf("- vector entrada sin ordenar \n\t");
            verVector(vec,N);
    #endif
    // Ordenar pares
    for (L=0; L < N; L+=2){ // recorre todo el vector de a pares
        if(vec[L]>vec[L+1]){ //si la dupla no esta ordenada
            aux = vec[L];
            vec[L] = vec[L+1]; //permuta
        		vec[L+1] = aux;
        }
    }
    #ifdef DEBUG2
            printf("\n- vector entrada ordenado pares \n\t");
            verVector(vec,N);
    #endif

    // odena de a potencia de 2 a partir de bloques de 2, 4, 8, 16...
    for (lengBloque=2; lengBloque <= N/2; lengBloque *= 2){
        for (L=0; L < N-1; L += (lengBloque*2) ){
          subordenar(vec,L,lengBloque);
		}
    }
    #ifdef DEBUG2
            printf("\n- vector totalmente ordenado \n\t");
            verVector(vec,N);
    #endif

}

 // para dos segmentos ordenados de vector,de longitud L se mesclan
 // parametro vector utilizado, desplazamiento inicial, tamaño del bloque
 // se asumen ambos bloques identicos
 // se utiliza como memoria temporal Vtemp ( el segmento de memoria con mismos desplazamientos que Vector original)
void subordenar(int* vec,int offset , int L){
  int* vec1= vec+(offset);
  int* vec2= vec+(offset+L);
  int* Vpriv=Vtemp+(offset);
  int indice1=0;
  int indice2=0;
  for(int u = 0; u < 2*L; u++){// recorro todos los datos, guardo ordenado en VecTemporal
    if((indice1 < L) && (indice2 < L)){// si hay datos para comparar
        if(vec1[indice1] < vec2[indice2])
          Vpriv[u]= vec1[indice1++];//guarda el n° mas pequeño
        else
          Vpriv[u]= vec2[indice2++];//guarda el n° mas pequeño
    }else{
        if(indice1 < L)// si solo quedan datos del Vector1
          Vpriv[u] = vec1[indice1++];//los guardo como vienen, porque el vector esta ordenado
        else// si solo quedan datos del Vector2
          Vpriv[u] = vec2[indice2++];////los guardo como vienen, porque el vector esta ordenado
    }
  }
  
  for(int u = 0; u < 2*L; u++){// guardo el troso ordenado en el vector 1 (porque vec1 y vec2 pertenecen a memoria contigua)
    vec1[u] = Vpriv[u];
  }
  #ifdef DEBUG
    verVector(Vpriv,2*L);
  #endif
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

void extraerParams(int argc, char* argv[]){
    if (argc < 2) {
        printf("Debe especificar la potencia N (entre 1..31) \n");
        printf("Debe especificar errores K \n");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N <= 0) {
        printf("N debe ser positivo\n");
        exit(2);
    }
  	if( N>=32){
      	printf("debe ingresar la potencia de N<32\n");
		exit(3);
    }
  	N = pow(2,N);
    printf("\t - Tamaño del vector ingresado %d \n",N);
  	
  	if(argc==3){
      K = atoi(argv[2]);
      printf("\t - %d errores insertados en vector 2\n",K);
    }
}

void verVector(int* v,int length){
  for(int u=0;u<length;u++){
    printf("v[%d]:%d, \t",u,v[u]);
  }
  printf("\n\n");
}
