/*****************************************************************************
 *                                                                           *
 *                         I N T D I S P A T C H E R                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zentrale Unterbrechungsbehandlungsroutine des Systems.   *
 *                  Der Parameter gibt die Nummer des aufgetretenen          *
 *                  Interrupts an. Wenn eine Interrupt Service Routine       *
 *                  registriert ist, wird diese aufgerufen.                  *
 *                                                                           *
 * Autor:           Michael Schoettner, 31.8.2016                            *
 *****************************************************************************/
#include "kernel/CPU.h"
#include "kernel/Globals.h"
#include "kernel/IntDispatcher.h"


extern "C" void int_disp (unsigned int slot);


/*****************************************************************************
 * Prozedur:        int_disp                                                 *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Low-Level Interrupt-Behandlung.                          *
 *                  Diese Funktion ist in der IDT fuer alle Eintraege einge- *
 *                  tragen. Dies geschieht bereits im Bootloader.            *
 *                  Sie wird also fuer alle Interrupts aufgerufen. Von hier  *
 *                  aus sollen die passenden ISR-Routinen der Treiber-Objekte*
 *                  aufgerufen werden.                                       *
 *****************************************************************************/
void int_disp (unsigned int slot) {

    /* hier muss Code eingefuegt werden */
    
    kout << "Ein Interrupt ist aufgetreten" << slot << endl;

}


/*****************************************************************************
 * Konstruktor:     IntDispatcher::IntDispatcher                             *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Initialisierung der ISR map mit einer Default-ISR.       *
 *****************************************************************************/
IntDispatcher::IntDispatcher () {
    for (unsigned int slot=0; slot<size; slot++)
        map[slot] = 0;
}


/*****************************************************************************
 * Methode:         IntDispatcher::assign                                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Registrierung einer ISR.                                 *
 *                                                                           *
 * Parameter:                                                                *
 *      slot:       Nummer der Unterbrechung                                 *
 *      isr:        ISR die registriert werden soll                          *
 *****************************************************************************/
void IntDispatcher::assign (unsigned int slot, ISR& isr) {

    /* hier muss Code eingefuegt werden */

}


/*****************************************************************************
 * Methode:         IntDispatcher::assign                                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Eingetragene ISR ermitteln.                              *
 *                                                                           *
 * Parameter:                                                                *
 *      slot:       Gibt ein registriertes ISR-Objekt oder 0 zurueck         *
 *****************************************************************************/
ISR* IntDispatcher::report (unsigned int slot) {

    /* hier muss Code eingefuegt werden */

}
