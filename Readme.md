# Project Sanctuary

## Descripción
Project Sanctuary es un juego de simulación y gestión de supervivencia por turnos ambientado en un apocalipsis zombie. El jugador asume el rol de líder de un grupo de supervivientes, debiendo tomar decisiones estratégicas cada jornada sobre expediciones, traslados y distribución de suministros para mantenerlos con vida el mayor tiempo posible.

## Cómo compilar y ejecutar
Para compilar el proyecto correctamente, debes asegurarte de enlazar la librería matemática (`-lm`), la cual es estrictamente necesaria para el cálculo de probabilidades en el sistema de expediciones. Abre tu terminal y ejecuta el siguiente comando:

`gcc tdas/*.c *.c -Wno-unused-result -o program -lm`

Una vez compilado sin errores, ejecuta el programa con:

`./program`

## Estado del Proyecto (Funcionalidades y Errores Conocidos)
El programa cuenta con un flujo principal operativo (ciclo de 20 días) y gestiona la lógica de supervivencia correctamente. Sin embargo, analizando el código fuente, existen implementaciones exitosas y algunas estructuras que fueron omitidas.

* **Lo que está programado y funciona correctamente:**
    * **Movimiento en el Mapa (Grafo):** Se utiliza la función `getEdges` para mostrar rutas adyacentes desde la ubicación actual. El riesgo se recalcula diariamente (`cambiarRiesgo`) y el traslado se aplica al pasar de día.
    * **Sistema de Supervivientes (Árbol/TreeMap):** Operativo mediante `TreeMap`. Los personajes se insertan, buscan y eliminan dinámicamente. El sistema procesa correctamente las muertes si la salud, hambre o sed llegan a cero debido a las penalizaciones diarias del riesgo (`cambiarNecesidades`).
    * **Sistema de Expediciones (Listas y Matemáticas):** Se almacenan en una lista (`lista_expediciones`). El código calcula la probabilidad matemática con `calcular_exito` usando la librería `<math.h>`. Al pasar los días correspondientes, el código procesa los porcentajes otorgando botines (suministros o personajes) o matando exploradores en caso de fracaso (`procesar_expedicion`).
    * **Inventario y Suministros (Hashmap):** Los recursos globales se guardan en el mapa `inventario_grupo`. El menú de `gestionar_suministros` lee las estadísticas individuales del árbol de supervivientes, descuenta del inventario y evita superar los límites máximos (MAXSED, MAXHAMBRE, MAXSALUD).
    * **Eventos Aleatorios (Hashmap):** Hay un 50% de probabilidad diaria de lanzar un evento. Este se busca exitosamente en el mapa `Hashmap_eventos` generando un string numérico aleatorio.
      
* **Lo que falta en el código o está incompleto:**
    * **Ausencia de Historial de Refugios (Lista Enlazada):** Al trasladarse, el programa simplemente sobreescribe el string `ubicacion_actual`. No existe ninguna lista que almacene el recorrido previo por el mapa.
    * **Sistema de Audio Inactivo:** Las funciones para el comando de audio (`escribir_comando_audio`, `reproducir_musica` y la lógica del "Maguito Explosivo") se encuentran comentadas (`/* ... */`) en el archivo fuente, por lo que el juego no emite sonido en esta compilación.

## Contribución de los Integrantes
El desarrollo del proyecto, tanto en código como en documentación, se distribuyó de la siguiente manera:

* **Florencia Alvarado:** Redactó la justificación conceptual e implementación técnica del árbol AVL, incluyendo el análisis de complejidad en la gestión de supervivientes. A nivel de código, se encargó del diseño de dicho árbol y la lógica general de distribución de suministros.
* **Luciano Melo:** Redactó la descripción de la aplicación, propósito, visión general, lista de funcionalidades y el mapa de navegación. En el desarrollo, diseñó e implementó el grafo del mapa (lista de adyacencia), el sistema de movimiento entre zonas y la actualización de riesgos.
* **Matías Verschae:** Redactó la justificación e implementación técnica de las tablas hash, junto con el análisis de complejidad de inventario y expediciones. En código, programó las tablas hash y toda la lógica central de expediciones.
* **Matías Segovia:** Redactó la introducción, el resumen, la planificación y la conclusión del informe, coordinando la revisión ortográfica y lógica general. A nivel de código, estuvo a cargo del diseño e implementación de la cola de eventos, la pila de historial y la lista doblemente enlazada de refugios visitados.