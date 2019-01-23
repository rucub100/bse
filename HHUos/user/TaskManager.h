/*****************************************************************************
 *                                                                           *
 *                                 L O O P                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Loop ist eine Koroutine, die nichts weiter tut als einen *
 *                  Zaehler hochzuzaehlen und dies auf dem Bildschirm anzu-  *
 *                  zeigen. Zwischendurch gibt sie die CPU ab.               *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/

#ifndef __TaskManager_include__
#define __TaskManager_include__

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"

class TaskManager : public Thread {

/* Hier muss die Koroutine Loop als Klasse definiert werden. */
private:
    TaskManager (const TaskManager &copy); // Verhindere Kopieren

public:
    TaskManager(unsigned int* stack) : Thread(stack)  {}
    ~TaskManager(){}

    void run() {
        static unsigned long stamp = 0;
        const unsigned int interval = 200;
        const unsigned char pit_attr = kout.attribute(CGA::LIGHT_GREY, CGA::BLACK, false);
        unsigned int sec = 0;

        int x, y;
        while (1) {
            if (systime > stamp) {
                stamp = systime + interval;
                if (sec != (systime / 1000)) {
                    sec = (systime / 1000);
                    kout.clear_line(24, pit_attr);
                    kout.get_lock()->acquire();
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
                    kout.get_lock()->free();
                    scheduler.yield();
                }
            }
        }
    }
};

#endif
