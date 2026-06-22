#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tdas/extra.h"

void escribirLento(const char *texto) {
    while (*texto) {
        putchar(*texto);
        fflush(stdout);
        usleep(22000); // 22 ms
        texto++;
    }
}

void presentacion(char* nombre){
    escribirLento("Año 2038.\n\n");
    escribirLento("Lo que comenzó como una extraña enfermedad en una pequeña ciudad se convirtió en el colapso total de la civilización. En cuestión de meses, gobiernos enteros desaparecieron, las comunicaciones se extinguieron y las calles quedaron dominadas por hordas de infectados.");

    escribirLento("\n\nSin embargo, antes de caer, los últimos gobiernos del mundo lanzaron un proyecto de emergencia conocido como Sanctuary. Su objetivo era simple: establecer una red de refugios seguros conectados entre sí y resistir hasta la llegada de una operación internacional de rescate.\n\n");

    escribirLento("Han pasado años desde entonces.\n\n");

    escribirLento("La mayoría de los refugios han sido abandonados, destruidos o tomados por los infectados. Los caminos que alguna vez fueron seguros ahora están llenos de peligros desconocidos. Los recursos son escasos, la medicina casi inexistente y cada día aparecen nuevas amenazas.\n\n");

    escribirLento("Tú eres el líder de uno de los últimos grupos de supervivientes que aún conserva la esperanza.\n\n");

    escribirLento("Una transmisión de radio recibida hace pocos días cambió todo:\n\n");

    escribirLento("============================================================================\n");
    escribirLento("'Si alguien sigue con vida... manténganse firmes. Una flota de \nevacuación llegará en unos meses. Resistan. No abandonen la red Sanctuary.'\n");
    escribirLento("============================================================================\n\n");

    escribirLento("Ahora tu misión es clara: mantener con vida a tu grupo hasta que llegue el rescate.\n\n");

    escribirLento("Cada decisión tendrá consecuencias.\n\n");

    escribirLento("¿Enviarás a tus mejores exploradores a buscar suministros sabiendo que podrían no regresar?\n\n");

    escribirLento("¿Compartirás la última dosis de medicina con un herido grave o la guardarás para una emergencia mayor?\n\n");

    escribirLento("¿Cruzarás una ruta infestada para alcanzar un refugio mejor equipado?\n\n");

    escribirLento("Mientras luchas contra el hambre, la sed y el desgaste emocional de los supervivientes, descubrirás que los infectados no son el único peligro. Tormentas, saqueadores, derrumbes y eventos inesperados pondrán a prueba tu liderazgo.\n\n");

    escribirLento("Algunos miembros de tu grupo se convertirán en héroes.\n\n");

    escribirLento("Otros no llegarán al amanecer.\n\n");

    escribirLento("Y cuando finalmente aparezcan las luces del rescate en el horizonte, solo una pregunta importará:\n\n");

    escribirLento("¿Cuántos de ustedes seguirán vivos para verlo? 🚁🧟‍♂️🏚️\n\n");

    return;
}