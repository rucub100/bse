/*****************************************************************************
 *                                                                           *
 *                                   P I T                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Programmable Interval Timer.                             *
 *                                                                           *
 * Autor:           Michael Schoettner, 25.8.2016                            *
 *****************************************************************************/

#include "devices/PIT.h"
#include "kernel/Globals.h"


/*****************************************************************************
 * Methode:         PIT::interval                                            *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zeitinervall programmieren.                              *
 *                                                                           *
 * Parameter:                                                                *
 *      us:         Zeitintervall in Mikrosekunden, nachdem periodisch ein   * 
 *                  Interrupt erzeugt werden soll.                           *
 *****************************************************************************/
void PIT::interval (int us) {
    int ns = us * 1000;
    int count = ns / time_base;
    // Channel 0, Mode 3, 16-bit, lo/hi
    control.outb(0x36);
    data0.outb(count);
    data0.outb(count >> 8);
}


/*****************************************************************************
 * Methode:         PIT::plugin                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Unterbrechungen fuer den Zeitgeber erlauben. Ab sofort   *
 *                  wird bei Ablauf des definierten Zeitintervalls die       *
 *                  Methode 'trigger' aufgerufen.                            *
 *****************************************************************************/
void PIT::plugin () {
    intdis.assign(IntDispatcher::timer, pit);
    pic.allow(PIC::timer);
}


/*****************************************************************************
 * Methode:         PIT::trigger                                             *
 *---------------------------------------------------------------------------*
 * Beschreibung:    ISR fuer den Zeitgeber. Wird aufgerufen, wenn der        * 
 *                  Zeitgeber eine Unterbrechung ausloest. Anzeige der Uhr   *
 *                  aktualisieren und Thread wechseln durch Setzen der       *
 *                  Variable 'threadSwitch', wird in 'int_disp' behandelt.   *
 *****************************************************************************/
void PIT::trigger () {
    static unsigned int ticks = 0;
    const unsigned int speed = 100;

    char timer_sym[] = { '-', '\\', '|', '/' };
    if (ticks % speed == 0) {
        int x, y;
        kout.getpos(x, y);
        kout.flush();
        kout.setpos(79, 0);
        kout << timer_sym[(ticks / speed) % 4] << endl;
        kout.setpos(x, y);
    }

    // alle 1ms, Systemzeit weitersetzen
    systime++;

    // Bei jedem Tick einen Threadwechsel ausloesen.
    // Aber nur wenn der Scheduler bereits fertig intialisiert wurde
    // und ein weiterer Thread rechnen moechte
    if ( scheduler.isInitialized() && (ticks % 10 == 0)) {
        if ( scheduler.prepare_preemption() ) {
            forceSwitch=1;
        } else {
            cpu.disable_int();
            kout.clear();
            kout << "PANIC: fatal scheduler error" << endl;
            cpu.halt();
        }
    }

    ticks++;
}


