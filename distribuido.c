// bibliotecas
#include "utils/simple_init.h"                  // extraerParamsNTK(), inicializarVectors(), dwalltime()
#include "utils/ordenar_distribuido.h"


#define MASTER_ID 0

//Prototipo
void extraerParamsMPI(int argc, char* argv[],int *N, int*K);
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

    extraerParamsMPI(argc, argv, &N, &K);

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

void extraerParamsMPI(int argc, char* argv[],int *N, int*K){
    if (argc < 2) {
        printf("Debe especificar la potencia NN (entre 1..31) \n");
        printf("puede especificar K diferencias entre vectores \n");
        MPI_Finalize();
        exit(1);
    }

    *N = atoi(argv[1]);

    if (*N <= 1) {
        printf("N debe ser positivo\n");
        MPI_Finalize();
        exit(2);
    }else
        if( *N>=32){
            printf("debe ingresar la potencia de N menor a 32");
            MPI_Finalize();
            exit(3);
        }

  	*N = pow(2,*N);
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