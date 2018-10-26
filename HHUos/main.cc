/*****************************************************************************
 *                                                                           *
 *                                M A I N                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Startroutine, wird direkt vom Bootlader angesprungen,    *
 *                  nachdem dieser in den Protected Mode geschaltet hat und  *
 *                  die GDT und IDT initalisiert hat.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 15.8.2016                       *
 *****************************************************************************/

#include "kernel/Globals.h"


int main() {
    kout.clear();
    kout << "hello osdev :)" << endl;
    kout << "this is fun" << endl;
    int var = 128;
    kout << hex << var << endl;
    kout << bin << var << endl;
    kout << oct << var << endl;
    kout << dec << var << endl;
    kout << "7" << endl;
    kout << "8" << endl;
    kout << "9" << endl;
    kout << "10" << endl;
    kout << "11" << endl;
    kout << "12" << endl;
    kout << "13" << endl;
    kout << "14" << endl;
    kout << "15" << endl;
    kout << "16" << endl;
    kout << "17" << endl;
    kout << "18" << endl;
    kout << "19" << endl;
    kout << "20" << endl;
    kout << "21" << endl;
    kout.flush();

    pcspk.tetris();

    for(int i = 0; i < 1000000; i++);

    pcspk.aerodynamic();

	while (1);

    /* Hier muess Code eingefuegt werden */

    // Bildschirm loeschen.

    // Startmeldung ausgeben

    // Bildschirmausgabe testen (Strings, hex, dec, ...)

    // PC-Lautsprecher testen

    return 0;
 }
