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

#include "kernel/threads/Dispatch.h"


/*****************************************************************************
 * Methode:         Dispatcher::start                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread starten.                                          *
 *                                                                           *
 * Parameter:                                                                *
 *      first       Zu startender Thread.                                    *
 *****************************************************************************/
void Dispatcher::start (Thread& first) {
    if (!life) {
        life = &first;
        life->start ();
    }
}


/*****************************************************************************
 * Methode:         Dispatcher::dispatch                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf einen gegebenen Thread wechseln.                     *
 *                                                                           *
 * Parameter:                                                                *
 *      next        Thread der die CPU erhalten soll.                        *
 *****************************************************************************/
void Dispatcher::dispatch (Thread& next) {
    Thread* current = life;
    life = &next;
    current->switchTo (next);
}
