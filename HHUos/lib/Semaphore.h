/*****************************************************************************
 *                                                                           *
 *                             S E M A P H O R E                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung von Sempahor-Objekten.                   *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.9.2016                             *
 *****************************************************************************/

#ifndef __Semaphore_include__
#define __Semaphore_include__


#include "lib/Queue.h"
#include "kernel/Globals.h"
#include "lib/Spinlock.h"

class Semaphore {

private:
    Semaphore (const Semaphore &copy); // Verhindere Kopieren

    Spinlock lock;

    // Queue fuer wartende Threads.
    Queue waitQueue;

    int counter;

public:
    // Konstruktor: Initialisieren des Semaphorzaehlers
    Semaphore (int c) : counter (c), waitQueue(), lock() {}

    // 'Passieren': Warten auf das Freiwerden eines kritischen Abschnitts.
    void p ();

    // 'Vreigeben': Freigeben des kritischen Abschnitts.
    void v ();
 };

#endif
