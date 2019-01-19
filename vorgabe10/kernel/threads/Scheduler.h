/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers. *
 *                  Rechenbereite Threads werden in 'readyQueue' verwaltet.  *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 17.1.2019                      *
 *****************************************************************************/

#ifndef __Scheduler_include__
#define __Scheduler_include__

#include "kernel/threads/Dispatch.h"
#include "kernel/threads/Thread.h"
#include "lib/Queue.h"

class Scheduler : public Dispatcher {
    
private:
    Scheduler (const Scheduler &copy); // Verhindere Kopieren
     
private:
    Queue readyQueue;   // auf die CPU wartende Threads
    
    // Scheduler wird evt. von einer Unterbrechung vom Zeitgeber gerufen,
    // bevor er initialisiert wurde
    bool  initialized;
    
public:
    Scheduler ();

    // Scheduler initialisiert?
    // Zeitgeber-Unterbrechung kommt evt. bevor der Scheduler fertig
    // intiialisiert wurde!
    bool isInitialized() { return initialized; }
    
    // ruft nur der Idle-Thread (erster Thread der vom Scheduler gestartet wird)
    void setInitialized() { initialized = true; }

    // Scheduler starten
    void schedule ();
    
    // Thread in ReadyQueue eintragen
    void ready (Thread& that);
    
    // Thread terminiert sich selbst
    void exit ();

    // Thread mit 'Gewalt' terminieren
    void kill (Thread& that);

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void yield ();

    // Wartet ein Thread auf CPU-Zuteilung?
    bool isThreadWaiting () {
        return readyQueue.size() > 0 ? true : false;
    }
    
    // CPU soll aktuellem Thread entzogen werden (Vorbereitungen)
    // Wird von Unterbrechungsroutine des PIT gerufen
    bool prepare_preemption ();

    // Aufrufer in die Warteschlange der Semaphor-Variable und
    // auf den nächsten Thread umschalten.
    void block ();

    // that in die readyQueue und zurückkehren
    // Umschalten erfolgt später
    void deblock (Thread& that);
};

#endif
