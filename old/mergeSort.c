

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
