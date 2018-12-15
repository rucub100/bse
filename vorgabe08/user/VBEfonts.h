/*****************************************************************************
 *                                                                           *
 *                            V B E F O N T S                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Font demo.                                               *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 25.12.2016                      *
 *****************************************************************************/
#ifndef __VBEfonts_include__
#define __VBEfonts_include__


#include "kernel/threads/Thread.h"


class VBEfonts : public Thread {
    
private:
    VBEfonts (const VBEfonts &copy); // Verhindere Kopieren

public:
    // Gib dem Anwendungsthread einen Stack.
    VBEfonts (unsigned int* stack) : Thread (stack) { }

    // Thread-Startmethode
    void run ();
};

#endif
