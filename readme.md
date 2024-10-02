# Trabajo Práctico Final - Sistemas Distribuidos y Paralelos

## Descripción
El siguiente proyecto consiste en implementar un algoritmo que compare dos arreglos desordenados de números enteros y determine si contienen los mismos elementos, teniendo en cuenta tanto los valores como la cantidad de repeticiones.

Se implementaron tres versiones del algoritmo:
- Secuencial
- Paralela utilizando Pthreads
- Distribuida utilizando MPI

Las pruebas se realizaron con arreglos de tamaño $2^N$ con N = 24, 25, 26 y 27, constituidos por valores aleatorios.

## Prerrequisitos
- Compilador GCC y librería Pthreads
- MPI instalado para la ejecución distribuida

## Ejecución

### Scripts para compilación y ejecución automatizada

En la carpeta de [scripts](https://github.com/notBraii/SDP-TP-final/tree/main/script) se encuentran tres scripts que permiten la compilación y ejecución automatizada por lotes de las tres versiones del algoritmo. Cada script compila el código correspondiente y ejecuta varias pruebas consecutivas, guardando los resultados en archivos `.log` para su posterior análisis.

1. **Compilación y Ejecución Secuencial**:
   - [compilar_ejecutar_secuencial.sh](https://github.com/notBraii/SDP-TP-final/blob/main/script/compilar_ejecutar_secuencial.sh)
   - **Descripción**: Este script compila la versión secuencial del algoritmo y ejecuta las pruebas para diferentes tamaños de arreglos. Los tiempos de ejecución se guardan en el archivo `resultados/sec_24_0.log` donde los numeros corresponden a los parametros para un vector de longitud $2^{24}$, con 0 diferencias entre los vectores .
   - **Uso**:
     ```bash
     bash compilar_ejecutar_secuencial.sh
     ```

2. **Compilación y Ejecución Paralela**:
   - [compilar_ejecutar_paralelo.sh](https://github.com/notBraii/SDP-TP-final/blob/main/script/compilar_ejecutar_paralelo.sh)
   - **Descripción**: Este script compila la versión paralela del algoritmo utilizando Pthreads y ejecuta las pruebas con diferentes números de hilos (8, 16, 32, 64). Los tiempos de ejecución se guardan en `resultados/paralelo_24_4_0.log` donde los numeros corresponden a los parametros para un vector de longitud $2^{24}$, con 4 hilos de ejecucion, con 0 diferencias entre los vectores.
   - **Uso**:
     ```bash
     bash compilar_ejecutar_paralelo.sh
     ```
     El script ejecutará automáticamente la solución paralela para los diferentes números de hilos y tamaños de arreglos especificados en el script.

3. **Compilación y Ejecución Distribuida**:
   - [compilar_ejecutar_distribuido.sh](https://github.com/notBraii/SDP-TP-final/blob/main/script/compilar_ejecutar_distribuido.sh)
   - **Descripción**: Este script compila la versión distribuida del algoritmo utilizando MPI y ejecuta las pruebas distribuidas en el clúster, con configuraciones de 4, 8 y 16 procesos. Los tiempos de ejecución se almacenan en `resultados/distribuido_24_0.log` donde los numeros corresponden a los parametros para un vector de longitud $2^{24}$, con 0 diferencias entre los vectores.
   - **Uso**:
     ```bash
     ./compilar_ejecutar_distribuido.sh
     ```
     El script ejecutará automáticamente la solución distribuida en el clúster y guardará los resultados de las pruebas en el archivo de log.

### Resultados de Ejecución

#### Tiempos de ejecución
| Arquitectura de Ejecución | $2^{24}$ (seg) | $2^{25}$ (seg) | $2^{26}$ (seg) | $2^{27}$ (seg) |
|--------------------------|------------------|------------------|------------------|------------------|
| **Secuencial (Xeon PHI)** | 15.066           | 31.164           | 64.465           | 133.166          |
| **Secuencial (Blade)**    | 7.956            | 16.317           | 33.459           | 68.610           |

#### Paralelo (Xeon PHI) usando Pthreads:
| Hilos | $2^{24}$ (seg) | $2^{25}$ (seg) | $2^{26}$ (seg) | $2^{27}$ (seg) |
|-------|------------------|------------------|------------------|------------------|
| 8     | 2.626            | 5.390            | 10.826           | 22.328           |
| 16    | 1.939            | 3.918            | 7.951            | 15.376           |
| 32    | 1.594            | 3.090            | 6.911            | 13.704           |
| 64    | 1.422            | 2.787            | 5.944            | 12.713           |

#### Distribuido (Blade) usando MPI:
| Procesos | $2^{24}$ (seg) | $2^{25}$ (seg) | $2^{26}$ (seg) | $2^{27}$ (seg) |
|----------|------------------|------------------|------------------|------------------|
| 4        | 4.018            | 8.113            | 19.926           | 33.336           |
| 8        | 3.152            | 6.342            | 12.834           | 25.834           |
| 16       | 2.772            | 5.564            | 11.234           | 39.842           |

### Speedup y Eficiencia

- **Speedup en Xeon PHI (Pthreads)**:
  - Con 64 hilos: speedup máximo de 5.85x.

- **Speedup en Blade (MPI)**:
  - Con 16 procesos: speedup máximo de 2.97x.

- **Eficiencia**: Se observó una caída de la eficiencia en configuraciones con más hilos o procesos, lo que indica limitaciones en la escalabilidad del algoritmo debido al overhead de sincronización y comunicación.

### Escalabilidad
El algoritmo presenta problemas de escalabilidad, tanto en la versión paralela con Pthreads como en la distribuida con MPI, especialmente a medida que se aumenta el número de hilos/procesos. La escalabilidad no es fuerte ni débil, como era de esperar para un algoritmo de complejidad $O(n \log n)$.

## Autores
- Pacheco, Nicolas
- Llamocca, Brian
