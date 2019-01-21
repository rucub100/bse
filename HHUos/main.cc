/*****************************************************************************
 *                                                                           *
 *                                M A I N                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Startroutine, wird direkt vom Bootlader angesprungen,    *
 *                  nachdem dieser in den Protected Mode geschaltet hat und  *
 *                  die GDT und IDT initalisiert hat.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 21.12.2018                      *
 *****************************************************************************/


#include "kernel/Globals.h"
#include "kernel/MemMgmt.h"
#include "user/Application.h"

int main() {
    // Speicherverwaltung initialisieren
    mm.mm_init();
    
    // Tastatur-Unterbrechungsroutine einstoepseln
    kb.plugin ();

    // Zeitgeber starten
    pit.plugin ();

    // Paging aktivieren
    pg_init ();

    // Interrupts erlauben (Tastatur)
    cpu.enable_int ();

    // Anwendung im Scheduler anmelden
    unsigned int *appl_stack = (unsigned int *) mm.mm_alloc(1024);
    Application demoApp(&appl_stack[1024]);
    scheduler.ready(demoApp);
    
    // Scheduler starten
    scheduler.schedule ();
    
    cpu.disable_int();
    kout.setpos(0, 0);
    kout.clear();
    kout << "PANIC: main method must not return!!!" << endl;
    cpu.halt();
    while(1);
    return 0;
 }
