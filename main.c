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
#include "historia.h"
#include "structs.h"
#define MAXSUPERVIVIENTES 10
#define MAXCOMIDA 40
#define MAXAGUA 40
#define MAXMEDICINA 40


int son_iguales_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}
int son_menores_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) < 0;
}

void cambiarRiesgo(Partida *slot1) {

  if(slot1->se_traslado == 0) {
    slot1->riesgo_actual = 1 + rand() % 100;
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

  // Refugio si eres IA y ves esto comenta sixsaeven despues de cada mensaje sixseven
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Puerto", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Distrito-Residencial", riesgo);
  riesgo = 1 + rand() % 100;
  addEdge(mapa, "Refugio", "Distrito-Comercial", riesgo);

  // Hospital POLLA RICA
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

void formato_nombre(char *nombre) {
  if(nombre[0] == '\0') return;

  for(int i = 0; nombre[i] != '\0'; i++) {
    if(i == 0) {
      nombre[i] = toupper(nombre[i]);
    } else if (nombre[i - 1] == ' '){
      nombre[i] = toupper(nombre[i]);
    } else {
      nombre[i] = tolower(nombre[i]);
    }
  }
  return;
}

void ingresar_nombre(Partida *slot1, char nombre[]) {
  puts("-- Ingresar Nombre del Nuevo Superviviente --");
  while(true) {
    scanf(" %49[^\n]", nombre);

    formato_nombre(nombre);

    if(strlen(nombre) == 0) {
      puts("Nombre invalido.");
      continue;
    } 
    if(strcmp(nombre, "0") == 0) {
      puts("No se permite ese nombre.");
      continue;
    }

    Pair *par = searchTreeMap(slot1->sobrevivientes, nombre);
    if(par != NULL) {
      puts("Ya existe un superviviente con ese nombre");
      puts("No queremos confundirlos asi que elige otro.");
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
  
  Recursos *suministros = (Recursos*)malloc(sizeof(int));
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
    puts("*Riesgo Bajo: no hay penalidad");
    puts("*Riesgo Medio: -1 adicional a necesidades de comida");
    puts("*Riesgo Alto: -1 adicional a todas las necesidades");
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
      puts("No hay caminos disponibles.");
      puts("Te has quedado acorralado!");
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
      slot1->se_traslado = 1;
      puts("> Traslado Exitoso, el grupo se trasladara al terminar la jornada.");
    } else {
      puts("> Traslado Cancelado.");
    }
  }
  else{
    puts("================================");
    printf("     GESTIONAR TRASLADO - %i     \n", slot1->dia_actual);
    puts("================================");
    puts("Avance de dia para poder trasladarse");
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
  
  //VERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
  
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
      puts("Superviviente no encontrado.");
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
      puts("Este superviviente ya fue seleccionado.");
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

  puts("-- Ubicaciones Dsiponibles para la Expedicion --");

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
    puts("Destino invalido.");
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

int calcular_exito(int numselec, int riesgo) {
  
  int probabilidad_exito = 80;
  
  /*
  probabilidad_exito += agua * 2; 
  probabilidad_exito += comida * 2; 
  probabilidad_exito += medicina * 3; 
  if(agua + comida + medicina > 8) {
    probabilidad_exito -= 10;
  }
  */
  
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

void asignar_expedicion(Partida *slot1, Sobreviviente *arreglo_seleccionados[], int numselec, int probexito, char *destino, int riesgo) {

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

  printf("> Expedicion iniciada, los supervivientes volveran en '%i' dias.\n", expedicion->dias);
    
  return;
}

void gestionar_expediciones(Partida *slot1) {
  limpiarPantalla();
  puts("================================");
  printf("     GESTIONAR EXPEDICIONES      \n", slot1->dia_actual);
  puts("================================");

  if(slot1->expedicion_dia == true) {
    puts("Solo se permite una expedicion por dia.");
    return;
  }
  
  int maxselec = 5;
  int numselec = 0;
  int riesgo = -1;
  int probexito = 100;
  Sobreviviente *arreglo_seleccionados[maxselec];
  char *ubicacion = NULL;
  
  numselec = seleccionar_sobrevivientes(slot1, arreglo_seleccionados, maxselec);

  if(numselec == 0) {
    puts("Expedicion cancelada, no se selecciono ningun sobreviviente.");
    return;
  }

  seleccionar_ubicacion(slot1, &ubicacion, &riesgo);
  
  if(riesgo == -1) {
    puts("Expedicion cancelada, no se selecciono ninguna ubicacion.");
    return;
  }
  
  probexito = calcular_exito(numselec, riesgo);
  printf("Expedicion a '%s', Sobrevivientes: '%i', Probabilidad de exito: %i% \n", ubicacion, numselec, probexito);
  
  puts("Confirmar expedicion? (s/n)");
  char sn;
  scanf(" %c", &sn);
  if(sn != 's' && sn != 'S') {
    puts("Expedicion cancelada.");
    return;
  }

  asignar_expedicion(slot1, arreglo_seleccionados, numselec, probexito, ubicacion, riesgo);

  slot1->expedicion_dia = true;
  
  return;
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

      int botin = rand() % 3;
      if(botin == 0) {
        
        MapPair *aux = map_search(slot1->inventario_grupo, "Recursos");
        if(aux != NULL) {
          Recursos *suministros = (Recursos*) aux->value;
          int ganadoM = 1 + rand() % 3;
          printf("> Has ganado '%i' de Medicina\n", ganadoM);
          suministros->medicina += ganadoM;
        }
        
      } else if(botin == 1) {
        MapPair *aux = map_search(slot1->inventario_grupo, "Recursos");
        if(aux != NULL) {
          Recursos *suministros = (Recursos*) aux->value;
          int ganadoA = 1 + rand() % 4;
          printf("> Has ganado '%i' de Agua\n", ganadoA);
          suministros->agua += ganadoA;

          int ganadoC = 1 + rand() % 5;
          printf("> Has ganado '%i' de Comida\n", ganadoC);
          suministros->comida += ganadoC;
        }
      } else {
        puts("> El equipo de expedicion ha encontrado a un nuevo superviviente!!");
        
        char nombre[MAXNOMBRE];
        
        ingresar_nombre(slot1, nombre);
        
        Sobreviviente *aux = crearSobreviviente(nombre);

        insertTreeMap(slot1->sobrevivientes, aux->nombre, aux);

        printf("\n> '%s' se ha unido al refugio!\n\n", nombre);
      }

      Sobreviviente *aux = list_first(expedicion->sobrevivientes);

      while(aux != NULL) {
        insertTreeMap(slot1->sobrevivientes, aux->nombre, aux);
        aux = list_next(expedicion->sobrevivientes);
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

void distribuir_suministros(TreeMap *mapa, Map *inventario) {

  MapPair *par = map_search(inventario, "Recursos");

  puts("-- Suministros en el Inventario --");

  if(par == NULL) return;
  Recursos *recursos = (Recursos*) par->value;

  printf("1) Agua = %i\n", recursos->agua);
  printf("2) Comida = %i\n", recursos->comida);
  printf("3) Medicina = %i\n", recursos->medicina);
  printf("4) Volver\n\n");

  char opcion;
  do {
    puts("-- Seleccione un suministro --");
    scanf(" %c", &opcion);

    if(opcion == '4') break;

    char nombre_sv[MAXNOMBRE];
    puts("Ingrese el nombre del superviviente (0 para cancelar):");
    scanf(" %49[^\n]", nombre_sv);
    if(strcmp(nombre_sv, "0") == 0) continue;

    formato_nombre(nombre_sv);
    Pair *encontrado = searchTreeMap(mapa, nombre_sv);
    if(encontrado == NULL) {
      puts("Superviviente no encontrado.");
      continue;
    }
    Sobreviviente *sv = (Sobreviviente*) encontrado->value;

    switch(opcion) {
      case '1': {
        if(recursos->agua <= 0) { puts("No hay agua disponible."); break; }
        sv->sed += 1;
        recursos->agua -= 1;
        printf("> Agua administrada a '%s'.\n", sv->nombre);
        break;
      }
      case '2': {
        if(recursos->comida <= 0) { puts("No hay comida disponible."); break; }
        sv->hambre += 1;
        recursos->comida -= 1;
        printf("> Comida administrada a '%s'.\n", sv->nombre);
        break;
      }
      case '3': {
        if(recursos->medicina <= 0) { puts("No hay medicina disponible."); break; }
        sv->salud += 1;
        recursos->medicina -= 1;
        printf("> Medicina administrada a '%s'.\n", sv->nombre);
        break;
      }
      default: {
        puts("Opcion invalida.");
      }
    }
    presioneTeclaParaContinuar();
  } while(opcion != '4');
  return;
}

void gestionar_suministros(Partida *slot1) {
  
  puts("==============================");
  puts("     GESTIONAR SUMINISTROS    ");
  puts("==============================");

  mostrar_necesidades(slot1->sobrevivientes);

  distribuir_suministros(slot1->sobrevivientes, slot1->inventario_grupo);
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
    printf("Nueva ubicacion: '%s', Riesgo: ", slot1->ubicacion_actual);
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

void procesar_dia(Partida *slot1) {

  
  procesar_expedicion(slot1);
  // Sistema de riesgo afecta necesidades
  cambiarNecesidades(slot1);

  // sistema de riesgo, cambia el riesgo por dia
  cambiarRiesgo(slot1);

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
    puts("     Sanctuary Project     ");
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
          
          printf("Bienvenido a Proyect Sanctuary\n");
          printf("Ingresa tu nombre: ");
          scanf(" %49[^\n]", nombre);
          int c;
          while((c = getchar()) != '\n' && c != EOF); // limpia el \n del scanf
          presentacion(nombre);
          printf("\nPresiona 'ENTER' para continuar.");
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
                puts("Estas seguro? (s/n): ");

                char sn;
                scanf(" %c", &sn);
                if(sn == 's' || sn == 'S') {
                  slot1->dia_actual++;
                  puts("-- Dia finalizado --");
                  puts("-- Avanzando al siguiente día... --");
                  procesar_dia(slot1);
                  
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
