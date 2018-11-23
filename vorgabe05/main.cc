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
#include "user/Application.h"


// Stack fuer den Hauptthread der Anwendung
static unsigned int appl_stack[1024];


int main() {
    // Bildschirm loeschen.
    kout.clear ();
    
    // Startmeldung ausgeben
    kout << "HHUos 0.5" << endl << "=========" << endl << endl;

    kout << "Unterstuetzte Funktionen:" << endl;
    kout << "   - Bildschirmausgaben" << endl;
    kout << "   - Sound ueber den PC-Lautsprecher" << endl;
    kout << "   - Tastatureingaben per Interrupt" << endl;
    kout << "   - kooperatives Multitasking" << endl;
    kout << endl;
    kout.flush ();

    
    // Tastatur-Unterbrechungsroutine einstoepseln
    kb.plugin ();

    // Interrupts erlauben (Tastatur)
    cpu.enable_int ();

    // Anwendung im Scheduler anmelden
    Application application(&appl_stack[1024]);
    scheduler.Scheduler::ready(application);
    
    // Scheduler starten
    scheduler.Scheduler::schedule ();

    return 0;
 }
