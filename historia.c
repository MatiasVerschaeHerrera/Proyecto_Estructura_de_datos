#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "tdas/extra.h"

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // Desactiva la espera del Enter (ICANON) y ocultamos lo que tecleas (ECHO)
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Restauramos la consola a la normalidad
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1; // Se presionó una tecla
    }
    return 0;     // No se presionó nada
}

// 2. Función para leer esa tecla y sacarla del buffer
int getch(void) {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// 3. Tu función original, ahora con el botón de "skip" integrado
void escribirLento(const char *texto) {
    int saltar = 0; // Bandera: 0 = lento, 1 = instantáneo

    while (*texto) {
        putchar(*texto);
        fflush(stdout);

        if (!saltar) {
            // Si detecta una tecla, activamos el salto
            if (kbhit()) {
                getch();    // Limpiamos la tecla presionada
                saltar = 1; // Dejamos de esperar
            } else {
                usleep(22000); // 22 ms de espera
            }
        }
        texto++;
    }
}

void presentacion(char* nombre){
    escribirLento("Año 2038.\n\nLo que comenzó como una extraña enfermedad en una pequeña ciudad se convirtió en el colapso total de la civilización. En cuestión de meses, gobiernos enteros desaparecieron, las comunicaciones se extinguieron y las calles quedaron dominadas por hordas de infectados.\n\nSin embargo, antes de caer, los últimos gobiernos del mundo lanzaron un proyecto de emergencia conocido como Sanctuary. Su objetivo era simple: establecer una red de refugios seguros conectados entre sí y resistir hasta la llegada de una operación nacional de rescate.\n\nHan pasado años desde entonces.\n\nLa mayoría de los refugios han sido abandonados, destruidos o tomados por los infectados. Los caminos que alguna vez fueron seguros ahora están llenos de peligros desconocidos. Los recursos son escasos, la medicina casi inexistente y cada día aparecen nuevas amenazas.\n\nTú eres el líder de uno de los últimos grupos de supervivientes que aún conserva la esperanza.\n\nUna transmisión de radio recibida hace pocos días cambió todo:\n\n============================================================================\n'Si alguien sigue con vida... manténganse ... flota de... \n la evacuación llegará en ... Resistan. No abandonen la red Sanctuary...'\n============================================================================\n\nAhora tu misión es clara: mantener con vida a tu grupo hasta que llegue el rescate.\n\nCada decisión tendrá consecuencias.\n\n¿Enviarás a tus mejores exploradores a buscar suministros sabiendo que podrían no regresar?\n\n¿Compartirás la última dosis de medicina con un herido grave o la guardarás para una emergencia mayor?\n\n¿Cruzarás una ruta infestada para alcanzar un refugio seguro?\n\nMientras luchas contra el hambre, la sed, el desgaste fisico de los supervivientes, descubrirás que los infectados no son el único peligro. Tormentas, saqueadores y eventos inesperados pondrán a prueba tu liderazgo.\n\nAlgunos miembros de tu grupo se convertirán en héroes.\n\nOtros no llegarán al amanecer.\n\nY cuando finalmente aparezcan las luces del rescate en el horizonte, solo una pregunta importará:\n\n¿Cuántos de ustedes seguirán vivos para verlo? 🚁🧟‍♂️🏚️\n\n");
}