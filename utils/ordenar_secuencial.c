
#include "ordenar_secuencial.h"
//#define WO_FUNCTION

void iterativeSort(int* vec, int* tempvec, int length) {
  int indice;
    // ordena los elementos de a pares
    for (indice = 0; indice < length - 1; indice += 2) {
        if (vec[indice] > vec[indice + 1]) {
            // permuta los numeros que no estan ordenados de a pares
            int temp = vec[indice];
            vec[indice] = vec[indice + 1];
            vec[indice + 1] = temp;
        }
    }

    // Merge increasingly larger blocks of elements
    for (int blockSize = 2; blockSize <= length / 2; blockSize *= 2) {
        for (indice = 0; indice < length - 1; indice += (blockSize * 2)) {
            mergeBlocks(vec, tempvec, indice, blockSize);
            #ifdef CHECK
              printf("check %d'%d - ",indice,2*blockSize);
              orderCheck(vec, indice, 2*blockSize); // chequeo que ese segmento este ordenado
            #endif
      }
      #ifdef CHECK
            printf("\n -\n ");
      #endif
    }

}

// recibe 2 bloques de numeros ordenados y los junta de forma ordenada
void mergeBlocks(int* vec, int* tempvec, int offset, int blockSize) {
    // punteros
    vec = vec + offset;
    int *firstBlockPtr = vec;
    int* secondBlockPtr = vec + blockSize;

    tempvec = tempvec + offset; // muevo los punteros locales para trabajar en sona del vector correcta

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

}

// funcion silimar al ordenar iterativo, pero opimiza en el copiado a memoria
// cada paso de ordenacion, permuta los vectores ordenando y ordenado, para evitar copiar todos los datos
void iterativeSortSwap(int** prtVec, int** ptrTempvec,int offset, int length) {
    int temp;
    int* vec= (*prtVec)+offset;
    int* vecOut=(*ptrTempvec)+offset;// guardo sub-referencia
    int* vect;

    // ordena los elementos de a pares
    for (int indice = 0; indice < length - 1; indice += 2) {
        if (vec[indice] > vec[indice + 1]) {
            // permuta los numeros que no estan ordenados de a pares
            temp = vec[indice];
            vec[indice] = vec[indice + 1];
            vec[indice + 1] = temp;
        }
    }
    //Mezcla incrementando la longitud de los bloques
    for (int blockSize = 2; blockSize <= length / 2; blockSize *= 2) {
        for (int indice = 0; indice < length - 1; indice += (blockSize * 2)) {
            mergeBlocksToOut(vec, vecOut, indice, blockSize);
#ifdef WO_FUNCTION
            int *firstBlockPtr = vec + indice;
            int* secondBlockPtr = vec + indice + blockSize;

            //vecOut = vecOut + offset; // muevo los punteros locales para trabajar en sona del vector correcta

            // Indices para recorrer vectores
            int firstBlockIndex = 0;
            int secondBlockIndex = 0;
            int tempIndex = 0;

            // mientras ningun indice llegue al final / compara y guarda el menor
            while (firstBlockIndex < blockSize && secondBlockIndex < blockSize) {
                if (firstBlockPtr[firstBlockIndex] <= secondBlockPtr[secondBlockIndex]) {
                    vecOut[indice + tempIndex++] = firstBlockPtr[firstBlockIndex++];
                } else {
                    vecOut[indice + tempIndex++] = secondBlockPtr[secondBlockIndex++];
                }
            }
            while (firstBlockIndex < blockSize) {
            vecOut[indice + tempIndex++] = firstBlockPtr[firstBlockIndex++];
            }
            while (secondBlockIndex < blockSize) {
            vecOut[indice + tempIndex++] = secondBlockPtr[secondBlockIndex++];
            }
#endif
        }
        
        //por cada nivel de ordenacion permuta los punteros
        vect=vec;
        vec=vecOut;
        vecOut=vect;
    }
    // al finalizar el algoritmo, guarda el estado de los punteros de trabajo en los punteros de entrada
    if(offset==0){
        *prtVec=vec;
        *ptrTempvec=vecOut;
    }
}

void mergeBlocksToOut(int* vec, int* vecOut, int offset, int blockSize) {
    // punteros
    vec = vec + offset;
    int *firstBlockPtr = vec;
    int* secondBlockPtr = vec + blockSize;

    vecOut = vecOut + offset; // muevo los punteros locales para trabajar en sona del vector correcta

    // Indices para recorrer vectores
    int firstBlockIndex = 0;
    int secondBlockIndex = 0;
    int tempIndex = 0;

    // mientras ningun indice llegue al final / compara y guarda el menor
    while (firstBlockIndex < blockSize && secondBlockIndex < blockSize) {
        if (firstBlockPtr[firstBlockIndex] <= secondBlockPtr[secondBlockIndex]) {
            vecOut[tempIndex++] = firstBlockPtr[firstBlockIndex++];
        } else {
            vecOut[tempIndex++] = secondBlockPtr[secondBlockIndex++];
        }
    }
    while (firstBlockIndex < blockSize) {
      vecOut[tempIndex++] = firstBlockPtr[firstBlockIndex++];
    }
    while (secondBlockIndex < blockSize) {
      vecOut[tempIndex++] = secondBlockPtr[secondBlockIndex++];
    }
}
