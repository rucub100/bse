/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers. *
 *                  Rechenbereite Threads werden in 'readQueue' verwaltet.   *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 22.8.2016                      *
 *****************************************************************************/

#include "kernel/threads/Scheduler.h"
#include "kernel/threads/IdleThread.h"


/* hier soll der Idle-Thread als globale Variable definiert werden */


/*****************************************************************************
 * Methode:         Scheduler::Scheduler                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Konstruktor des Schedulers. Registriert den Leerlauf-    *
 *                  Thread in der readyQueue.                                *
 *****************************************************************************/
Scheduler::Scheduler () {

    /* hier muss Code eingefuegt werden */
    
}

    
/*****************************************************************************
 * Methode:         Scheduler::schedule                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Scheduler starten. Wird nur einmalig aus main.cc gerufen.*
 *****************************************************************************/
void Scheduler::schedule () {

    /* hier muss Code eingefuegt werden */

}


/*****************************************************************************
 * Methode:         Scheduler::ready                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread in readyQueue eintragen.                          *
 *                                                                           *
 * Parameter:                                                                *
 *      that        Einzutragender Thread                                    *
 *****************************************************************************/
void Scheduler::ready (Thread& that) {

    /* hier muss Code eingefuegt werden */

}


/*****************************************************************************
 * Methode:         Scheduler::exit                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread ist fertig und terminiert sich selbst. Er traegt  *
 *                  sich hierzu aus der readyQueue aus und dann wird auf den *
 *                  naechsten Thread mithilfe des Dispatchers umgeschaltet.  *
 *****************************************************************************/
void Scheduler::exit () {

    /* hier muss Code eingefuegt werden */

}


/*****************************************************************************
 * Methode:         Scheduler::kill                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread mit 'Gewalt' terminieren. Er wird aus der         *
 *                  readyQueue ausgetragen und wird dann nicht mehr aufge-   *
 *                  rufen. Der Aufrufer dieser Methode muss ein anderer      *
 *                  Thread sein.                                             *
 *                                                                           *
 * Parameter:                                                                *
 *      that        Zu terminierender Thread                                 *
 *****************************************************************************/
void Scheduler::kill (Thread& that) {

    /* hier muss Code eingefuegt werden */

}


/*****************************************************************************
 * Methode:         Scheduler::yield                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    CPU freiwillig abgeben und Auswahl des naechsten Threads.*
 *                  Naechsten Thread aus der readyQueue holen, den aktuellen *
 *                  aus der readyQueue austragen und auf den naechsten       *
 *                  mithilfe des Dispatchers umschalten.                     *
 *****************************************************************************/
void Scheduler::yield () {

    /* hier muss Code eingefuegt werden */

}
