/*****************************************************************************
 *                                                                           *
 *                                   P I T                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Programmable Interval Timer.                             * 
 *                                                                           *
 * Autor:           Michael Schoettner, 23.8.2016                            *
 *****************************************************************************/

#ifndef __PIT_include__
#define __PIT_include__

#include "kernel/interrupts/ISR.h"
#include "kernel/IOport.h"

class PIT : public ISR {
    
private:
    PIT(const PIT &copy); // Verhindere Kopieren

    IOport control;     // Steuerregister (write only)
    IOport data0;       // Zaehler-0 Datenregister (read/write)
         
private:
    enum { time_base = 838 };  /* ns */
    int timer_interval;

public:
    // Zeitgeber initialisieren.
    PIT (int us) :
    control(0x43),
    data0(0x40) {
        interval (us);
    }
    
    // Konfiguriertes Zeitintervall auslesen.
    int interval () {
        return timer_interval;
    }
    
    // Zeitintervall in Mikrosekunden, nachdem periodisch ein Interrupt
    //erzeugt werden soll.
    void interval (int us);

    // Aktivierung der Unterbrechungen fuer den Zeitgeber
    void plugin ();
    
    // Unterbrechnungsroutine des Zeitgebers.
    void trigger ();
};

#endif
