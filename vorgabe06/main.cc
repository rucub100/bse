/*****************************************************************************
 *                                                                           *
 *                                M A I N                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Startroutine, wird direkt vom Bootlader angesprungen,    *
 *                  nachdem dieser in den Protected Mode geschaltet hat und  *
 *                  die GDT und IDT initalisiert hat.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 16.12.2016                      *
 *****************************************************************************/

#include "kernel/Globals.h"
#include "user/VBEdemo.h"
#include "user/VBEfonts.h"


// Stack fuer den Hauptthread der Anwendung
static unsigned int appl_stack[1024];


int main() {
    // Bildschirm loeschen.
    kout.clear ();
    
    // Startmeldung ausgeben
    kout << "HHUos 0.6" << endl << "=========" << endl << endl;

    kout << "Unterstuetzte Funktionen:" << endl;
    kout << "   - Bildschirmausgaben" << endl;
    kout << "   - Sound ueber den PC-Lautsprecher" << endl;
    kout << "   - Tastatureingaben per Interrupt" << endl;
    kout << "   - kooperatives Multitasking" << endl;
    kout << "   - VESA ueber BIOS" << endl;
    kout << endl;
    kout.flush ();

    
    // Tastatur-Unterbrechungsroutine einstoepseln
    kb.plugin ();

    // Interrupts erlauben (Tastatur)
    cpu.enable_int ();

    // Anwendung im Scheduler anmelden
    VBEdemo demoApp(&appl_stack[1024]);
    scheduler.Scheduler::ready(demoApp);
    
    // VBEfonts demoApp(&appl_stack[1024]);
    // scheduler.Scheduler::ready(demoApp);

    // Scheduler starten
    scheduler.Scheduler::schedule ();

    return 0;
 }
