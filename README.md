# Cache-Compartido-y-paralelizacion

%-----------------------------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

%-----------------------------------------------------------------------------------------

Autores: Felipe Moya Coto, Emanuel Hernández Castillo

RESUMEN DEL PROYECTO

En el presente trabajo se pretende simular el comportamiento de un caché compartido utilizando el protocolo MESI para mantener coherencia en los datos se trabajó con un caché local L1 de 8kB para cada uno de los cpu's así como con un caché compartido L2 de 64kB, se procesó un archivo que con direcciones de memoria y
datos, alternando entre el cpu0 y cpu1 y se verificó que los estados de cada cpu coincidan con los definidos con este protocolo para mantener coherencia. Además se paralelizó un programa utilizando la biblioteca openMPI con el fin de comparar el rendimiento de un proceso serial con un proceso serializado, demostrando que se obtiene una reducción considerable del tiempo de ejecución de un programa al estar este paralelizado. 
%-----------------------------------------------------------------------------------------

COMPILACIÓN Y EJECUCIÓN

El proyecto consta de los archivos cache.cpp, cache.h, criba.cpp, cribaMPI.cpp y Makefile. Para compilar y ejecutar el proyecto, en la misma carpeta que contiene los archivos cache.cpp, cache.h, criba.cpp, cribaMPI.cpp y Makefile (asumiendo que el archivo .trace también se encuentra en esta carpeta), se deben seguir los siguentes pasos, en una terminal de linux:

Para compilar el cache:
$ make -f cache.make
Para ejecutar el cache:
$./cache
Para compilar la criba secuencial sin paralelizar:
$ make -f criba.make
Para ejecutar la criba secuencial:
$ ./criba
Para compilar la criba paralelizada:
$ make -f cribaMPI.make
Para ejecutar la criba paralelizada:
$ mpirun -np 2 cribaMPI


%-----------------------------------------------------------------------------------------

RESULTADOS ESPERADOS

Una vez ejecutado el simulador de caché se obtuvo una transición de estados satisfactoria, ya que las líneas de caché cambiaban su estado de acuerdo con lo establecido con el protocolo MESI. Por otro lado, la paralelización de la criba de Erastótenes resultó en una disminución de aproximadamente el 30% del tiempo de ejecución secuencial de la criba, lo cual cumple con lo requerido.

%-----------------------------------------------------------------------------------------

FIN
