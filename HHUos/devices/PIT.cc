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
    const unsigned int speed = 100;
    const unsigned char pit_attr = kout.attribute(CGA::LIGHT_GREY, CGA::BLACK, false);

    char* timer_sym[] = { "[-]", "[\\]", "[|]", "[/]" };
    if (systime % speed == 0) {
        kout.print_pos(kout.COLUMNS - 3, 0, timer_sym[(systime / speed) % 4], pit_attr);
    }
    if (systime % 1000 == 0) {
        int x, y;
        int sec = (systime / 1000);
        kout.clear_line(24, pit_attr);
        kout.flush();
        kout.getpos(x, y);
        kout.setpos(kout.COLUMNS - 60, kout.ROWS - 1);
        unsigned int avail, meta, used;
        mm.mm_usage(avail, meta, used);
        kout << avail << "/" << used << "/" << meta;
        kout << " | #Threads: " << dec << scheduler.totalThreads();
        kout << " | Uptime: ";
        if (sec / 10 == 0) kout << "0";
        if (sec / 100 == 0) kout << "0";
        if (sec / 1000 == 0) kout << "0";
        if (sec / 10000 == 0) kout << "0";
        if (sec / 100000 == 0) kout << "0";
        kout << dec << sec << " Sek.";
        kout.flush(pit_attr);
        kout.setpos(x, y);
    }

    // alle 1ms, Systemzeit weitersetzen
    systime++;

    // Bei jedem Tick einen Threadwechsel ausloesen.
    // Aber nur wenn der Scheduler bereits fertig intialisiert wurde
    // und ein weiterer Thread rechnen moechte
    if ( scheduler.isInitialized() && (systime % 10 == 0)) {
        if ( scheduler.prepare_preemption() ) {
            forceSwitch=1;
        } else {
            cpu.disable_int();
            kout.clear();
            kout << "PANIC: fatal scheduler error" << endl;
            cpu.halt();
        }
    }
}


