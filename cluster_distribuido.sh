#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive

mkdir -p resultados
mpicc -o distribuido distribuido.c utils/simple_init.c utils/ordenar_secuencial.c utils/check.c utils/ordenar_distribuido.c -lm

if [ -z "$1" ]; then
    echo "Error: ingrese parametro cantidad de Hilos."
    exit 1
fi

if ! [[ $1 =~ ^[0-9]+$ ]]; then
    echo "Error: cantidad de hilos debe ser numerico."
    exit 1
fi

if [ "$1" -le 1 ]; then
    echo "Error: debe tener almenos 1 Hilo"
    exit 1
fi
#SBATCH --tasks-per-node=$1

#SBATCH -o resultados/distrib_24.txt
#SBATCH -e resultados/errores_distrib_24.txt
mpirun distribuido 24 0
#SBATCH -o resultados/distrib_25.txt
#SBATCH -e resultados/errores_distrib_25.txt
mpirun distribuido 25 0
#SBATCH -o resultados/distrib_26.txt
#SBATCH -e resultados/errores_distrib_26.txt
mpirun distribuido 26 0
#SBATCH -o resultados/distrib_27.txt
#SBATCH -e resultados/errores_distrib_27.txt
mpirun distribuido 27 0
