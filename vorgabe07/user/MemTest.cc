/**
 * 
 * Test: Speicherverwaltung
 * 
 * Autor: Ruslan Curbanov, HHU, 09.12.2018
 * 
 **/

#include "user/MemTest.h"
#include "kernel/Globals.h"

void MemTest::run() {
    static unsigned int i = 1;
    unsigned int avail, used, meta;
    
    while (i < 1000) {
        char* x = new char[132117];
        i++;
        mm.mm_usage(avail, meta, used);
        kout << endl << "Avail: " << avail << "  Used: " << used << "  Meta: " << meta << "    " << endl;
        scheduler.yield();
    }

    char* y = new char[600];
    mm.mm_usage(avail, meta, used);
    kout << endl << "Avail: " << avail << "  Used: " << used << "  Meta: " << meta << "    " << endl;

    kout << "FINISHED" << endl;

    scheduler.exit();
}