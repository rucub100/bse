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


extern "C" bool int_disp (unsigned int slot);


/*****************************************************************************
 * Prozedur:        int_disp                                                 *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Low-Level Interrupt-Behandlung.                          *
 *                  Diese Funktion ist in der IDT fuer alle Eintraege einge- *
 *                  tragen. Dies geschieht bereits im Bootloader.            *
 *****************************************************************************/
void int_disp (unsigned int slot) {
    ISR* isr;

    
   /*****************************************************************************/
   /* Diese IF-Abfrage muss in den eigenen Code eingebaut werden                */
   /* Hiermit wird im Falle einer x86-Exception der Bluescreen aufgerufen       */
   /*****************************************************************************/

    if (slot <32) {
        bs_dump(slot);
        cpu.halt ();
    }

  

