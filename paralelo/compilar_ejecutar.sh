#!/bin/bash

# compilar proyecto
gcc -pthread -o paralelo paralelo.c simple_init.c check.c ordenar_secuencial.c ordenar_paralelo.c -lm

# Nombre del archivo ejecutable
ejecutable="./paralelo"

# Parámetros para cada ejecución
parametros=(
    "20 1 0"  # arreglo N:2^20 , hilos T:1 ; diferencias 0
    "20 2 0"  # arreglo N:2^20 , hilos T:2 ; diferencias 0
    "20 4 2"  # arreglo N:2^20 , hilos T:4 ; diferencias 2
    "20 8 0"  # arreglo N:2^20 , hilos T:8 ; diferencias 0
)

# Nombre de la subcarpeta para los archivos .dat
subcarpeta="resultados"

# Crear la subcarpeta si no existe
mkdir -p $subcarpeta

# Número total de ejecuciones
num_ejecuciones=${#parametros[@]}

# Loop para ejecutar el programa secuencial con diferentes parámetros
for ((i=0; i<num_ejecuciones; i++)); do
    # Nombre del archivo de salida
    archivo_salida="$subcarpeta/dat_$((i+1)).dat"

    # Ejecutar el programa con los parámetros correspondientes y guardar la salida en la subcarpeta
    echo "Ejecutando $ejecutable con parámetros ${parametros[i]} y guardando salida en $archivo_salida"
    $ejecutable ${parametros[i]} > $archivo_salida
done