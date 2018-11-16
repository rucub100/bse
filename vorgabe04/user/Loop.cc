/*****************************************************************************
 *                                                                           *
 *                                 L O O P                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Loop ist eine Koroutine, die nichts weiter tut als einen *
 *                  Zaehler hochzuzaehlen und dies auf dem Bildschirm anzu-  *
 *                  zeigen. Zwischendurch gibt sie die CPU ab.               *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/

#include "user/Loop.h"
#include "kernel/Globals.h"


/* Hier folgt die Implementierung der Methode(n) der Klasse Loop */

void Loop::run() {
    for (unsigned int i = 0; i < 1000000000; i++) {
        kout.setpos(_x, _y);
        kout << count;
        kout.flush();
        count++;
        yield();
    }
}