
#include "check.h"


// recibe la direccion del vector, su longitud
// imprime por consola el cada posicion con su valor
void verVector(int* vec,int length){
  for(int u=0;u<length;u++){
    printf("v[%d]:%d, \t",u,vec[u]);
  }
  printf("\n");
}

// recibe el vector, el desplasamiento de la posicion inicial donde debe comparar, y la longitud
// cuando encutra 2 valores que no estan ordenados de menor a mayor, imprime Error en consola
void orderCheck(int* vec, int offset,int length){ // parametros tipicos (id*N/T , N/T)
    int* vec1=vec+offset;
    for (int i=0; i < length-1; i++){
        if (vec1[i] > vec1[i+1]){
            printf("Error Ordenamiento: V[%d]:%d < V[%d]:%d \n", i+offset+1, vec1[i+1], i+offset, vec1[i]);
            break;
        }
    }
}

// compara que ambos vectores sean iguales ordenadamente
// si encuentra diferencia prende el flag de diferencia
void compararVec(int* vec1, int* vec2, int offset, int NLength, int* flag_diference){
  vec1+=offset;
  vec2+=offset;
  for (int i=0; (i<NLength)&&(!flag_diference); i++){
    if(vec1[i] != vec2[i]){
      *flag_diference = 1;
      break;
    }
  }
}
