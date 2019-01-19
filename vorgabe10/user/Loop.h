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

#ifndef __loop_include__
#define __loop_include__

#include "kernel/threads/Thread.h"
#include "lib/Semaphore.h"

class Loop : public Thread {

/* Hier muss die Koroutine Loop als Klasse definiert werden. */
private:
    Loop (const Loop &copy); // Verhindere Kopieren

    unsigned int count;
    unsigned int _x;
    unsigned int _y;

    Semaphore* _s;

public:
    Loop(unsigned int* stack, unsigned int x, unsigned int y, Semaphore* s) 
    : Thread(stack), count (0) {
        _x = x;
        _y = y;
        _s = s;
    }

    ~Loop(){}

    void run();
};

#endif
