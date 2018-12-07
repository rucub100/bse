/*****************************************************************************
 *                                                                           *
 *                          D I S P A T C H E R                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung des Dispatchers.                         *
 *                  Der Dispatcher verwaltet den life-Pointer, der den       *
 *                  jeweils aktiven Thread angibt. Mit start() wird der      *
 *                  life Pointer initialisiert und der erste Thread ge-      *
 *                  startet, alle weiteren Kontextwechsel werden mit         *
 *                  dispatch() ausgeloest. active() liefert den life Pointer *
 *                  zurueck.                                                 *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/

#ifndef __dispatch_include__
#define __dispatch_include__

#include "kernel/threads/Thread.h"

class Dispatcher {
    
private:
    Thread* life;     // aktiver Thread

    Dispatcher(const Dispatcher &copy); // Verhindere Kopieren

public:
    Dispatcher () : life (0) {}
    void start (Thread& first);
    void dispatch (Thread& next);
    Thread* active () { return life; }
};

#endif
