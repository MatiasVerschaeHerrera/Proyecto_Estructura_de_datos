#ifndef EVENTOS_H
#define EVENTOS_H
#define MAXDESCRIPCION 300
#include "structs.h"
#include "tdas/list.h"
#include "tdas/hashmap.h"
#include <stdbool.h>

typedef struct{
  char descripcion[MAXDESCRIPCION];
  int impacto_agua;
  int impacto_comida;
  int impacto_medicina;
  int impacto_sobrevivientes;
  List *lista_sobrevivientes;
  int impacto_riesgo;
  bool especial;
  bool narrativo;
  bool rescate;
} Evento;

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

void crear_eventos(HashMap *Hashmap_eventos) {

  //Evento 1
  //Bloque eventos 
  Evento *aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Desde las cenizas de una antigua civilizacion, ha renacido una leyenda, Maguito Explosivo, muchos dicen que su poder sigue oculto, otros creen que es solo un gato, lo unico que es seguro, es que ha traido galletas de pescado.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 5;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = false;
  Sobreviviente *sobreviviente = crearSobreviviente("Maguito Explosivo");
  list_pushBack(aux->lista_sobrevivientes, sobreviviente);
  insertMap(Hashmap_eventos, "1" , aux); // CAMBIAR NUMERO PARA CADA EVENTO
  //Fin bloque eventos

  //Evento 2
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un grupo de perros salvajes ha saqueado el refugio!");
  aux->impacto_agua = -3;
  aux->impacto_comida = -5;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "2" , aux);

  //Evento 3
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Te has topado con un sobreviviente que se encuentra en peligro y decides ayudarlo, como agradecimiento el sobreviviente te regala comida.");
  aux->impacto_agua = -1;
  aux->impacto_comida = 2;
  aux->impacto_medicina = -1;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "3" , aux);

  //Evento 4
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un fuerte ruido los ha despertado, el gobierno ha lanzado cajas de suministros a los refugios de la region.");
  aux->impacto_agua = 10;
  aux->impacto_comida = 15;
  aux->impacto_medicina = 5;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "4" , aux);

  //Evento 5
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "La comida que cenaron ayer estaba caducada, varios en tu grupo de sobrevivientes se sienten enfermos.");
  aux->impacto_agua = -5;
  aux->impacto_comida = 0;
  aux->impacto_medicina = -3;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "5" , aux);

  //Evento 6
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Uno de tus sobrevivientes era un rebelde infiltrado.");
  aux->impacto_agua = -1;
  aux->impacto_comida = -1;
  aux->impacto_medicina = -1;
  aux->impacto_sobrevivientes = 2;
  aux->impacto_riesgo = 100;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "6" , aux);

  //Evento 7
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Ha ocurrido una lluvia inesperada.");
  aux->impacto_agua = 3;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "7" , aux);

  //Evento 8
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un fuerte ruido los ha despertado, un terremoto azota la zona, debido al pánico los sobrevivientes corren en diferentes direcciones. Después de un tiempo se logran reencontrar, sin embargo falta un sobreviviente.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 1;
  aux->impacto_riesgo = 50;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "8" , aux);

  //Evento 9
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un comerciante misterioso con alias 'Papu Burguers' te ha ofrecido comida a cambio de medicina");
  aux->impacto_agua = 0;
  aux->impacto_comida = 4;
  aux->impacto_medicina = -1;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "9" , aux);

  //Evento 10
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un fuerte ruido los ha despertado, alguien no apagó la alarma del celular... Una horda de zombies ataca tu refugio, los sobrevivientes logran escapar, sin embargo, uno resulta herido.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = -2;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 100;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "10" , aux);

  //Evento 11
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Han encontrado una despensa llena de suministros.");
  aux->impacto_agua = 2;
  aux->impacto_comida = 4;
  aux->impacto_medicina = 1;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "11" , aux);

  //Evento 12
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Alguien ha dejó la llave abierta toda la noche.");
  aux->impacto_agua = -4;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "12" , aux);

  //Evento 13
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Encontraste una bolsa de ramen picante!");
  aux->impacto_agua = -3;
  aux->impacto_comida = 2;
  aux->impacto_medicina = -2;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "13" , aux);

  //Evento 14
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Han encontrado y saqueado una farmacia, no habia mucho pero sirve.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 2;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "14" , aux);

  //Evento 15
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Caza exitosa por los alrededores, aunque nadie sabe lo que realmente era.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 5;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "15" , aux);

  //Evento 16
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Una lluvia torrencial ha caido por la noche, ha llenado los depositos pero estropeado parte de la comida.");
  aux->impacto_agua = 5;
  aux->impacto_comida = -3;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "16" , aux);

  //Evento 17
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un superviviente se ha embarcado en una expedicion solo, vuelve con suministros, pero algo herido.");
  aux->impacto_agua = 4;
  aux->impacto_comida = 2;
  aux->impacto_medicina = -2;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "17" , aux);

  //Evento 18
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Una camioneta blindada ha llegado al refugio, han sido dos supervivientes que escucharon del Proyecto Sanctuary, se unen al refugio.");
  aux->impacto_agua = 4;
  aux->impacto_comida = 5;
  aux->impacto_medicina = 2;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = false;
  
  sobreviviente = crearSobreviviente("-1");
  list_pushBack(aux->lista_sobrevivientes, sobreviviente);
  
  sobreviviente = crearSobreviviente("-1");
  list_pushBack(aux->lista_sobrevivientes, sobreviviente);
  insertMap(Hashmap_eventos, "18" , aux);

  //Evento 19
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Ratas! tu refugio se encontraba plagado de ratas, estas se comen parte de tus raciones.");
  aux->impacto_agua = 0;
  aux->impacto_comida = -5;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "19" , aux);

  // Evento 20
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "El cierre de tu mochila se encontraba abierto, pero era muy tarde cuando te diste cuenta.");
  aux->impacto_agua = -2;
  aux->impacto_comida = -4;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "20" , aux);

  //Evento 21
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Uno de tus sobrevivientes estaba infectado.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 1;
  aux->impacto_riesgo = 70;
  aux->lista_sobrevivientes = list_create();
  aux->especial = false;
  aux->narrativo = false;
  insertMap(Hashmap_eventos, "21" , aux);

  //Eventos Narrativos, chicos estos los puse para que el jugador reflexiones y se de cmo cuenta de que las vidas de la gente que está gestionando importan

  //Evento 22
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Hoy fue el cumpleaños de %s, nadie lo menciona en voz alta, no hay celebraciones, ni regalos, pero durante el desayuno alguien deja una especie de galleta frente a su plato. Ese momento de silencio quiza vale mas que cualquier fiesta que haya tenido antes del colapso.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = true;
  aux->rescate = false;
  insertMap(Hashmap_eventos, "22" , aux);

  //Evento 23
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Anoche %s pidio salir unos minutos del refugio. No para buscar refugios ni vigilar, solo queria ver el cielo. dijo que es dificil recordar como era la ciudad cuando todavia habian luces por las calles.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = true;
  aux->rescate = false;
  insertMap(Hashmap_eventos, "23" , aux);

  //Evento 24
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "A las 4 de la mañana, la radio capto una señal. Era musica, alguien en algun lugar seguia trasmitiendo canciones como si el mundo nunca hubiese terminado. Durante unos minutos nadie durmio, todos escuchaban, como si algo hubiese cambiado dentro de todos.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = true;
  aux->rescate = false;
  insertMap(Hashmap_eventos, "24" , aux);

  //Evento 25
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Revisando el mapa %s te pregunta sin vacilar, ¿Crees que el rescate es real? No tienes una respuesta honesta que darle, le dices que si, el sabe que mientes, pero lo necesita igual. A veces liderar es cargar con las dudas de todos para que los demas puedan dormir.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = true;
  aux->rescate = false;
  insertMap(Hashmap_eventos, "25" , aux);

  //Evento 26
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Durante una exploracion %s reconoce una calle, su casa estaba a cinco cuadras. Lo ves detenerse y mirar por unos segundos al horizonte, siguen caminando sin preguntar. Algunos sentimientos no necesitan explicacion para ser entendidos, el grupo sigue adelante en silencio.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = true;
  aux->rescate = false;
  insertMap(Hashmap_eventos, "26" , aux);

  //Evento 27
  aux = (Evento *) malloc(sizeof(Evento));
  strcpy(aux->descripcion, "Un fuerte ruido los ha despertado, han enviado a %s a investigar.");
  aux->impacto_agua = 0;
  aux->impacto_comida = 0;
  aux->impacto_medicina = 0;
  aux->impacto_sobrevivientes = 0;
  aux->impacto_riesgo = 0;
  aux->lista_sobrevivientes = list_create();
  aux->especial = true;
  aux->narrativo = true;
  aux->rescate = true;
  insertMap(Hashmap_eventos, "27" , aux);
  
  
  return;
}

#endif