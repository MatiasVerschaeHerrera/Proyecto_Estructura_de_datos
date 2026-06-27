// bibliotecas importadas 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// archivos desde la carpeta tdas 
#include "tdas/extra.h"
#include "tdas/heap.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/multimap.h"
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
#define TOTAL_EVENTOS 27

//función que verifica si 2 string son identicos
int son_iguales_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

//función que verifica si un string es menor a al otro.
int son_menores_strings(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) < 0;
}

// Al procesar el día, se debe actualizar el riesgo que tiene cada ubicación en el mapa, esta función hace exactamente eso.
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


// Esta función crea el mapa y todos los nodos (ubicaciones). Además, hace la conexión entre la ubicaciones (ya que no todas están conectadas las unas a las otras) y finalmente le agrega un riesgo inicial a cada una de estas.
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

  // Distrito Comercial
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


 // Esta función recibe el nombre que elige el jugador para identificarse, la función verifica que el nombre no esté repetido entre los nombres de los actuales sobrevivientes y que este no sea un conjunto de numeros o un espacio en blanco.
void ingresar_nombre(Partida *slot1, char nombre[]) {
  
  puts("-- Ingresar Nombre del Nuevo Superviviente --");
  
  while(true) {
    bool valido = true;
    bool existe = false;
    
    scanf(" %49[^\n]", nombre);

    if(strlen(nombre) == 0) {
      puts("> Nombre invalido, debe tener como minimo un caracter.");
      continue;
    }
    
    if(strcmp(nombre, "0") == 0) {
      puts("> No se permite ese nombre.");
      continue;
    }
    
    for(int i = 0; nombre[i] != '\0'; i++) {
      if(!isalpha(nombre[i]) && nombre[i] != ' ') {
        puts("> Nombre invalido, debe tener solo letras.");
        valido = false;
        break;
      }
    }
    
    formato_nombre(nombre);

    if(!valido) continue;
    
    if(strcmp(nombre, "Maguito Explosivo") == 0) {
      puts("> No se permite este nombre, quien sabe si está por allí afuera esperando.");
      continue;
    }

    Pair *par = searchTreeMap(slot1->sobrevivientes, nombre);
    if(par != NULL) {
      puts("> Ya existe un superviviente con ese nombre, no queremos confundirlos asi que elige otro.");
      continue;
    }
    
    Expedicion *expedicion = list_first(slot1->lista_expediciones);
    
    while(expedicion != NULL && !existe) {
      
      Sobreviviente *sobreviviente = list_first(expedicion->sobrevivientes);

      while(sobreviviente != NULL) {
        if(strcmp(sobreviviente->nombre, nombre) == 0) {
          existe = true;
          break;
        }
        sobreviviente = list_next(expedicion->sobrevivientes);
        
      }
    
      expedicion = list_next(slot1->lista_expediciones);
    }
    
    if(existe) {
      puts("> Ese nombre ya pertenece a alguien que esta en expedicion.");
      continue;
    }

    break;
  }

  return;
}

// Esta función crea a los sobrevivientes principales (se hace uso de la estructura arbol) 
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

// Esta función crea la partida e inicializa todos los datos necesarios (día actual, el nombre del juagdor, la generación del mapa, la ubicación actual, los suministros base, etc).
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
  aux->Hashmap_eventos = createMap(TOTAL_EVENTOS * 2);
  
  crear_eventos(aux->Hashmap_eventos);
  
  Recursos *suministros = malloc(sizeof(Recursos));
  suministros->agua = 8;
  suministros->comida = 5;
  suministros->medicina = 3;
  
  map_insert(aux->inventario_grupo, "Recursos", suministros);

  return aux;
}

// Esta sección es el menu principal para gestionar un translado, esta función inicialmente muestra los tipos de riegos que existen y la penalidad que tiene cada uno
// Esta función imprime por pantalla la ubicación actual del grupo y crea una lista de aristas (ubicaciones las cuales se puede mover el jugador)
// La función deja al jugador elegir a donde transladars y el riesgo que esto implica. Finalmente, el jugador puede confiermar si quiere realizar el translado o no. El translado se hace efectivo al pasar al siguiente día.
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

// Esta función se utiliza cuando el usuario desea administrar los suministros. Esta imprime en pantalla la salud, hambre y sed actual de todos los sobrevivientes.
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

// Esta sección muestra en pantalla los sobrevivientes que se encuentran disponibles para hacer una expedición. La función le pide al usuario que ingrese los/el nombre del sobreviviente que quiere enviar a la expedición y lo agrega a un arreglo. Finalmente retorna el arreglo con los sobrevivientes que irán a la expedición.
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


// Esta sección muestra en pantalla las ubicaciones disponibles para hacer una expedición y el riesgo que conlleva ir a estos lugares, esta función le pide al jugador ingresar un caracter numerico para elegir la ubicación en donde se quiere realizar la expedición y verifica que sea valido.
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

// Esta sección calcula la probabilidad de éxito de una expedición, esto lo hace a partir de la cantidad de sobrevivientes que irán, los suministros que llevan consigo y el riesgo que  significa hacer la expedición.
//Si el jugador lleva demasiados sobrevivientes y suministros a la expedición, hay una condición que resta la probabilidad de éxito, balanceando un poco el programa. Retorna la probabilidad de éxito que tendrá la expedición.
int calcular_exito(int numselec, int riesgo, int agua, int comida, int medicina) {

  double bonoRecursos = agua * 1.2 + comida * 1 + medicina * 1.5;
  
  double bonoEquipo = numselec * 12;
  
  double bonoRiesgo = riesgo * 0.6;

  double bonos = bonoRecursos + bonoEquipo - bonoRiesgo;
  
  double probabilidad_exito = 100.0 * (1.0 - exp(-bonos/40.0)); // aca usa la funcion de ley de rendimientos decrecientes para que asignar suministros, sobrevivientes y riesgo bajo al inicio sume mucho, pero mientras más coloques menos sume

  if(probabilidad_exito < 10){
    probabilidad_exito = 10;
  } else if(probabilidad_exito > 80) {
    probabilidad_exito = 80;
  }

  //return 100;
  return probabilidad_exito;
}

// En caso de confirmar la expedición, esta función se hace cargo de crear la expedición con todas los datos previamente otorgados (cant de sobrevivientes que irán, cant de suministros asignados, etc) y finalmente imprime en pantalla cuanto tardará la expedición (numero aleatorio)
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


// Esta sección le pregunta al usuario que suministros quiere llevar para las expediciones, le pide inicialmente que recurso desea llevar y posteriormente la cantidad deseada. 
//los suministros que le asigna se le resta a los recursos totales del grupo (inventario_grupo) 
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
          
          while(*agua < 0 || *agua > 5){
            puts("> no se pueden llevar más de 5 unidades por suministro");
            printf("Valor inválido, ingrese nuevamente");
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
          while(*comida < 0 || *comida > 5){
            puts("> no se pueden llevar más de 5 unidades por suministro");
            printf("Valor imválido, ingrese nuevamente");
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
          while(*medicina < 0 || *medicina > 5){
            puts("> no se pueden llevar más de 5 unidades por suministro");
            printf("Valor inválido, ingrese nuevamente");
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


// Esta sección es el menu principal para realizar las expediciones, en esta se inicializan los valores (prob éxito, los recursos, cant máx de sobrevivientes)
//En este menu se le pide al jugador elegir a quien enviar a la expedición, el lugar a donde ira y si quiere o no enviar al sobreviviente con suministros.
//Luego de realizar los cambios, esta función imprime por pantalla todas las decisiones que tomó el jugador y la probabilidad de éxito de la expedición.
// Finalmente, el jugador puede confirmar o no la expedición. 
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


// Selecciona y retorna un elemento (pair) aleatorio del mapa. La funcion calcula el tamaño total del arbol para generar un indice aleatorio valido. Luego, utilizando las funciones de recorrido del TDA, avanza linealmente desde el primer nodo, hasta alcanzar el Pair generado.
Pair *TreePair_aleatorio(TreeMap *mapa) {
  int total = TreeMapSize(mapa);

  int i = rand() % total;
  Pair *par = firstTreeMap(mapa);
  for(int j = 0; j < i; j++) {
    par = nextTreeMap(mapa);
  }
  return par;
}

// Esta función es la que verifica que se hayan cumplido los días requeridos para la culminación de la expedición.
// luego de verificar que los plazos se hayan completado, la función verifica que la expedición haya sido un éxito o no. Si la expedición logró ser un éxito, la función recompensa al usuario con un botín, el cual puede ser suministros o la obtención de un nuevo sobreviviente.
// Si la expedición fue un fracaso, el jugador pierde al sobreviviente que fue enviado a la expedición (RIP).
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
          if(suministros->agua <= MAXAGUA) {
            int ganadoA = 1 + rand() % 4;
            printf("> Has ganado '%i' de Agua\n", ganadoA);
            suministros->agua += ganadoA;
          } else if(suministros->comida <= MAXCOMIDA) {
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


//La siguiente función es un menu en donde se van a poder administrar los suministros entre los sobrevivientes para asegurar su bienestar.
//Para empezar, la función muestra las necesidades de todos los sobrevivientes y la cantidad de suministros que existen actualmente. Si el jugador desea dar alguno de los sumistros a algún sobreviviente este primero debe marcar que suministro desea entregar y el nombre del sobreviviente al cual va dirigido tal suministro.
//La función también se encarga que: no se puedan dar suministros que no existan dentro del inventario, que se le den al sobreviviente correcto y que no se pueda dar más de lo establecido.
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

//Esta función imprime por pantalla todas las instrucciones del juego.
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

//Esta función crea una desventaja en base al riesgo de la ubicación donde se encuentra actualmente el grupo de sobrevivientes, esta desventaja se aplica en la salud, hambre y sed de cada sobreviviente del grupo.
//Asimismo, la función informa al jugador si algún sobreviviente se encuentra en baja salud, hambre o sed.
//Finalmente la función también se encarga de avisar si algún sobreviviente ha muerto.
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
      eraseTreeMap(slot1->sobrevivientes, nombre);
    }
    nombre = list_next(slot1->lista_muertos);
  }
  return;
}


//Tras hacer un translado, esta función se encarga de mostrar la ubicación y el riesgo del nuevo lugar.
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

/*
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
*/


//Función que procesa los eventos aleatorios 
//Calcula la probabilidad de que salga un evento
// define si el evento es de tipo narrativo o aleatorio (los de tipo narrativo no tienen un impacto en el grupo)
// Si el evento es de tipo aleatorio, la función analiza si el evento tiene un impacto positivo o negativo frente al grupo y lo imprime por pantalla. 
//La función también informa si ha muerto o ganado un sobreviviente.
// Si se ha ganado un sobreviviente y la cantidad sobrevivientes alcanzó su máximo, la función se encarga de no incluirlo a la lista de sobrevivientes.
int procesar_eventos(Partida *slot1) {

  if(rand() % 100 < 50) return 0; // 50 % de que salga o no un evento
  
  char random[3];
  sprintf(random, "%i", rand() % TOTAL_EVENTOS); //
  
  //sprintf(random, "%i", 1); // probar un evento en específico
  
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
    printf(" *%s\n\n", descripcion);
    
    if(evento->rescate) return -1;
    
    printf(" *Los eventos narrativos no tienen impacto\n");
    
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

  /*
  if(strcmp(random, EVENTO_MAGUITO) == 0) {
    reproducir_musica("maguito.mp3");
  } else{
    detener_musica();
  }
  */
  
  printf(" *%s\n\n", evento->descripcion);

  if(evento->impacto_agua > 0) {
    if((recursos->agua + evento->impacto_agua) <= MAXAGUA) {
      recursos->agua += evento->impacto_agua;
    } else {
      recursos->agua = MAXAGUA;
    }
    printf("> Has ganado '%i' de Agua\n", evento->impacto_agua);
  }
  if(evento->impacto_comida > 0) {
    if((recursos->comida + evento->impacto_comida) <= MAXCOMIDA) {
      recursos->comida += evento->impacto_comida;
    } else {
      recursos->comida = MAXCOMIDA;
    }
    printf("> Has ganado '%i' de Comida\n", evento->impacto_comida);
  }
  if(evento->impacto_medicina > 0) {
    if((recursos->medicina + evento->impacto_medicina) <= MAXMEDICINA) {
      recursos->medicina += evento->impacto_medicina;
    } else {
      recursos->medicina = MAXMEDICINA;
    }
    printf("> Has ganado '%i' de Medicina\n", evento->impacto_medicina);
  }
  if(evento->impacto_agua < 0) {
    if((recursos->agua + evento->impacto_agua) >= 0) {
      recursos->agua += evento->impacto_agua;
    } else {
      recursos->agua = 0;
    }
    printf("> Has perdido '%i' de Agua\n", abs(evento->impacto_agua) );
  }
  if(evento->impacto_comida < 0) {
    if((recursos->comida + evento->impacto_comida) >= 0) {
      recursos->comida += evento->impacto_comida;
    } else {
      recursos->comida = 0;
    }
    printf("> Has perdido '%i' de Comida\n", abs(evento->impacto_comida) );
  }
  if(evento->impacto_medicina < 0) {
    if((recursos->medicina + evento->impacto_medicina) >= 0) {
      recursos->medicina += evento->impacto_medicina;
    } else {
      recursos->medicina = 0;
    }
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
      eraseTreeMap(slot1->sobrevivientes, ParArbol->key);
      printf("> Ha muerto '%s'\n", sobreviviente->nombre);
      ParArbol = TreePair_aleatorio(slot1->sobrevivientes);
      if (ParArbol == NULL) break;
      sobreviviente = ParArbol->value;
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


//función que se encarga de procesar el nuevo día, esta realiza los cambios en las necesidades de lo sobrevivientes, el cambio de riesgo de la ubicación, procesar la aparición de un evento y si se realizó o no un translado (muestra la nueva ubicación del grupo)
bool procesar_dia(Partida *slot1) {

  int riesgo = procesar_eventos(slot1);

  if(riesgo == -1) return true;
    
  procesar_expedicion(slot1);
  // Sistema de riesgo afecta necesidades
  cambiarNecesidades(slot1);

  // sistema de riesgo, cambia el riesgo por dia
  cambiarRiesgo(slot1, riesgo);

  mostrarTraslado(slot1);

  slot1->se_traslado = 0;
  slot1->expedicion_dia = false;

  return false;
}

//Menu principal, aquí es donde el jugador podrá iniciar una nueva partida y realizar la gran mayoría de acciones (gestionar translados, administrar recursos, terminar jornada , etc) 
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
          bool rescate = false;
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
                  rescate = procesar_dia(slot1);
                  
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

          } while (slot1->dia_actual <= 20 && firstTreeMap(slot1->sobrevivientes) != NULL && rescate == false);
          
          if(firstTreeMap(slot1->sobrevivientes) != NULL) {
            puts("====================");
            puts("     HAS GANADO     ");
            puts("====================");
            
            ganado();
            
          } else {
            puts("=====================");
            puts("     HAS PERDIDO     ");
            puts("=====================");
            
            perdido();
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

