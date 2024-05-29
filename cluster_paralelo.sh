#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
mkdir -p resultados


gcc -pthread -o paralelo paralelo.c utils/simple_init.c utils/check.c utils/ordenar_secuencial.c utils/ordenar_paralelo.c -lm

if [ -z "$1" ]; then
    echo "Error: ingrese parametro cantidad de Hilos."
    exit 1
fi

if ! [[ $1 =~ ^[0-9]+$ ]]; then
    echo "Error: cantidad de hilos debe ser numerico."
    exit 1
fi

if [ "$1" -le 1 ]; then
    echo "Error: debe tener al menos 1 Hilo"
    exit 1
fi

#SBATCH -o resultados/paralelo_24.txt
#SBATCH -e resultados/error_paralelo_24.txt
./paralelo 24 $1 0
#SBATCH -o resultados/paralelo_25.txt
#SBATCH -e resultados/error_paralelo_25.txt
./paralelo 25 $1 0
#SBATCH -o resultados/paralelo_26.txt
#SBATCH -e resultados/error_paralelo_26.txt
./paralelo 26 $1 0
#SBATCH -o resultados/paralelo_27.txt
#SBATCH -e resultados/error_paralelo_27.txt
./paralelo 27 $1 0
