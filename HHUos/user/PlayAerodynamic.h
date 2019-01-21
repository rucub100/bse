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

#ifndef __PlayAerodynamic_include__
#define __PlayAerodynamic_include__

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"

class PlayAerodynamic : public Thread {

/* Hier muss die Koroutine Loop als Klasse definiert werden. */
private:
    PlayAerodynamic (const PlayAerodynamic &copy); // Verhindere Kopieren

public:
    PlayAerodynamic(unsigned int* stack) : Thread(stack)  {}
    ~PlayAerodynamic(){}

    void run() {
        pcspk.aerodynamic();
    }
};

#endif
