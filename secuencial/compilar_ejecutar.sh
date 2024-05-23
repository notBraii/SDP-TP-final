#!/bin/bash

# compilar proyecto
gcc -o secuencial secuencial.c simple_init.c check.c ordenar_secuencial.c -lm 

# Nombre del archivo ejecutable
ejecutable="./secuencial"

# Parámetros para cada ejecución
parametros=(
    "20 0"
    "20 1"
    "20 2"
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