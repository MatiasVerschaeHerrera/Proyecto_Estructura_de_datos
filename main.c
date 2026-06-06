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

void gestionar_traslado() {


  return;
}

void gestionar_expediciones() {


  return;
}
  
void gestionar_suministros() {


  return;
}
  
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
  
  char opcion1;
  do {
    // aca muestra el menu con las opciones varias
    puts("==================");
    printf("     Menu     ");
    puts("==================");

    puts("1) Ver Instrucciones");
    puts("2) Iniciar Partida");
    puts("3) Salir del Juego");

    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

      switch (opcion) {
        case '1':
          mostrar_instrucciones();
          break;
        case '2': {
          char nombre[50];
                  
          printf("Bienvenido a !!!\n");
          printf("Ingresa tu nombre: ");
          scanf(" %49[^\n]", nombre);
          presentacion(nombre);
                  
          Partida* slot1 = iniciarPartida();

          char opcion2;
          do {
            // aca muestra el menu con las opciones varias
            puts("==================");
            printf("     Dia %i     ", slot1->dia);
            puts("==================");

            puts("1) Trasladarce");
            puts("2) Expediciones");
            puts("3) Administrar Suministros");
            puts("4) -- Terminar Jornada --");

            printf("Ingrese su opción: ");
            scanf(" %c", &opcion);

            switch (opcion) {
              case '1': {
                gestionar_traslado();
                break;
              }
              case '2': {
                gestionar_expediciones();
                break;
              }
              case '3': {
                gestionar_suministros();
                break;
              }
            }
            // Evitamos pausar y limpiar pantalla si el usuario eligió salir
            if (opcion != '4') {
              presioneTeclaParaContinuar();
              limpiarPantalla();
            }

          } while (opcion != '4');
          if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
          }
          slot1->dia++;
        }
        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
    }
    if (opcion != '3') {
      presioneTeclaParaContinuar();
      limpiarPantalla();
    }
  } while (opcion != '3');
  
  return 0;
}