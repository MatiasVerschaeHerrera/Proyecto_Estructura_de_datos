// bibliotecas importadas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// archivos desde la carpeta tdas
#include "tdas/extra.h"
#include "tdas/heap.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/multimap.h"
#include "tdas/queue.h"
#include "tdas/stack.h"
#include "historia.h"
#include "structs.h"

Graph *generarMapa() {
  Graph *mapa = createGraph();
  addNode(mapa, "Refugio");
  addNode(mapa, "Hospital");
  addNode(mapa, "Distrito-Industrial");
  addNode(mapa, "Distrito-Comercial");
  addNode(mapa, "Distrito-Residencial");
  addNode(mapa, "Puerto");

  srand(time(NULL));
  int riesgo;

  // Refugio
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Refugio", "Puerto", riesgo);
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Refugio", "Distrito-Residencial", riesgo);
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Refugio", "Distrito-Comercial", riesgo);

  // Hospital
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Hospital", "Distrito-Comercial", riesgo);

  // Distrito Industrial
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Distrito-Industrial", "Distrito-Comercial", riesgo);
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Distrito-Industrial", "Puerto", riesgo);

  // Distrito Comericial
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Distrito-Comercial", "Distrito-Residencial", riesgo);
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Distrito-Comercial", "Hospital", riesgo);

  // Distrito Residencial
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Distrito-Residencial", "Refugio", riesgo);
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Distrito-Residencial", "Puerto", riesgo);

  // Puerto
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Puerto", "Distrito-Industrial", riesgo);
  riesgo = rand() % 10 + 1;
  addEdge(mapa, "Puerto", "Refugio", riesgo);
}

Partida *iniciarPartida(char* nombre) {
  
  Partida* aux = (Partida *) malloc(sizeof(Partida));
  
    aux->dia_actual= 1;
    strcpy(aux->usuario, nombre);
    aux->Mapa = generarMapa();
    aux->ubicacion_actual = "Refugio";

    aux->inventario_grupo = map_create(is_equal_string);
  int* agua = (int *) malloc(sizeof(int));
  *agua = 8;
  map_insert(aux->inventario_grupo, "Agua", agua);
  
  int* comida = (int *) malloc(sizeof(int));
  *comida = 5;
  map_insert(aux->inventario_grupo, "Comida", agua);
  
  int* medicina = (int *) malloc(sizeof(int));
  *medicina = 3;
  map_insert(aux->inventario_grupo, "Medicina", agua);

  return aux;
}

int main(){
  limpiarPantalla();
  char nombre[50];
  
  printf("Bienvenido a !!!\n");
  printf("Ingresa tu nombre: ");
  scanf(" %49[^\n]", nombre);
  presentacion(nombre);

  Partida* slot1 = iniciarPartida();
  /*
  typedef struct{
  int dia_actual;
  char usuario[MAXNOMBRE];
  Graph* Mapa;
  Refugio* ubicacion_actual;
  Map* inventario_grupo;

  Map* base_refugios;
  Queue* cola_eventos;
  //Stack* pila_deshacer;
  List* historial_viaje;
  //Heap* ranking_sobrevivientes;
  }  Partida;
*/
  
  char opcion;
      do {
          // aca muestra el menu con las opciones varias
          puts("==================");
          printf("     Menu     ");
          puts("==================");

          puts("1) Glosario de grafico");
          puts("2) Búsqueda en Profundidad");
          puts("3) Buscar en Anchura");
          puts("4) Buscar A*");
          puts("5) Salir");

          printf("Ingrese su opción: ");
          scanf(" %c", &opcion);

          switch (opcion) {
              case '1':
                  mostrarGlosario();
                  break;
              case '2':
                  int iteracionesDFS = 0; 
                  deep_first_search(&estado_inicial, &iteracionesDFS);
                  printf("Cantidad de iteraciones = %i\n", iteracionesDFS);
                  break;
              case '3':
                  int iteracionesBFS = 0;
                  breadth_first_search(&estado_inicial, &iteracionesBFS);
                  printf("Cantidad de iteraciones = %i\n", iteracionesBFS);
                  break;
              case '4':
                  int iteracionesAstar = 0;
                  busqueda_Astar(&estado_inicial, &iteracionesAstar);
                  printf("Cantidad de iteraciones = %i\n", iteracionesAstar);
                  break;
          }

          // Evitamos pausar y limpiar pantalla si el usuario eligió salir
          if (opcion != '5') {
              presioneTeclaParaContinuar();
              limpiarPantalla();
          }

    } while (opcion != '4');

    return 0;
  }

  
  printf("Hola0");
  
  return 0;
}