/*****************************************************************************
 *                                                                           *
 *                     C G A _ S T R E A M                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Klasse CGA_Stream ermoeglicht die Ausgabe verschied. *
 *                  Datentypen als Zeichenketten auf dem CGA-Bildschirm eines*
 *                  PCs. Fuer weitergehende Formatierung oder spezielle      *
 *                  Effekte stehen die Methoden der Klasse CGA zur           *
 *                  Verfuegung.                                              *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 1.8.16          *
 *****************************************************************************/
#ifndef __CGA_Stream_include__
#define __CGA_Stream_include__

#include "devices/CGA.h"
#include "lib/OutStream.h"
#include "lib/Spinlock.h"

class CGA_Stream : public OutStream, public CGA {
    
private:
    CGA_Stream(CGA_Stream &copy); // Verhindere Kopieren

    Spinlock lock;   

public:
    CGA_Stream () : OutStream(), CGA (), lock() { flush(); }
    virtual ~CGA_Stream () {}

    // Methode zur Ausgabe des Pufferinhalts der Basisklasse StringBuffer.
    virtual void flush ();

    void flush (unsigned char attr);

    void println(char *string);

    void print_pos(int x, int y, char* string);

    void clear_line(int line_num);

    void println(char *string, unsigned char attr);

    void print_pos(int x, int y, char* string, unsigned char attr);

    void clear_line(int line_num, unsigned char attr);

    Spinlock* get_lock() { return &lock; }
};

#endif
