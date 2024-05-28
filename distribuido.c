// consigna : 
//          verificar si 2 vectores contienen los mismos números
// Estrategia de resolución: 
//          ordenar ambos vectores y comparar posición a posición si estan todos los mismos números
//          a cada proceso le toca ordenar un segmento del vector.
//          luego algunos procesos van mezclando los resultados ordenados para mayor paralelismo.
//          Para la comparación se sigue la misma idea: se reparten porciones de V1 y V2 a cada proceso y compara localmente
//          Para obtener el resultado al final de cada comparación del proceso se hace una reducción buscando el valor máximo de las diferencias encontradas.

// compilar con :
//          mpicc -o distribuido distribuido.c utils/simple_init.c utils/ordenar_secuencial.c utils/check.c utils/ordenar_distribuido.c -lm
// ejecutar (2^20 datos, 4 hilos, 0 errores insertados) con :
//          ./paralelo 20 4 0

// bibliotecas
#include "utils/simple_init.h"                  // extraerParamsNTK(), inicializarVectors(), dwalltime()
#include "utils/ordenar_distribuido.h"


#define MASTER_ID 0

//Prototipo
void extraerParamsMPI(int argc, char* argv[],int *N, int*K, int miID);
/*
int flag_diferencia = 0; // flag deteccion de diferencias
*/

int main(int argc, char* argv[]){
    
    int miID,
        cantProcesos,
        N,
        K;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &miID);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    extraerParamsMPI(argc, argv, &N, &K, miID);

    if(miID == MASTER_ID){
        //DENTRO DEL MASTER DEBEN IR LAS INICIALIZACIONES DE VECTORES, Y LA LÓGICA DE DISTRIBUCIÓN
        master(N, K, cantProcesos);
    }
    else{
        //SOLO RECIBE POR PARÁMETROS EN LOS MENSAJES SU PORCIÓN ASIGNADA DEL VECTOR, COMUNICACIÓN CONSTANTE CON EL MASTER
        slave(N, cantProcesos, miID);
    }

    MPI_Finalize();
    return (0);
}

void extraerParamsMPI(int argc, char* argv[],int *N, int*K, int miID){
    if (argc < 2) {
        if (miID == MASTER_ID){
            printf("Debe especificar la potencia NN (entre 1..31) \n");
            printf("puede especificar K diferencias entre vectores \n");
        }
        MPI_Finalize();
        exit(1);
    }

    *N = atoi(argv[1]);

    if (*N <= 1) {
        if (miID == MASTER_ID)
            printf("N debe ser positivo\n");
        MPI_Finalize();
        exit(2);
    }else
        if( *N>=32){
            if (miID == MASTER_ID) 
                printf("debe ingresar la potencia de N menor a 32");
            MPI_Finalize();
            exit(3);
        }

  	*N = pow(2,*N);
    if (miID == MASTER_ID){
            
        printf("\t - Tamaño del vector ingresado %d \n",*N);    
        if(argc==3){
            *K = atoi(argv[2]);
            if(*K<0){
                printf("K debe ser positivo\n");
                MPI_Finalize();
                exit(4);
            }

            printf("\t - %d errores insertados en vector 2 \n \n",*K);
        }
    }
}