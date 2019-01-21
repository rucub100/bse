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

#ifndef __PlayTetris_include__
#define __PlayTetris_include__

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"

class PlayTetris : public Thread {

/* Hier muss die Koroutine Loop als Klasse definiert werden. */
private:
    PlayTetris (const PlayTetris &copy); // Verhindere Kopieren

public:
    PlayTetris(unsigned int* stack) : Thread(stack)  {}
    ~PlayTetris(){}

    void run() {
        pcspk.tetris();
    }
};

#endif
