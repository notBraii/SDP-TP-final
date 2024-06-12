
#include "check.h"

void verVector(int* vec,int length){
  for(int u=0;u<length;u++){
    printf("v[%d]:%d, \t",u,vec[u]);
  }
  printf("\n");
}

int orderCheck(int* vec, int offset,int length){
    int* vec1=vec+offset;
    for (int i=0; i < length-1; i++){
        if (vec1[i] > vec1[i+1]){
            printf("Error Ordenamiento: V[%d]:%d < V[%d]:%d \n", i+offset+1, vec1[i+1], i+offset, vec1[i]);
            return 0;
        }
    }
    return 1;
}

void compararVec(int* vec1, int* vec2, int offset, int length, int* flag){
  vec1+=offset;
  vec2+=offset;
  for (int i=0; (i<length)&&(!(*flag)); i++){
    if(vec1[i] != vec2[i]){
      *flag = 1;
      break;
    }
  }
}
