#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
mkdir -p resultados
gcc -o secuencial secuencial.c utils/simple_init.c utils/check.c utils/ordenar_secuencial.c -lm

#SBATCH -o resultados/sec_24.txt
#SBATCH -e resultados/error_sec_24.txt
./secuencial 24 0
#SBATCH -o resultados/sec_25.txt
#SBATCH -e resultados/error_sec_25.txt
./secuencial 25 0
#SBATCH -o resultados/sec_26.txt
#SBATCH -e resultados/error_sec_26.txt
./secuencial 26 0
#SBATCH -o resultados/sec_27.txt
#SBATCH -e resultados/error_sec_27.txt
./secuencial 27 0
