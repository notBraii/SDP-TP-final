#!/bin/bash

# compilar proyecto
gcc -o crearVec crearVec.c -lm

# Nombre del archivo ejecutable
ejecutable="./crearVec"

$ejecutable 3 # ejecuto cada parametro
$ejecutable 4 # ejecuto cada parametro
$ejecutable 5 # ejecuto cada parametro
$ejecutable 6 # ejecuto cada parametro

