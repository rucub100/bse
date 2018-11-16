/*****************************************************************************
 *                                                                           *
 *                         A P P L I C A T I O N                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einstieg in eine Anwendung.                              *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 15.8.2016                       *
 *****************************************************************************/

#include "kernel/Globals.h"
#include "user/Application.h"
#include "user/Loop.h"


static unsigned int stack[3][1024];


/*****************************************************************************
 * Methode:         Application::main                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    main-Methode der Anwendung.                              *
 *****************************************************************************/
void Application::main () {
    Loop a (&stack[0][1024], 1, 0, 15);
    Loop b (&stack[1][1024], 2, 30, 15);
    Loop c (&stack[2][1024], 3, 60, 15);

    a.setNext(&b);
    b.setNext(&c);
    c.setNext(&a);

    a.start();
}
