#!/bin/bash

# compilar proyecto
gcc -pthread -o paralelo paralelo.c utils/simple_init.c utils/check.c utils/ordenar_secuencial.c utils/ordenar_paralelo.c -lm

# Nombre del archivo ejecutable
ejecutable="./paralelo"

# Parámetros para cada ejecución
parametros=(
    "25 16 0 swap"  # arreglo N:2^20 , hilos T:1 ; diferencias 0
)

# Nombre de la subcarpeta para los archivos .log
subcarpeta="resultados"

# Crear la subcarpeta si no existe
mkdir -p $subcarpeta

# Número total de ejecuciones
num_ejecuciones=${#parametros[@]}

# Loop para ejecutar el programa secuencial con diferentes parámetros
for ((i=0; i<num_ejecuciones; i++)); do
    # Obtener los parámetros y formar una cadena para incluirlos en el nombre del archivo
    parametros_str=$(echo "${parametros[i]}" | tr ' ' '_')
    
    # Nombre del archivo de salida
    archivo_salida="$subcarpeta/paralelo_${parametros_str}.log"

    # Ejecutar el programa con los parámetros correspondientes y guardar la salida en la subcarpeta
    echo "Ejecutando $ejecutable con parámetros ${parametros[i]} y guardando salida en $archivo_salida"
    $ejecutable ${parametros[i]} > $archivo_salida
done