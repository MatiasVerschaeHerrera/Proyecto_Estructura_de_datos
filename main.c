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
#include "tdas/graph.h"
#include "tdas/treemap.h"
#include "historia.h"
#include "structs.h"


int son_iguales_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}
int son_menores_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) < 0;
}

void cambiarRiesgo(Partida *slot1) {
  List *lista_ubicaciones = getAllLabels(slot1->Mapa);
  char *ubicacion = list_first(lista_ubicaciones);

  while(ubicacion != NULL) {
    List *lista_aristas = getEdges(slot1->Mapa, ubicacion);
    Edge *arista = list_first(lista_aristas);

    while(arista != NULL) {
      arista->weight = 1 + rand() % 100;
      arista = list_next(lista_aristas);
    }
    ubicacion = list_next(lista_ubicaciones);
  }
  free(lista_ubicaciones);
  return;
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
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Puerto", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Distrito-Residencial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Distrito-Comercial", riesgo);

  // Hospital
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Hospital", "Distrito-Comercial", riesgo);

  // Distrito Industrial
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Industrial", "Distrito-Comercial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Industrial", "Puerto", riesgo);

  // Distrito Comericial
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Comercial", "Distrito-Residencial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Comercial", "Hospital", riesgo);

  // Distrito Residencial
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Residencial", "Refugio", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Residencial", "Puerto", riesgo);

  // Puerto
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Puerto", "Distrito-Industrial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Puerto", "Refugio", riesgo);


  return mapa;
}

Sobreviviente *crearSobreviviente(char *nombre) {
  Sobreviviente *aux = (Sobreviviente *) malloc(sizeof(Sobreviviente));
  strcpy(aux->nombre, nombre);
  aux->salud = 10;
  aux->hambre = 8;
  aux->sed = 4;

  return aux;
}

TreeMap* iniciar_sobrevivientes() {

  TreeMap* arbol = createTreeMap(son_menores_strings);

  Sobreviviente *aux1 = crearSobreviviente("Segovia");
  Sobreviviente *aux2 = crearSobreviviente("Verscahe");
  Sobreviviente *aux3 = crearSobreviviente("Florencia");
  Sobreviviente *aux4 = crearSobreviviente("Luciano");

  insertTreeMap(arbol, aux1->nombre, aux1);
  insertTreeMap(arbol, aux2->nombre, aux2);
  insertTreeMap(arbol, aux3->nombre, aux3);
  insertTreeMap(arbol, aux4->nombre, aux4);

  return arbol;
}

Partida *iniciarPartida(char* nombre) {
  
  Partida* aux = (Partida *) malloc(sizeof(Partida));
  
  aux->dia_actual= 1;
  strcpy(aux->usuario, nombre);
  aux->Mapa = generarMapa();
  aux->ubicacion_actual = "Refugio";
  aux->riesgo_actual = 0;

  aux->inventario_grupo = map_create(son_iguales_strings);
  aux->sobrevivientes = iniciar_sobrevivientes();
  aux->lista_muertos = list_create();
  
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
  puts("*Riesgo Bajo: no hay penalidad");
  puts("*Riesgo Medio: -1 adicional a necesidades de comida");
  puts("*Riesgo Alto: -1 adicional a todas las necesidades");
  printf("Ubicacion Actual: '%s', Riesgo: ", slot1->ubicacion_actual);
  if(slot1->riesgo_actual < 51){
    printf("'Bajo'\n");
  } else if(slot1->riesgo_actual < 81) {
    printf("'Medio'\n");
  } else {
    printf("'Alto'\n");
  }
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
    printf("Ubicacion: '%s', Riesgo: ", arista->target);
    if(arista->weight < 51){
      printf("'Bajo'\n");
    } else if(arista->weight < 81) {
      printf("'Medio'\n");
    } else {
      printf("'Alto'\n");
    }
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
  
  printf("Ubicacion a trasladar: '%s', Riesgo: ", arista->target);
  if(arista->weight < 51){
    printf("'Bajo'\n");
  } else if(arista->weight < 81) {
    printf("'Medio'\n");
  } else {
    printf("'Alto'\n");
  }
  puts("Confirmar traslado (s/n): ");

  char sn;
  scanf(" %c", &sn);
  if(sn == 's' || sn == 'S') {
    slot1->ubicacion_actual = arista->target;
    slot1->riesgo_actual = arista->weight;
    puts("Traslado Exitoso, el grupo se trasladara al terminar la jornada.");
  } else {
    puts("Traslado Cancelado.");
  }
  return;
}

void gestionar_expediciones(Partida *slot1) {


  return;
}


void gestionar_suministros(Partida *slot1) {
  Pair *par = firstTreeMap(slot1->sobrevivientes);
  if(par == NULL) return;
  
  puts("==============================");
  printf("     NECESIDADES DIA - %i     \n", slot1->dia_actual);
  puts("==============================");
  
  while(par != NULL) {
    Sobreviviente *aux = par->value;
    printf("  %s - Salud: %i - Hambre: %i - Sed: %i\n", aux->nombre, aux->salud, aux->hambre, aux->sed);
    par = nextTreeMap(slot1->sobrevivientes);
  }
  printf("\n");
  return;
}

void mostrar_instrucciones() {
  puts("asdasdasdaswdawsfa");
  return;
}

void metricas_dia(Partida *slot1) {

  // Sistema de riesgo afecta necesidades
  int bonoRiesgoM = 0;
  int bonoRiesgoA = 0;
  int bonoRiesgoS = 0;

  if(slot1->riesgo_actual < 51){
    bonoRiesgoM = 0;
    bonoRiesgoA = 0;
    bonoRiesgoS = 0;
  } else if(slot1->riesgo_actual < 81) {
    bonoRiesgoM = 0;
    bonoRiesgoA = 1;
    bonoRiesgoS = 0;
  } else {
    bonoRiesgoM = 1;
    bonoRiesgoA = 1;
    bonoRiesgoS = 1;
  }

  int desventajaM = 1 + bonoRiesgoM;
  int desventajaA = 1 + bonoRiesgoA;
  int desventajaS = 1 + bonoRiesgoS;
  
  Pair* par = firstTreeMap(slot1->sobrevivientes);

  while(par != NULL) {
    Sobreviviente *sobreviviente = par->value;
    
    sobreviviente->salud -= desventajaM;
    sobreviviente->hambre -= desventajaA;
    sobreviviente->sed -= desventajaS;

    if(sobreviviente->salud <= 0 || sobreviviente->hambre <= 0 || sobreviviente->sed <= 0) {
      list_pushBack(slot1->lista_muertos, strdup(sobreviviente->nombre));
    } 
    par = nextTreeMap(slot1->sobrevivientes);
  }
  
  char *nombre = (char *) list_first(slot1->lista_muertos);
  while(nombre != NULL) {
    if(searchTreeMap(slot1->sobrevivientes, nombre) != NULL) {
      printf("> Ha muerto '%s'\n.", nombre);
      eraseTreeMap(slot1->sobrevivientes, nombre);
    }
    nombre = list_next(slot1->lista_muertos);
  }

  cambiarRiesgo(slot1);
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
                  metricas_dia(slot1);
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

          } while (slot1->dia_actual <= 20 && firstTreeMap(slot1->sobrevivientes) != NULL);
          
          if(firstTreeMap(slot1->sobrevivientes) != NULL) {
            puts("Has Ganado!!");
          } else {
            puts("Has Perdido!!");
          }
          free(slot1);
          break;
        }

        
    }
    if (opcion1 != '3') {
      presioneTeclaParaContinuar();
      limpiarPantalla();
    }
  } while (opcion1 != '3');
  
  return 0;
}