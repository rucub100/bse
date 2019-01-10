/**
 * 
 * Test: Speicherverwaltung
 * 
 * Autor: Ruslan Curbanov, HHU, 09.12.2018
 * 
 **/

#ifndef __MemTest_include__
#define __MemTest_include__

#include "kernel/threads/Thread.h"

class TestObject {
    private:
        int test_int_1;
        int test_int_2;
        int test_int_3;
        int test_int_4;
        bool test_bool;
    public:
        TestObject() {
            test_int_1 = 10;
            test_int_2 = 100;
            test_int_3 = 1000;
            test_int_4 = 10000;
            test_bool = true;
        };
};

class MemTest : public Thread {

private:
    MemTest (const MemTest &copy); // Verhindere Kopieren

public:
    // Gib dem Anwendungsthread einen Stack.
    MemTest (unsigned int* stack) : Thread (stack) { }

    // Thread-Startmethode
    void run ();

};

#endif