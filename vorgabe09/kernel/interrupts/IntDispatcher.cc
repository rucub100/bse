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
 * Autor:           Michael Schoettner, 1.1.2017                             *
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
 *                                                                           *
 * Rueckgabewert:   Thread-Wechsel veranlassen? Die geht nur indirekt ueber  *
 *                  den Assembler-Code, da hier der IRET verwendet wird und  *
 *                  nur so Interrupts wieder zugelassen werden und auch der  *
 *                  Stack richtig aufgeräumt werden muss.                    *
 *                  Interrupt-Routine des PIT setzt 'forceSwitch', falls die *
 *                  Zeitscheibe aufgebraucht ist.                            * 
 *****************************************************************************/
bool int_disp (unsigned int slot) {
    ISR* isr;
    
    if (slot <32) {
        bs_dump(slot);
        cpu.halt ();
    }

    isr = intdis.report (slot);
    if (isr!=0) {
        isr->trigger();
        if (forceSwitch==1) {
            forceSwitch=0;
            return true;
        }
    }
    else {
        kout << "Panic: unexpected interrupt " << slot;
        kout << " - processor halted." << endl;
        cpu.halt ();
    }
    return false;
}


