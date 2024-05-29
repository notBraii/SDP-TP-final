#!/bin/bash

# compilar proyecto
mpicc -o distribuido distribuido.c utils/simple_init.c utils/ordenar_secuencial.c utils/check.c utils/ordenar_distribuido.c -lm

# Nombre del archivo ejecutable
ejecutable="distribuido"

# Parámetros para cada ejecución
parametros=(
    "23 0"
    "24 0"
    "25 0"
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
    archivo_salida="$subcarpeta/distribuido_${parametros_str}.log"

    # Ejecutar el programa con los parámetros correspondientes y guardar la salida en la subcarpeta
    echo "Ejecutando $ejecutable con parámetros ${parametros[i]} y guardando salida en $archivo_salida"
    mpirun -np 2 $ejecutable ${parametros[i]} > $archivo_salida
done