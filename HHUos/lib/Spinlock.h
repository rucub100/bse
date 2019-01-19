/*****************************************************************************
 *                                                                           *
 *                             S P I N L O C K                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung von Spinlock-Objekten.                   *
 *                                                                           *
 * Autor:           Ruslan Curbanov, 19.01.2019                              *
 *****************************************************************************/

#ifndef __Spinlock_include__
#define __Spinlock_include__

class Spinlock {

private:
    Spinlock (const Spinlock &copy); // Verhindere Kopieren

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
    unsigned long lock;
    unsigned long *ptr;

public:
    Spinlock() : lock(0), ptr(&lock) {}

    void acquire()
    {
        while (CAS(ptr, 0, 1) != 1) ;
    }

    void free()
    {
        lock = 0;
    }
};

#endif
