/*****************************************************************************
 *                                                                           *
 *                                M A I N                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Startroutine, wird direkt vom Bootlader angesprungen,    *
 *                  nachdem dieser in den Protected Mode geschaltet hat und  *
 *                  die GDT und IDT initalisiert hat.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 11.11.2016                      *
 *****************************************************************************/

#include "kernel/Globals.h"


int main() {
    // Bildschirm loeschen.
    kout.clear();
    
    // Startmeldung ausgeben
    kout << "HHUos 0.2" << endl << "=========" << endl << endl;
    
    kout << "Unterstuetzte Funktionen:" << endl;
    kout << "   - Bildschirmausgaben" << endl;
    kout << "   - Sound ueber den PC-Lautsprecher" << endl;
    kout << "   - Tastatureingaben per Abfrage" << endl;
    kout << endl;
    kout.flush();
    
    // Test some numbers
    kout << "Test der Zahlenausgabefunktionen" << endl;
    kout.setpos(0,8);
    for (int i = 0; i < 17; i+=2)
        kout << dec << i << ' ' << hex << i << "    " << endl;
    
    kout << endl << "Tastatur mit Eingaben bitte testen" << endl;

    kb.set_repeat_rate(0, 1);

    while (true) {
        Key key;
        
        /* hier Code einfügen ('key_hit' aufrufen und Zeichen ausgeben) */
        
        key = kb.key_hit();

        if (key.valid()) {
            kout << key.ascii();
            kout.flush();
        }
    }
    
    return 0;
 }
