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
    unsigned int avail, used, meta;
    
    char* addr[1000];

    mm.mm_usage(avail, meta, used);
    kout << endl << "Avail: " << dec << avail << "  Used: " << used << "  Meta: " << meta << "    " << endl;

    for (int i = 0; i < 1000; i++) {
        addr[i] = new char[125000];
        mm.mm_usage(avail, meta, used);
        kout << endl << "Avail: " << dec << avail << "  Used: " << used << "  Meta: " << meta << "                  " << endl;
    }

    //for(int c = 0; c < 100000000; c++);

    for (int i = 1; i < 1000; i+=2) {
        delete addr[i];
        mm.mm_usage(avail, meta, used);
        kout << endl << "Avail: " << dec << avail << "  Used: " << used << "  Meta: " << meta << "                  " << endl;
    }

    //for(int c = 0; c < 100000000; c++);

    for (int i = 0; i < 1000; i+=2) {
        delete addr[i];
        mm.mm_usage(avail, meta, used);
        kout << endl << "Avail: " << dec << avail << "  Used: " << used << "  Meta: " << meta << "                  " << endl;
    }

    kout << "FINISHED" << endl;

    scheduler.exit();
}