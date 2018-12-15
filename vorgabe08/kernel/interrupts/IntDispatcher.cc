/*****************************************************************************
 *                                                                           *
 *                         I N T D I S P A T C H E R                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zentrale Unterbrechungsbehandlungsroutine des Systems.   *
 *                  Der Parameter gibt die Nummer des aufgetretenen          *
 *                  Interrupts an. Wenn eine Interrupt Service Routine       *
 *                  registriert ist, wird diese aufgerufen.                  *
 *                  x86 Exceptions werden an Bluescreen.cc weitergeleitet.   *
 *                                                                           *
 * Autor:           Michael Schoettner, 14.12.2018                           *
 *****************************************************************************/
#include "kernel/CPU.h"
#include "kernel/Globals.h"
#include "kernel/interrupts/IntDispatcher.h"
#include "kernel/interrupts/Bluescreen.h"


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
    // https://wiki.osdev.org/8259_PIC#Spurious_IRQs
    if (slot == 39 || slot == 47) { // Master or Slave - IRQ7
        if (pic.status(slot - 32)) {
            return;
        }
    }

    /*****************************************************************************/
    /* Hiermit wird im Falle einer x86-Exception der Bluescreen aufgerufen       */
    /*****************************************************************************/
    if (slot <32) {
        bs_dump(slot);
        cpu.halt ();
    }

    ISR* isr =  intdis.report(slot);

    if (isr != 0) {
        isr->trigger();
    } else {
        cpu.disable_int();
        kout.clear();
        for (int i = 0; i < 16; i++) {
            if (!pic.status(i)) {
                kout << "int " << i << endl;
            }
        }

        kout << "ERROR: ISR " << slot << " not assigned!" << endl;
        cpu.halt();
    }
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
    map[slot] = &isr;
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
    return map[slot];
}
