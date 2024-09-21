#!/bin/bash

# compilar proyecto
gcc -Iinclude -o secuencial secuencial.c lib/simple_init.c lib/check.c lib/ordenar_secuencial.c -lm

# Verificación de compilación
if [ $? -ne 0 ]; then
    echo "Error al compilar el proyecto"
    exit 1
fi

# Nombre del archivo ejecutable
ejecutable="./secuencial"

# Parámetros para cada ejecución
parametros=(
    "24 0"
    "24 1"
    "25 0"
    "25 1"
    "26 0"
    "26 1"
    "27 0"
    "27 1"
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
    archivo_salida="$subcarpeta/sec_${parametros_str}.log"

    # Ejecutar el programa con los parámetros correspondientes y guardar la salida en la subcarpeta
    echo "Ejecutando $ejecutable con parámetros ${parametros[i]} y guardando salida en $archivo_salida"
    $ejecutable ${parametros[i]} > $archivo_salida
done
