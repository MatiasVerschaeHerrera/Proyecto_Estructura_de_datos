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

int main(){
  limpiarPantalla();
  char nombre[50];
  
  printf("Bienvenido a !!!\n");
  printf("Ingresa tu nombre: ");
  scanf(" %49[^\n]", nombre);
  presentacion(nombre);

  return 0;
}