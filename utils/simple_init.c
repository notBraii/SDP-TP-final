
#include "simple_init.h"

//Valida la cantidad de parámetros y modifica variables globales de N, T, K
void extraerParamsNTK(int argc, char *argv[], int *Nleng, int *Threads, int *Kdiferences){
    *Kdiferences = 0;
    if (argc < 3) {
        printf("Especificar al menos N y T\n");
        exit(1);
    }

    *Nleng = atoi(argv[1]);
    *Threads = atoi(argv[2]);

    if (*Nleng <= 1) {
        printf("N debe ser positivo\n");exit(2);
    }else if( *Nleng>=32){
            printf("Debe ingresar la potencia de N menor a 32\n");exit(3);
        }
    
    
    *Nleng = pow(2,*Nleng);

    if (*Threads < 1) {
        printf("debe ejecutarse con almenos T=1 \n"); exit(2);
    }
    if (*Nleng % *Threads != 0){
        printf(" N debe ser multiplo de T\n"); exit(4);
    }

    printf("\t - Tamaño del vector ingresado %d \n", *Nleng);
    printf("\t - Cantidad de hilos: %d \n", *Threads);

    if(argc == 4){
        *Kdiferences = atoi(argv[3]);
        if(*Kdiferences < 0){
            printf("K debe ser positivo\n"); exit(2);
        }
        printf("\t - %d errores insertados en vector 2 \n\n", *Kdiferences);
    }
}

//Valida la cantidad de parámetros y modifica variables globales de N, K
void extraerParams(int argc, char* argv[],int *N, int*K){
    if (argc < 2) {
        printf("Debe especificar la potencia NN (entre 1..31) \n");
        printf("puede especificar K diferencias entre vectores \n");
        exit(1);
    }

    *N = atoi(argv[1]);

    if (*N <= 1) {       printf("N debe ser positivo\n");        exit(2);}
    else   if( *N>=32){  printf("debe ingresar la potencia de N menor a 32");       exit(3);}

  	*N = pow(2,*N);
    printf("\t - Tamaño del vector ingresado %d \n",*N);
  	
  	if(argc==3){
      *K = atoi(argv[2]);
      if(*K<0){         printf("K debe ser positivo\n");   exit(4);}
      printf("\t - %d errores insertados en vector 2 \n \n",*K);
    }
}

//Inicializa los vectores globales con valores aleatorios iguales en ambos vectores, agregando valores diferentes en posiciones aleatorias del vector 2 según el valor del parámetro K
void inicializarVectors(int* vec1, int* vec2, int length, int Kdiference){
    srand(2000);
    for (int i=0; i<length; i++) { // inicializa valores aleatorios
    	vec1[i] = rand() % 10000;
    	vec2[i] = vec1[i];
    }
    for(int j=0;j<Kdiference;j++){// inserta errores aleatorios en vector2
      	vec2[rand()%length] = rand() % 10000;
    }
    int temp;
    int indice1;
    int indice2;
    for(int i=0; i<(length/2); i++){ // mescla algunos valores del vector2
        indice1=rand()%length;
        indice2=rand()%length;
        temp=vec2[indice2];
        vec2[indice2] = vec2[indice1];
        vec2[indice1] = temp;
    }
}

double dwalltime(){
    double sec;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}
