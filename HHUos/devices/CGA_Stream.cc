/*****************************************************************************
 *                                                                           *
 *                     C G A _ S T R E A M                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Klasse CGA_Stream ermoeglicht die Ausgabe verschied. *
 *                  Datentypen als Zeichenketten auf dem CGA-Bildschirm eines*
 *                  PCs. Fuer weitergehende Formatierung oder spezielle      *
 *                  Effekte stehen die Methoden der Klasse CGA_Stream zur    *
 *                  Verfuegung.                                              *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 1.8.16          *
 *****************************************************************************/

#include "devices/CGA_Stream.h"


/*****************************************************************************
 * Methode:         CGA_Stream::flush                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Methode zur Ausgabe des Pufferinhalts der Basisklasse    *
 *                  StringBuffer. Die Methode wird implizit aufgerufen,      *
 *                  sobald der Puffer voll ist, kann aber auch explizit      *
 *                  verwendet werden, um eine Ausgabe zu erzwingen.          *
 *****************************************************************************/
void CGA_Stream::flush () {
    print (buffer, pos);
    pos = 0;
}

void CGA_Stream::flush(unsigned char attr) {
    print (buffer, pos, attr);
    pos = 0;
}

void CGA_Stream::println(char *string) {
    println(string, STD_ATTR);
}


void CGA_Stream::println(char *string, unsigned char attr) {
    lock.acquire();
    flush();
    char* pos = string;
    int n = 0;
    while (*pos) {
        pos++;
        n++;
    }
    print(string, n, attr);
    print("\n", 1, STD_ATTR);
    lock.free();
}

void CGA_Stream::print_pos(int x, int y, char* string) {
    print_pos(x, y, string, STD_ATTR);
}

void CGA_Stream::print_pos(int x, int y, char* string, unsigned char attr) {
    int _x, _y;
    lock.acquire();
    flush();
    getpos(_x, _y);
    setpos(x, y);
    char* pos = string;
    int n = 0;
    while (*pos) {
        pos++;
        n++;
    }
    print(string, n, attr);
    setpos(_x, _y);
    lock.free();
}

void CGA_Stream::clear_line(int line_num) {
    clear_line(line_num, STD_ATTR);
}

void CGA_Stream::clear_line(int line_num, unsigned char attr) {
    int _x, _y;
    lock.acquire();
    getpos(_x, _y);
    setpos(0, line_num);
    int c = COLUMNS;
    if (line_num == ROWS - 1) {
        c--;
        show(c, line_num, ' ', attr);
    }
    for (int i = 0; i < c; i++) {
        print(" ", 1, attr);
    }
    setpos(_x, _y);
    lock.free();
}