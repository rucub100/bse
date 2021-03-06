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
    acquire_spinlock(1);
    
    if (counter > 0) {
        counter--;
    } else {
        // Warte auf v
        waitQueue.enqueue(scheduler.active());
        scheduler.block();
    }

    free_spinlock();
}

void Semaphore::v() {
    acquire_spinlock(1);
    
    if (waitQueue.size() > 0) {
        // Prozess aus der waitQueue in die readyQueue
        scheduler.deblock(*((Thread*)waitQueue.dequeue()));
    } else {
        counter++;
    }

    free_spinlock();
}