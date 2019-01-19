/*****************************************************************************
 *                                                                           *
 *                             S E M A P H O R E                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung von Sempahor-Objekten.                   *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.9.2016                             *
 *****************************************************************************/

#include "lib/Semaphore.h"

void Semaphore::p() {
    lock.acquire();
    
    if (counter > 0) {
        counter--;
    } else {
        // Warte auf v
        waitQueue.enqueue(scheduler.active());
        scheduler.block();
    }

    lock.free();
}

void Semaphore::v() {
    lock.acquire();
    
    if (waitQueue.size() > 0) {
        // Prozess aus der waitQueue in die readyQueue
        scheduler.deblock(*((Thread*)waitQueue.dequeue()));
    } else {
        counter++;
    }

    lock.free();
}