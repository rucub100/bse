/*****************************************************************************
 *                                                                           *
 *                             S E M A P H O R E                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung von Sempahor-Objekten.                   *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.9.2016                             *
 *****************************************************************************/

#ifndef __Semaphore_include__
#define __Semaphore_include__


#include "lib/Queue.h"
#include "kernel/Globals.h"

class Semaphore {

private:
    Semaphore (const Semaphore &copy); // Verhindere Kopieren

    // Queue fuer wartende Threads.
    Queue waitQueue;

    int counter;

    // https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/cmpxchg.h
    static inline unsigned long CAS(unsigned long *ptr, unsigned long old, unsigned long _new) {
        volatile unsigned long *__ptr = (volatile unsigned long *)(ptr);
        __typeof__(*(ptr)) __ret;
	    __typeof__(*(ptr)) __old = (old);
        __typeof__(*(ptr)) __new = (_new);
		asm volatile("lock;"
                     "cmpxchgl %2,%1"
			         : "=a" (__ret), "+m" (*__ptr)
			         : "r" (__new), "0" (__old)
                     : "memory");
        return __ret;
    }

    /*
        Lock stuff
    */
    unsigned long lock = 0;
    unsigned long *ptr = &lock;

    void acquire_spinlock(int t)
    {
        while (CAS(ptr, 0, t) != t) ;
    }

    void free_spinlock()
    {
        lock = 0;
    }

public:
    // Konstruktor: Initialisieren des Semaphorzaehlers
    Semaphore (int c) : counter (c), waitQueue() {}

    // 'Passieren': Warten auf das Freiwerden eines kritischen Abschnitts.
    void p ();

    // 'Vreigeben': Freigeben des kritischen Abschnitts.
    void v ();
 };

#endif
