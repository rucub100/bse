/*****************************************************************************
 *                                                                           *
 *                         A P P L I C A T I O N                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einstieg in eine Anwendung.                              *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 21.8.2016                       *
 *****************************************************************************/

#include "kernel/Globals.h"
#include "user/Application.h"
#include "user/Loop.h"

static unsigned int stack[3][1024];

/*****************************************************************************
 * Methode:         Application::run                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Der Anwendungsthread erzeugt drei Threads die Zaehler    *
 *                  ausgeben und terminiert sich selbst.                     *
 *****************************************************************************/
void Application::run () {
    static bool init = false;
    static unsigned count = 0;

    Loop l1(&stack[0][1024], 10, 20);
    Loop l2(&stack[1][1024], 30, 20);
    Loop l3(&stack[2][1024], 50, 20);
    
    if (init) goto loop;
    init = true;
    
    scheduler.ready(l1);
    scheduler.ready(l2);
    scheduler.ready(l3);
    
    loop:
    // terminiere loop 2
    while (count < 10000) {
        count++;
        scheduler.yield();
    }

    scheduler.kill(l3);

    while (count < 50000) {
        count++;
        scheduler.yield();
    }

    scheduler.kill(l1);

    scheduler.exit();
}
