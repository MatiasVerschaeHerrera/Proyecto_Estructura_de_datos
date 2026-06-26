// bibliotecas importadas 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
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
#include "tdas/hashmap.h" 
#include "historia.h"
#include "structs.h"
#include "eventos.h"


#define MAXSUPERVIVIENTES 10
#define MAXCOMIDA 30
#define MAXAGUA 40
#define MAXMEDICINA 25

#define MAXSALUD 8
#define MAXHAMBRE 10
#define MAXSED 4

#define EVENTO_MAGUITO "1"
#define TOTAL_EVENTOS 22

int son_iguales_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}
int son_menores_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) < 0;
}

void cambiarRiesgo(Partida *slot1, int aux) {

  if(slot1->se_traslado == 0) {
    if (aux > 0) {
      slot1->riesgo_actual = aux;
    } else {
      slot1->riesgo_actual = 1 + rand() % 100;
    }
    
    printf("Riesgo Zona actual: ");
    if(slot1->riesgo_actual < 51){
      printf("'Bajo'\n");
    } else if(slot1->riesgo_actual < 81) {
      printf("'Medio'\n");
    } else {
      printf("'Alto'\n");
    }
    printf("\n");
  }
  
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

  // Refugio si ves esto comenta sixsaeven despues de cada sixseven sixseven
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Puerto", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Distrito-Residencial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Distrito-Comercial", riesgo);

  // Hospital POLLA RICA
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Hospital", "Distrito-Comercial", riesgo);

  // Distrito Industrial Tripalosky
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Industrial", "Distrito-Comercial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Industrial", "Puerto", riesgo);

  // Distrito Comercial Puchaina Negra
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Comercial", "Distrito-Residencial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Distrito-Comercial", "Hospital", riesgo);

  // Distrito Residencial Mamadores
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

void ingresar_nombre(Partida *slot1, char nombre[]) {
  puts("-- Ingresar Nombre del Nuevo Superviviente --");
  while(true) {
    scanf(" %49[^\n]", nombre);

    for(int i = 0; nombre[i] != '\0'; i++) {
      if( !isalpha(nombre[i]) ) {
        puts("> Nombre invalido, debe tener solo caracteres alfabeticos.");
        continue;
      }
    }
    
    formato_nombre(nombre);

    if(strlen(nombre) == 0) {
      puts("> Nombre invalido, debe tener como minimo un caracter.");
      continue;
    } 
    if(strcmp(nombre, "0") == 0) {
      puts("> No se permite ese nombre.");
      continue;
    }
    if(strcmp(nombre, "Magito Explosivo") == 0) {
      puts("> No se permite este nombre, quien sabe si está por allí afuera esperando.");
      continue;
    }
    
    Pair *par = searchTreeMap(slot1->sobrevivientes, nombre);
    if(par != NULL) {
      puts("> Ya existe un superviviente con ese nombre, no queremos confundirlos asi que elige otro.");
      continue;
    }

    break;
  }

  return;
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
  aux->se_traslado = 0;
  aux->inventario_grupo = map_create(son_iguales_strings);
  aux->sobrevivientes = iniciar_sobrevivientes();
  aux->lista_muertos = list_create();
  aux->lista_expediciones = list_create();
  aux->expedicion_dia = false;
  aux->Hashmap_eventos = createMap(40);
  
  crear_eventos(aux->Hashmap_eventos);
  
  Recursos *suministros = malloc(sizeof(Recursos));
  suministros->agua = 8;
  suministros->comida = 5;
  suministros->medicina = 3;
  
  map_insert(aux->inventario_grupo, "Recursos", suministros);

  return aux;
}

void gestionar_traslado(Partida *slot1) {
  limpiarPantalla();
  if(slot1->se_traslado == 0){
    puts("================================");
    printf("     GESTIONAR TRASLADO - %i     \n", slot1->dia_actual);
    puts("================================");
    puts(" *Riesgo Bajo: no hay penalidad");
    puts(" *Riesgo Medio: -1 adicional a necesidades de comida");
    puts(" *Riesgo Alto: -1 adicional a todas las necesidades");
    printf("\n");
    
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
      puts("> No hay caminos disponibles.");
      puts("> Te has quedado acorralado!");
      presioneTeclaParaContinuar();
      return;
    }
  
    puts("-- Zonas Disponibles para el Traslado --");
  
    Edge *arista = (Edge *) list_first(lista_aristas);
    int cont = 1;
    while(arista != NULL) {
      printf("%i) Ubicacion: '%s', Riesgo: ", cont, arista->target);
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
  
    puts("-- Seleccionar Número del Destino --");
    puts(" *Escriba '0' para Volver al Menu");
  
    int opcion1;
    scanf(" %i", &opcion1);
  
    while(true){
      if(opcion1 > list_size(lista_aristas) + 1 || opcion1 < 0) {
        printf("> Opcion Invalida.\n");
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
    
    printf(" *Ubicacion a trasladar: '%s', Riesgo: ", arista->target);
    if(arista->weight < 51){
      printf("'Bajo'\n");
    } else if(arista->weight < 81) {
      printf("'Medio'\n");
    } else {
      printf("'Alto'\n");
    }
    puts(" *Confirmar traslado (s/n): ");

    char sn;
    scanf(" %c", &sn);
    if(sn == 's' || sn == 'S') {
      slot1->ubicacion_actual = arista->target;
      slot1->riesgo_actual = arista->weight;
      slot1->se_traslado = 1;
      puts("> Traslado Exitoso, el grupo se trasladara al terminar la jornada.");
    } else {
      puts("> Traslado Cancelado.");
    }
  }
  else{
    puts("================================");
    printf("     GESTIONAR TRASLADO - %i    \n", slot1->dia_actual);
    puts("================================");
    puts("> Avance de dia para poder trasladarse");
  }
  return;
}

void mostrar_necesidades(TreeMap *mapa) {
  Pair *par = firstTreeMap(mapa);
  if(par == NULL) return;
  while(par != NULL) {
    Sobreviviente *aux = par->value;
    printf("  %s - Salud: %i - Hambre: %i - Sed: %i\n", aux->nombre, aux->salud, aux->hambre, aux->sed);
    par = nextTreeMap(mapa);
  }
  printf("\n");
}

int seleccionar_sobrevivientes(Partida *slot1, Sobreviviente *arreglo_seleccionados[], int maxselec) {
  
  puts("-- Sobrevivientes Disponibles --");
  mostrar_necesidades(slot1->sobrevivientes);

  puts(" *Se permiten un maximo de 5 supervivientes por expedicion."); //MAXSUPERVIVIENTES
  int numselec = 0;
  char nombre[MAXNOMBRE];

  while(numselec < maxselec) {
    puts(" *Ingrese el nombre del superviviente/s:");
    puts(" *(Escriba 0 para terminar la selección)");
    
    scanf(" %49s", nombre);
    if(strcmp(nombre, "0") == 0) break;

    formato_nombre(nombre);

    Pair *par = searchTreeMap(slot1->sobrevivientes, nombre);
    if(par == NULL) {
      puts("> Superviviente no encontrado.");
      continue;
    }
    
    Sobreviviente *superviviente = par->value;
    bool yaesta = false;
    for(int i = 0; i < numselec; i++) {
      if(strcmp(arreglo_seleccionados[i]->nombre, superviviente->nombre) == 0) { 
        yaesta = true;
        break;
      }
    }
    if (yaesta) {
      puts("> Este superviviente ya fue seleccionado.");
      continue;
    }

    arreglo_seleccionados[numselec] = superviviente;
    numselec++;
    printf("\n> Has seleccionado a '%s'.\n\n", superviviente->nombre);
    //eraseTreeMap();
  }
  
  return numselec;
}

void seleccionar_ubicacion(Partida *slot1, char **ubicacion, int *riesgo) {
  
  List *lista_aux = getEdges(slot1->Mapa, slot1->ubicacion_actual);

  puts("\n-- Ubicaciones Dsiponibles para la Expedicion --\n");

  Edge *arista = list_first(lista_aux);
  int i = 1;

  while(arista != NULL) {

    printf("%i) Ubicacion: '%s', Riesgo: ", i, arista->target);
      if(arista->weight < 51) {
        printf("'Bajo'\n");
      } else if(arista->weight < 81) {
        printf("'Medio'\n");
      } else {
        printf("'Alto'\n");
      }
    
    i++;
    arista = list_next(lista_aux);
  }
  
  printf("\n");
  
  puts(" *Seleccione el numero del destino");
  puts(" *(Escriba 0 para cancelar la selección)");

  int opcion;
  scanf(" %i", &opcion);
  if(opcion == 0) {
    return;
  }

  if(opcion < 1 || opcion >= i) {
    puts("> Destino invalido.");
    return;
  }

  arista = list_first(lista_aux);

  for(int j = 1; j < opcion; j++) {
    arista = list_next(lista_aux);
  }

  *ubicacion = arista->target;
  *riesgo = arista->weight;
  
  return;
}

int calcular_exito(int numselec, int riesgo, int agua, int comida, int medicina) {
  
  int probabilidad_exito = 80;

  probabilidad_exito += agua * 2; 
  probabilidad_exito += comida * 2; 
  probabilidad_exito += medicina * 3; 
  if(agua + comida + medicina > 8) {
    probabilidad_exito -= 10;
  }
  
  probabilidad_exito += numselec * 6;
  if(numselec >= 4) {
    probabilidad_exito -= 8;
  }

  probabilidad_exito -= riesgo / 2;

  if (probabilidad_exito < 10) {
    probabilidad_exito = 10;
  } else if (probabilidad_exito > 90) {
    probabilidad_exito = 90;
  }
  
  return probabilidad_exito;
}

void asignar_expedicion(Partida *slot1, Sobreviviente *arreglo_seleccionados[], int numselec, int probexito, char *destino, int riesgo, int agua, int comida, int medicina) {

  Expedicion *expedicion = (Expedicion *) malloc(sizeof(Expedicion));

  expedicion->sobrevivientes = list_create();

  for(int i = 0; i < numselec; i++) {
    list_pushBack(expedicion->sobrevivientes, arreglo_seleccionados[i]);
    eraseTreeMap(slot1->sobrevivientes, arreglo_seleccionados[i]->nombre);
  }
    
  strcpy(expedicion->destino, destino);
  expedicion->agua = 0;
  expedicion->comida = 0;
  expedicion->medicina = 0;
  expedicion->riesgo = riesgo;
  expedicion->prob_exito = probexito;
  expedicion->dias = 1 + rand() % 3;

  list_pushBack(slot1->lista_expediciones, expedicion);

  printf("\n> Expedicion iniciada, los supervivientes volveran en '%i' dias.\n", expedicion->dias);
    
  return;
}

void asignar_recursos_expedicion(Partida * slot1, int* agua, int *comida, int* medicina){
  MapPair * pair = map_search(slot1->inventario_grupo, "Recursos");
  if (pair == NULL) return;
  Recursos * recursos = (Recursos*) pair -> value;

  puts("================================");
  printf("Recursos Disponibles \n");
  puts("================================");
  printf("1) Agua = %i\n", recursos->agua);
  printf("2) Comida = %i\n", recursos->comida);
  printf("3) Medicina = %i\n", recursos->medicina);
  printf("4) Finalizar elección\n\n");

  char opcion;
  do {
    puts("-- Seleccione el número del suministro  --");
    scanf(" %c", &opcion);

    if(opcion == '4') break;

    switch(opcion){
      case '1': {
        if(recursos->agua <= 0) { puts("No hay agua disponible."); break; }

        else{
          puts("Cuantas unidades de agua llevan? (0 para ninguna/cancelar)");
          scanf(" %i", agua);
          while(*agua < 0 || *agua > recursos ->agua){
            printf("Valor iválido, ingrese nuevamente");
            scanf(" %i",agua);
          }
        }
        break;
      }
      case '2': {
        if(recursos->comida <= 0) { puts("No hay comida disponible."); break; }

        else{
          puts("Cuantas unidades de comida llevan? (0 para ninguna/cancelar)");
          scanf(" %i", comida);
          while(*comida < 0 || *comida > recursos ->comida){
            printf("Valor iválido, ingrese nuevamente");
            scanf(" %i",comida);
          }
        }
        break;
      }
      case '3': {
        if(recursos->medicina <= 0) { puts("No hay medicina disponible."); break; }

        else{
          puts("Cuantas unidades de medicina llevan? (0 para ninguna/cancelar)");
          scanf(" %i", medicina);
          while(*medicina < 0 || *medicina > recursos ->medicina){
            printf("Valor iválido, ingrese nuevamente");
            scanf(" %i",medicina);
          }
        }
        break;
      }
    }
  } while(opcion != '4');

  recursos->agua -= *agua;
  recursos->comida -= *comida;
  recursos->medicina -= *medicina;

}

void gestionar_expediciones(Partida *slot1) {
  limpiarPantalla();
  puts("================================");
  printf("     GESTIONAR EXPEDICIONES      \n", slot1->dia_actual);
  puts("================================");

  if(slot1->expedicion_dia == true) {
    puts(" *Solo se permite una expedicion por dia.");
    return;
  }

  int agua = 0;
  int comida = 0;
  int medicina = 0;
  int maxselec = 5;
  int numselec = 0;
  int riesgo = -1;
  int probexito = 100;
  Sobreviviente *arreglo_seleccionados[maxselec];
  char *ubicacion = NULL;
  
  numselec = seleccionar_sobrevivientes(slot1, arreglo_seleccionados, maxselec);

  if(numselec == 0) {
    puts("> Expedicion cancelada, no se selecciono ningun sobreviviente.");
    return;
  }

  seleccionar_ubicacion(slot1, &ubicacion, &riesgo);
  
  if(riesgo == -1) {
    puts("> Expedicion cancelada, no se selecciono ninguna ubicacion.");
    return;
  }

  asignar_recursos_expedicion(slot1, &agua, &comida, &medicina);
  
  probexito = calcular_exito(numselec, riesgo, agua, comida, medicina);
  printf("> Expedicion a '%s', Sobrevivientes: '%i', Probabilidad de exito: %i% \n", ubicacion, numselec, probexito);
  
  puts(" *Confirmar expedicion? (s/n)");
  char sn;
  scanf(" %c", &sn);
  if(sn != 's' && sn != 'S') {
    puts("> Expedicion cancelada.");
    MapPair * pair = map_search(slot1 -> inventario_grupo, "Recursos");
    Recursos * recursos = (Recursos*) pair -> value;

    recursos -> agua += agua;
    recursos -> comida += comida;
    recursos -> medicina += medicina;
    
    return;
  }

  asignar_expedicion(slot1, arreglo_seleccionados, numselec, probexito, ubicacion, riesgo, agua, comida, medicina);

  slot1->expedicion_dia = true;
  
  return;
}

/*int TreeMap_size(TreeMap* mapa) {
  int cont = 0;

  Pair *aux = firstTreeMap(mapa);

  while(aux != NULL) {
    cont++;
    aux = nextTreeMap(mapa);
  }

  return cont;
}*/

Pair *TreePair_aleatorio(TreeMap *mapa) {
  int total = TreeMapSize(mapa);

  int i = rand() % total;
  Pair *par = firstTreeMap(mapa);
  for(int j = 0; j < i; j++) {
    par = nextTreeMap(mapa);
  }
  return par;
}

void procesar_expedicion(Partida *slot1) {
  
  Expedicion *expedicion = list_first(slot1->lista_expediciones);
  
  while(expedicion != NULL) {
    Expedicion *temp = list_next(slot1->lista_expediciones);
    
    int random = rand() % 100;

    expedicion->dias--;

    if(expedicion->dias > 0) {

      expedicion = list_next(slot1->lista_expediciones);
      
      continue;
    }
    
    if (random < expedicion->prob_exito) {
      printf("> La expedicion ha sido un EXITO!\n");
      
      Sobreviviente *aux = list_first(expedicion->sobrevivientes);

      while(aux != NULL) {
        insertTreeMap(slot1->sobrevivientes, aux->nombre, aux);
        aux = list_next(expedicion->sobrevivientes);
      }
      
      int numsupervivientes = TreeMapSize(slot1->sobrevivientes);
      int botin = rand() % 3;
      if(botin == 0) {
        
        MapPair *aux = map_search(slot1->inventario_grupo, "Recursos");
        if(aux != NULL) {
          Recursos *suministros = (Recursos*) aux->value;
          if(suministros->medicina > MAXMEDICINA) break;
          int ganadoM = 1 + rand() % 3;
          printf("> Has ganado '%i' de Medicina\n", ganadoM);
          suministros->medicina += ganadoM;
        }
        
      } else if(botin == 1) {
        
        MapPair *aux = map_search(slot1->inventario_grupo, "Recursos");
        if(aux != NULL) {
          Recursos *suministros = (Recursos*) aux->value;
          if(suministros->medicina <= MAXAGUA) {
            int ganadoA = 1 + rand() % 4;
            printf("> Has ganado '%i' de Agua\n", ganadoA);
            suministros->agua += ganadoA;
          } else if(suministros->medicina <= MAXCOMIDA) {
            int ganadoC = 1 + rand() % 5;
            printf("> Has ganado '%i' de Comida\n", ganadoC);
            suministros->comida += ganadoC;
          }
        }
        
      } else {
        
        puts("> El equipo de expedicion ha encontrado a un nuevo superviviente!!");
        
        if (numsupervivientes > MAXSUPERVIVIENTES) {
          puts(" *Se ha alcanzado el maximo de supervivientes, el refugio no da a basto, lo has dejado ir");
          break;
        }
        char nombre[MAXNOMBRE];
        
        ingresar_nombre(slot1, nombre);
        Sobreviviente *aux = crearSobreviviente(nombre);
        insertTreeMap(slot1->sobrevivientes, aux->nombre, aux);
        printf("\n> '%s' se ha unido al refugio!\n\n", nombre);
        
      }

      
  
    } else {
      printf("> La expedicion ha sido un FRACASO!\n");
      int muertos = 1 + rand() % 2;

      Sobreviviente *aux = list_first(expedicion->sobrevivientes);

      while(aux != NULL) {
        if(muertos > 0) {
          printf("> Ha muerto '%s' en la expedicion\n", aux->nombre);
          muertos--;
          list_pushBack(slot1->lista_muertos, aux->nombre);
        } else {
          insertTreeMap(slot1->sobrevivientes, aux->nombre, aux);
        }
        
        aux = list_next(expedicion->sobrevivientes);
      }
      //free(aux);
    }

    list_popFront(slot1->lista_expediciones);
    free(expedicion->sobrevivientes);
    free(expedicion);
    expedicion = temp;
  }
  
  return;
}

/*void mostrar_suministros(Map *inventario) {
  puts("-- Suministros en el Inventario --");
  MapPair *par = map_first(inventario);
  if(par == NULL) return;
  while(par != NULL) {
    printf("%s = %i\n", par->key, *(int*)par->value);
    par = map_next(inventario);
  }
  printf("\n");
}
*/

void gestionar_suministros(Partida *slot1) {
  
  TreeMap *mapa = slot1->sobrevivientes;
  Map *inventario = slot1->inventario_grupo;
  
  MapPair *par = NULL;
  Recursos *recursos = NULL;

  char opcion;
  do {
    limpiarPantalla();

    puts("==============================");
    puts("     GESTIONAR SUMINISTROS    ");
    puts("==============================");

    puts("-- Necesidades de los supervivientes --");
    mostrar_necesidades(mapa);

    par = map_search(inventario, "Recursos");

    puts("-- Suministros en el Inventario --");

    if(par == NULL) return;
    recursos = (Recursos*) par->value;

    printf("1) Agua = %i\n", recursos->agua);
    printf("2) Comida = %i\n", recursos->comida);
    printf("3) Medicina = %i\n", recursos->medicina);
    printf("4) Volver\n\n");

    puts("-- Seleccione un suministro --");
    scanf(" %c", &opcion);

    if(opcion == '4') break;

    char nombre_sv[MAXNOMBRE];
    puts("* Ingrese el nombre del superviviente (0 para cancelar):");
    scanf(" %49[^\n]", nombre_sv);
    if(strcmp(nombre_sv, "0") == 0) continue;

    formato_nombre(nombre_sv);
    Pair *encontrado = searchTreeMap(mapa, nombre_sv);
    if(encontrado == NULL) {
      puts("> Superviviente no encontrado.");
      continue;
    }
    Sobreviviente *sv = (Sobreviviente*) encontrado->value;

    switch(opcion) {
      case '1': {
        if(recursos->agua <= 0) { 
          puts("No hay agua disponible."); 
          break; 
        }

        if(sv->sed >= MAXSED) {
          puts("> Este superviviente ya tiene la sed al maximo.");
          break;
        }

        sv->sed += 1;
        recursos->agua -= 1;

        if(sv->sed > MAXSED) {
          sv->sed = MAXSED;
        }

        printf("> Agua administrada a '%s'.\n", sv->nombre);
        break;
      }

      case '2': {
        if(recursos->comida <= 0) { 
          puts("No hay comida disponible."); 
          break; 
        }

        if(sv->hambre >= MAXHAMBRE) {
          puts("> Este superviviente ya tiene el hambre al maximo.");
          break;
        }

        sv->hambre += 1;
        recursos->comida -= 1;

        if(sv->hambre > MAXHAMBRE) {
          sv->hambre = MAXHAMBRE;
        }

        printf("> Comida administrada a '%s'.\n", sv->nombre);
        break;
      }

      case '3': {
        if(recursos->medicina <= 0) { 
          puts("No hay medicina disponible."); 
          break; 
        }

        if(sv->salud >= MAXSALUD) {
          puts("> Este superviviente ya tiene la salud al maximo.");
          break;
        }

        sv->salud += 1;
        recursos->medicina -= 1;

        if(sv->salud > MAXSALUD) {
          sv->salud = MAXSALUD;
        }

        printf("> Medicina administrada a '%s'.\n", sv->nombre);
        break;
      }
      default: {
        puts("> Opcion invalida.");
      }
    }
    presioneTeclaParaContinuar();
  } while(opcion != '4');
  return;
  return;
}

void mostrar_instrucciones() {
  limpiarPantalla();
  puts("================================================");
  puts("          PROJECT SANCTUARY - INSTRUCCIONES     ");
  puts("================================================");
  puts("");
  puts("Eres el lider de un grupo de supervivientes en");
  puts("un apocalipsis zombie. Tu objetivo es mantener");
  puts("a todos con vida durante 20 dias hasta el rescate.");
  puts("");
  puts("------------------------------------------------");
  puts("  JORNADA DIARIA");
  puts("------------------------------------------------");
  puts("Cada dia puedes realizar las siguientes acciones:");
  puts("");
  puts("1) TRASLADARSE");
  puts("   Mueve a tu grupo a una zona adyacente.");
  puts("   Cada ruta tiene un nivel de riesgo:");
  puts("   - Bajo  (1-50):  sin penalidad.");
  puts("   - Medio (51-80): -1 adicional de hambre.");
  puts("   - Alto  (81-100):-1 adicional de todo.");
  puts("   Solo puedes trasladarte una vez por dia.");
  puts("");
  puts("2) EXPEDICIONES");
  puts("   Envia hasta 5 supervivientes a buscar recursos.");
  puts("   - Mas integrantes = mayor probabilidad de exito.");
  puts("   - El riesgo de la ruta afecta el resultado.");
  puts("   - Pueden traer agua, comida, medicina o");
  puts("     un nuevo superviviente.");
  puts("   - Si fracasan, alguno puede morir.");
  puts("   - Los expedicionarios regresan en 1 a 3 dias.");
  puts("   Solo puedes iniciar una expedicion por dia.");
  puts("");
  puts("3) ADMINISTRAR SUMINISTROS");
  puts("   Distribuye recursos entre tus supervivientes.");
  puts("   - Agua    -> reduce la sed.");
  puts("   - Comida  -> reduce el hambre.");
  puts("   - Medicina-> recupera salud.");
  puts("");
  puts("4) TERMINAR JORNADA");
  puts("   Avanza al dia siguiente. Al hacerlo:");
  puts("   - Cada superviviente pierde salud, hambre y sed.");
  puts("   - Se procesan los resultados de expediciones.");
  puts("   - El riesgo de todas las rutas cambia.");
  puts("   - Si un superviviente llega a 0 en cualquier");
  puts("     necesidad, muere.");
  puts("");
  puts("------------------------------------------------");
  puts("  CONDICIONES DE FIN");
  puts("------------------------------------------------");
  puts("  VICTORIA: sobrevivir los 20 dias.");
  puts("  DERROTA:  todos los supervivientes mueren.");
  puts("================================================");
  puts("");
  return;
}

void cambiarNecesidades(Partida *slot1){
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

    if(sobreviviente->hambre == 1) {
      printf(" *'%s' se encuentra en inanicion, alimentalo o morira mañana\n", sobreviviente->nombre);
    }
    if(sobreviviente->salud == 1) {
      printf(" *'%s' se encuentra con heridas graves, curalo o morira mañana\n", sobreviviente->nombre);
    }
    if(sobreviviente->sed == 1) {
      printf(" *'%s' se encuentra deshidratado, dale de beber o morira mañana\n", sobreviviente->nombre);
    }
    if(sobreviviente->salud <= 0 || sobreviviente->hambre <= 0 || sobreviviente->sed <= 0) {
      list_pushBack(slot1->lista_muertos, strdup(sobreviviente->nombre));
    } 
    par = nextTreeMap(slot1->sobrevivientes);
  }

  char *nombre = list_first(slot1->lista_muertos);
  while(nombre != NULL) {
    if(searchTreeMap(slot1->sobrevivientes, nombre) != NULL) {
      printf("> Ha muerto '%s'.\n", nombre);
      list_pushBack(slot1->lista_muertos, nombre);
      eraseTreeMap(slot1->sobrevivientes, nombre);
    }
    nombre = list_next(slot1->lista_muertos);
  }
  return;
}

void mostrarTraslado(Partida *slot1) {
  if(slot1->se_traslado == 1) {
    printf("* Nueva ubicacion: '%s', Riesgo: ", slot1->ubicacion_actual);
    if(slot1->riesgo_actual < 51){
      printf("'Bajo'\n");
    } else if(slot1->riesgo_actual < 81) {
      printf("'Medio'\n");
    } else {
      printf("'Alto'\n");
    }
    printf("\n");
  }

  return;
}

void escribir_comando_audio(const char *comando) {
  FILE *archivo = fopen("audio/audio_command.txt", "w");

  if(archivo == NULL) {
    printf("> No se pudo conectar con el archivo de audio.\n");
    return;
  }

  fprintf(archivo, "%s", comando);
  fclose(archivo);
}

void reproducir_musica(const char *nombre_archivo) {
  char comando[100];

  sprintf(comando, "PLAY %s", nombre_archivo);
  escribir_comando_audio(comando);
}

void detener_musica() {
  escribir_comando_audio("STOP");
}



int procesar_eventos(Partida *slot1) {

  //if(rand() % 100 < 50) return 0; // 50 % de que salga o no un evento
  
  char random[3];
  sprintf(random, "%i", rand() % TOTAL_EVENTOS); // del 0 al 19 en eventos
  
  sprintf(random, "%i", 22); // probar un evento en específico
  
  HashPair *ParHash = searchMap(slot1->Hashmap_eventos, random); 
  
  int cont = 0;
  if (ParHash == NULL) return 0;
  Evento *evento = (Evento*) ParHash->value;

  if(evento->narrativo) {
    printf("\n");
    puts("-- Evento Narrativo --");
    printf("\n");
    
    Pair *ParArbol = TreePair_aleatorio(slot1->sobrevivientes);
    if(ParArbol == NULL) return 0;
    Sobreviviente *sobreviviente = ParArbol->value;
    
    char descripcion[MAXDESCRIPCION];
    sprintf(descripcion, evento->descripcion, sobreviviente->nombre);
    printf("> %s\n\n", descripcion);

    printf(" *Los eventos narratios no tienen impacto\n");

    printf("\n");
    if(evento->especial) {
      eraseMap(slot1->Hashmap_eventos, random);
    }

    return 0;
  }

  printf("\n");
  puts("-- Evento Aleatorio! --");
  printf("\n");
  
  MapPair *ParMapa = map_search(slot1->inventario_grupo, "Recursos");
  if (ParMapa == NULL) return 0;
  Recursos *recursos = (Recursos*) ParMapa->value;

  if(strcmp(random, EVENTO_MAGUITO) == 0) {
    reproducir_musica("maguito.mp3");
  } else{
    detener_musica();
  }
  
  printf("> %s\n\n", evento->descripcion);
  if(recursos->agua <= MAXAGUA) recursos->agua += evento->impacto_agua;
  if(recursos->comida <= MAXCOMIDA) recursos->comida += evento->impacto_comida;
  if(recursos->medicina <= MAXMEDICINA) recursos->medicina += evento->impacto_medicina;

  if(evento->impacto_agua > 0) {
    printf("> Has ganado '%i' de Agua\n", evento->impacto_agua);
  }
  if(evento->impacto_comida > 0) {
    printf("> Has ganado '%i' de Comida\n", evento->impacto_comida);
  }
  if(evento->impacto_medicina > 0) {
    printf("> Has ganado '%i' de Medicina\n", evento->impacto_medicina);
  }
  if(evento->impacto_agua < 0) {
    printf("> Has perdido '%i' de Agua\n", abs(evento->impacto_agua) );
  }
  if(evento->impacto_comida < 0) {
    printf("> Has perdido '%i' de Comida\n", abs(evento->impacto_comida) );
  }
  if(evento->impacto_medicina < 0) {
    printf("> Has perdido '%i' de Medicina\n", abs(evento->impacto_medicina) );
  }

  int riesgo = evento->impacto_riesgo;
  
  printf("\n");
  
  cont = evento->impacto_sobrevivientes;
  if (cont > 0) {
    Pair *ParArbol = TreePair_aleatorio(slot1->sobrevivientes);
    if (ParArbol == NULL) return 0;
    Sobreviviente *sobreviviente = ParArbol->value;
    for(int i = 1; i <= cont; i++) {
      printf("> Ha muerto '%s'\n", sobreviviente->nombre);
      eraseTreeMap(slot1->sobrevivientes, ParArbol->key);
      ParArbol = TreePair_aleatorio(slot1->sobrevivientes);
      if (ParArbol == NULL) break;
    }
  }
  
  cont = list_size(evento->lista_sobrevivientes);
  if(cont > 0) {
    char nombre[MAXNOMBRE];
    Sobreviviente *sobreviviente = list_first(evento->lista_sobrevivientes);
    for(int j = 1; j <= cont; j++) {
      if (TreeMapSize(slot1->sobrevivientes) > MAXSUPERVIVIENTES) {
        puts(" *Se ha alcanzado el maximo de supervivientes, el refugio no da a basto, lo has dejado ir");
        sobreviviente = list_next(evento->lista_sobrevivientes);
        continue;
      }
      if (strcmp(sobreviviente->nombre, "-1") == 0) {
        char nuevo_nombre[MAXNOMBRE];
        ingresar_nombre(slot1, nuevo_nombre);
        strcpy(sobreviviente->nombre, nuevo_nombre);
      }
      insertTreeMap(slot1->sobrevivientes, sobreviviente->nombre, sobreviviente);
      printf("> '%s' se ha unido al refugio!\n", sobreviviente->nombre);
      sobreviviente = list_next(evento->lista_sobrevivientes);
    }
  }

  /*
  sobreviviente = list_first(evento->lista_sobrevivientes);
  while(sobreviviente != NULL) {
    insertTreeMap(slot1->sobrevivientes, sobreviviente->nombre, sobreviviente);
    printf("> '%s' se ha unido al refugio!\n", sobreviviente->nombre);
    sobreviviente = list_next(evento->lista_sobrevivientes);
  }
  */
  printf("\n");
  if(evento->especial) {
    eraseMap(slot1->Hashmap_eventos, random);
  }
  
  return riesgo;
}

void procesar_dia(Partida *slot1) {

  int riesgo = procesar_eventos(slot1);
  
  procesar_expedicion(slot1);
  // Sistema de riesgo afecta necesidades
  cambiarNecesidades(slot1);

  // sistema de riesgo, cambia el riesgo por dia
  cambiarRiesgo(slot1, riesgo);

  mostrarTraslado(slot1);

  slot1->se_traslado = 0;
  slot1->expedicion_dia = false;
  
}

int main() {
  limpiarPantalla();
  char opcion1;
  do {
    // aca muestra el menu con las opciones varias
    puts("===========================");
    puts("     Project Sanctuary     ");
    puts("===========================");
    
    puts("-- Menu --");
    
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
          
          printf("-- Bienvenido a Proyect Sanctuary --\n");
      
          printf("* Ingresa tu nombre: ");
          scanf(" %49[^\n]", nombre);
          int c;
          while((c = getchar()) != '\n' && c != EOF); // limpia el \n del scanf
          presentacion(nombre);
          printf("\n* Presiona 'ENTER' para continuar.");
          getchar();
          
          Partida* slot1 = iniciarPartida(nombre);
          
          char opcion2;
          do {
            limpiarPantalla();
            // aca muestra el menu con las opciones varias
            puts("==================");
            printf("     Dia %i     \n", slot1->dia_actual);
            puts("==================");
            puts("-- Opciones --");
            puts("1) Trasladarse");
            puts("2) Expediciones");
            puts("3) Administrar Suministros");
            puts("4) -- Terminar Jornada --");
            printf("\n");
            puts("-- Menu --");
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
                puts("* Estas seguro? (s/n): ");

                char sn;
                scanf(" %c", &sn);
                if(sn == 's' || sn == 'S') {
                  slot1->dia_actual++;
                  puts("> Dia finalizado");
                  puts("> Avanzando al siguiente día...");
                  procesar_dia(slot1);
                  
                } else {
                  puts("> Operacion cancelada.");
                  
                }
                
                break;
              }
              case '5':
                return 0;
              default: {
                puts("> Operacion invalida.");
                
              }
            }
            // Evitamos pausar y limpiar pantalla si el usuario eligió salir
            if (opcion2 != '5') {
              presioneTeclaParaContinuar();
            }

          } while (slot1->dia_actual <= 20 && firstTreeMap(slot1->sobrevivientes) != NULL);
          
          if(firstTreeMap(slot1->sobrevivientes) != NULL) {
            puts("> Has Ganado!!");
          } else {
            puts("> Has Perdido!!");
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
