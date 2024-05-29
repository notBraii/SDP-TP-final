// al programa se le ingresa por parametro la longitud del vector a calcular
// crea este vector de valores aleatorios (entre 0..1000) separados por ","
// compilar
//      gcc -o crearVec crearVec.c -lm
// ejecutar
//      ./crearVec 27

#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <stddef.h>   // incluye NULL
#include <sys/time.h> // tiempo
#include <time.h>     // random seed
#include <math.h>     // pow

char nombre_archivo[100];

int main(int argc, char *argv[])
{
    if(argc<=1){
        printf("ingrese el parametro de la potencia de 2 a calcular \n");
        return 1;
    }
    int Nlength = atoi(argv[1]);
    int length = pow(2, Nlength);

    sprintf(nombre_archivo, "vec_2_%d.txt", Nlength); // carga en nombre del archivo
    printf("archivo de salida %s \n", nombre_archivo);

    FILE *archivo = fopen(nombre_archivo, "w"); // abrir y sobre-escribir el archivo

    // Verificar si se pudo abrir el archivo correctamente
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    srand(16409);
    //srand(time(NULL));
    printf(" - Calculando los 2^%d numeros : %d \n",Nlength,length);


    for (int i = 0; i < length; i++)
    { // inicializa valores aleatorios
        fprintf(archivo,"%4.0d, ", (int)(rand() % 1001)); // random entre 0..1000
    }
    printf("\n - el archivo creado correctamente \n");
}