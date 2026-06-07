#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/stack.h"
#include "tdas/queue.h"
#include "tdas/heap.h"
#include "tdas/treemap.h"
#include "tdas/graph.h"
#define MAXNOMBRE 50
#define MAXDESCRIPCION 200

// entidades basicas
/*
typedef struct{
  int agua;
  int comida;
  int medicina;
} Recursos;
*/

typedef struct{
  char nombre[MAXNOMBRE];
  int salud;
  int hambre;
  int sed;
} Sobreviviente;

typedef struct{
  char descripcion[MAXDESCRIPCION];
  int impacto_agua;
  int impacto_comida;
  int impacto_salud;
  int impacto_sobrevivientes;
} Evento;

typedef struct{
  char nombre[MAXNOMBRE];
  List* adyacentes;
  int visitado;
} Ubicacion;

// estructura auxiliar para los caminos del grafo
typedef struct{
  Ubicacion* destinos;
  int riesgo;
} Camino;

// estructura para guardar en la pila para el deshacer
/*
typedef struct{
  int dia;
  Refugio* ubicacion_momento;
  Recursos recursos_momento;
} EstadoTurno;
*/
// ver esto

// Estado de la partida
typedef struct{
  int dia_actual;
  char usuario[MAXNOMBRE];
  Graph* Mapa;
  char* ubicacion_actual;
  int riesgo_actual;
  Map* inventario_grupo;
  TreeMap * sobrevivientes;
  Map* base_refugios;
  Map* Hashmap_eventos;
  //Stack* pila_deshacer;
  List* lista_muertos;
  //Heap* ranking_sobrevivientes;
}  Partida;

#endif