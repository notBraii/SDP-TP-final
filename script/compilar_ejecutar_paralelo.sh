#!/bin/bash

# compilar proyecto
gcc -pthread -o paralelo paralelo.c utils/simple_init.c utils/check.c utils/ordenar_secuencial.c utils/ordenar_paralelo.c -lm

# Nombre del archivo ejecutable
ejecutable="./paralelo"

# Parámetros para cada ejecución
parametros=(
    "24 1 0"
    "24 1 1"
    "24 2 0"
    "24 2 1"
    "24 4 0"
    "24 4 1"
    "24 16 0"
    "24 16 1"
    "25 1 0"
    "25 1 1"
    "25 2 0"
    "25 2 1"
    "25 4 0"
    "25 4 1"
    "25 16 0"
    "25 16 1"
    "26 1 0"
    "26 1 1"
    "26 2 0"
    "26 2 1"
    "26 4 0"
    "26 4 1"
    "26 16 0"
    "26 16 1"
    "27 1 0"
    "27 1 1"
    "27 2 0"
    "27 2 1"
    "27 4 0"
    "27 4 1"
    "27 16 0"
    "27 16 1"
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