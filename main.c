// bibliotecas importadas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
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
#include "tdas/graph.h"

int son_iguales_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

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


  return mapa;
}

Partida *iniciarPartida(char* nombre) {
  
  Partida* aux = (Partida *) malloc(sizeof(Partida));
  
  aux->dia_actual= 1;
  strcpy(aux->usuario, nombre);
  aux->Mapa = generarMapa();
  aux->ubicacion_actual = "Refugio";
  aux->riesgo_actual = 0;

  aux->inventario_grupo = map_create(son_iguales_strings);
  
  int* agua = (int *) malloc(sizeof(int));
  *agua = 8;
  map_insert(aux->inventario_grupo, "Agua", agua);
  
  int* comida = (int *) malloc(sizeof(int));
  *comida = 5;
  map_insert(aux->inventario_grupo, "Comida", comida);
  
  int* medicina = (int *) malloc(sizeof(int));
  *medicina = 3;
  map_insert(aux->inventario_grupo, "Medicina", medicina);

  return aux;
}

void gestionar_traslado(Partida *slot1) {
  limpiarPantalla();

  puts("=======================");
  printf("     MAPA DIA - %i     \n", slot1->dia_actual);
  puts("=======================");
  puts("*Escala de Riesgo: 1-10");
  printf("Ubicacion Actual: '%s', Riesgo: '%i'\n", slot1->ubicacion_actual, slot1->riesgo_actual);
  printf("\n");
  
  List* lista_aristas = getEdges(slot1->Mapa, slot1->ubicacion_actual);

  if(list_first(lista_aristas) == NULL) {
    puts("No hay caminos disponibles.");
    puts("Te has quedado acorralado!");
    presioneTeclaParaContinuar();
    return;
  }

  puts("-- Zonas Disponibles para el Traslado --");

  Edge *arista = (Edge *) list_first(lista_aristas);
  int cont = 1;
  while(arista != NULL) {
    printf("%i.- Ubicacion: '%s', Riesgo: '%i'\n", cont, arista->target, arista->weight);
    arista = list_next(lista_aristas);
    cont++;
  }
  printf("\n");

  puts("-- Seleccionar Destino --");
  puts(" *Escriba '0' para Volver al Menu");

  int opcion1;
  scanf(" %i", &opcion1);

  while(true){
    if(opcion1 > list_size(lista_aristas) + 1 || opcion1 < 0) {
      printf("Opcion Invalida.\n");
      scanf(" %i", &opcion1);
    } else {
      break;
    }
  }

  if(opcion1 == 0) return;
  
  arista = list_first(lista_aristas);
  for(int j = 1; j < opcion1; j++){
    arista = list_next(lista_aristas);
  }
  
  printf("Ubicacion a trasladar: '%s', Riesgo: '%i'\n", arista->target, arista->weight);
  puts("Confirmar traslado (s/n): ");

  char sn;
  scanf(" %c", &sn);
  if(sn == 's' || sn == 'S') {
    slot1->ubicacion_actual = arista->target;
    puts("Traslado Exitoso, el grupo se trasladara al terminar la jornada");
  } else {
    puts("Traslado Cancelado");
  }
  return;
}

void gestionar_expediciones(Partida *slot1) {


  return;
}
  
void gestionar_suministros(Partida *slot1) {


  return;
}

void mostrar_instrucciones() {
  puts("asdasdasdaswdawsfa");
  return;
}

int main() {
  limpiarPantalla();
  char opcion1;
  do {
    // aca muestra el menu con las opciones varias
    puts("==================");
    printf("     Menu     \n");
    puts("==================");

    puts("1) Ver Instrucciones");
    puts("2) Iniciar Partida");
    puts("3) Salir del Juego");

    printf("Ingrese su opción: ");
    scanf(" %c", &opcion1);

      switch (opcion1) {
        case '1':
          mostrar_instrucciones();
          break;
        case '2': {
          char nombre[50];
          
          printf("Bienvenido a !!!\n");
          printf("Ingresa tu nombre: ");
          scanf(" %49[^\n]", nombre);

          presentacion(nombre);
                  
          Partida* slot1 = iniciarPartida(nombre);

          char opcion2;
          do {
            limpiarPantalla();
            // aca muestra el menu con las opciones varias
            puts("==================");
            printf("     Dia %i     \n", slot1->dia_actual);
            puts("==================");

            puts("1) Trasladarce");
            puts("2) Expediciones");
            puts("3) Administrar Suministros");
            puts("4) -- Terminar Jornada --");
            printf("\n");
            puts("5) Finalizar Partida");

            printf("Ingrese su opción: ");
            scanf(" %c", &opcion2);

            switch (opcion2) {
              case '1': {
                gestionar_traslado(slot1);
                break;
              }
              case '2': {
                gestionar_expediciones(slot1);
                break;
              }
              case '3': {
                gestionar_suministros(slot1);
                break;
              }
              case '4': {
                puts("Seguro que quiere finalizar la jornada? (s/n): ");

                char sn;
                scanf(" %c", &sn);
                if(sn == 's' || sn == 'S') {
                  slot1->dia_actual++;
                  puts("-- Dia finalizado --");
                  puts("-- Avanzando al siguiente día... --");
                } else {
                  puts("Operacion cancelada.");
                  
                }
                
                break;
              }
              case '5':
                return 0;
              default: {
                puts("Operacion invalida.");
                
              }
            }
            // Evitamos pausar y limpiar pantalla si el usuario eligió salir
            if (opcion2 != '5') {
              presioneTeclaParaContinuar();
            }

          } while (slot1->dia_actual <= 3);
          free(slot1);
          break;
        }
        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
    }
    if (opcion1 != '3') {
      presioneTeclaParaContinuar();
      limpiarPantalla();
    }
  } while (opcion1 != '3');
  
  return 0;
}