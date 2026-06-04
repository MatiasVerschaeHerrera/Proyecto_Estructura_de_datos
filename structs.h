#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/stack.h"
#include "tdas/queue.h"
#include "tdas/heap.h"

// entidades basicas
typedef struct{
  int agua;
  int comida;
  int medicina;
} Recursos;

typedef struct{
  int nombre[50];
  int salud;
  int hambre;
} Sobreviviente;

typedef struct{
  char descripcion[100];
  int impacto_agua;
  int impacto_comida;
  int impacto_salud;
} Evento;

typedef struct{
  char nombre[50];
  Recursos recursos;
  List* adyacentes;
  int visitado;
} Refugio;

// estructura auxiliar para los caminos del grafo
typedef struct{
  Refugio* destinos;
  int riesgo;
} Camino;

// estructura para guardar en la pila para el deshacer
typedef struct{
  int dia;
  Refugio* ubicacion_momento;
  Recursos recursos_momento;
}EstadoTurno;

// Estado de la partida
typedef struct{
  int dia_actual;
  Refugio* ubicacion_actual;
  Recursos inventario_grupo;

  Map* base_refugios;
  Queue* cola_eventos;
  Stack* pila_deshacer;
  List* historial_viaje;
  Heap* ranking_sobrevivientes;
}  Partida;


#endif