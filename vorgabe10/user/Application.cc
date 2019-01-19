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

static unsigned int stack[2][1024];
static Semaphore sem(1);

/*****************************************************************************
 * Methode:         Application::run                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Der Anwendungsthread erzeugt drei Threads die Zaehler    *
 *                  ausgeben und terminiert sich selbst.                     *
 *****************************************************************************/
void Application::run () {
    Loop l1(&stack[0][1024], 10, 20, &sem);
    Loop l2(&stack[1][1024], 40, 20, &sem);
    scheduler.ready(l1);
    scheduler.ready(l2);

    //pcspk.tetris();
    pcspk.aerodynamic();

    scheduler.exit();
}
