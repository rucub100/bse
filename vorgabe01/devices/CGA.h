/*****************************************************************************
 *                                                                           *
 *                                    C G A                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Mit Hilfe dieser Klasse kann man auf den Bildschirm des  *
 *                  PCs zugreifen. Der Zugriff erfolgt direkt auf der Hard-  *
 *                  wareebene, d.h. ueber den Bildschirmspeicher und den     *
 *                  I/O-Ports der Grafikkarte.                               *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 21.8.2016       *
 *****************************************************************************/
#ifndef __CGA_include__
#define __CGA_include__

#include "kernel/IOport.h"

class CGA {
    
private:
    IOport index_port;      // Auswahl eines Register der Grafikkarte
    IOport data_port;       // Lese-/Schreib-Zugriff auf Register der Grafikk.
    
    unsigned int cursor_pos_x;
    unsigned int cursor_pos_y;

    // Copy Konstrutkor unterbinden
    CGA(const CGA &copy);

    void flush_cursor_pos() {
        unsigned int pos = cursor_pos_x + cursor_pos_y * COLUMNS;

        //high byte
        index_port.outb(14);
        data_port.outb((unsigned char) (pos >> 8));

        // low byte
        index_port.outb(15);
        data_port.outb((unsigned char) pos);
    }
    
public:
    const char *CGA_START;  // Startadresse des Buldschirmspeichers
    
    // Konstruktur mit Initialisierung der Ports
    CGA () :
    index_port (0x3d4),
    data_port (0x3d5),
    cursor_pos_x (0),
    cursor_pos_y (0) {
        CGA_START = (const char*)0xb8000;
        flush_cursor_pos();
    }
    
    // Konstanten fuer die moeglichen Farben im Attribut-Byte.
    typedef enum {
        BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GREY,
        DARK_GREY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED,
        LIGHT_MAGENTA, YELLOW, WHITE
    } color;
    
    // Standardzeichenfarbe
    enum { STD_ATTR = BLACK << 4 | LIGHT_GREY };
    
    // Groesse des Bildschirms (25 Zeilen, 80 Spalten)
    enum { ROWS = 25, COLUMNS = 80 };
    
    // Setzen des Cursors in Spalte x und Zeile y.
    void setpos (int x, int y);
    
    // Abfragen der Cursorpostion
    void getpos (int& x, int& y);
    
    // Anzeige eines Zeichens mit Attribut an einer bestimmten Stelle
    void show (int x, int y, char character, unsigned char attrib = STD_ATTR);
    
    // Anzeige mehrerer Zeichen ab der aktuellen Cursorposition
    void print (char* string, int n, unsigned char attrib = STD_ATTR);
    
    // Verschiebt den Bildschirminhalt um eine Zeile nach oben.
    // Neue Zeile am unteren Bildrand mit Leerzeichen fuellen
    void scrollup ();
    
    // Lösche den Textbildschirm
    void clear ();
    
    // Hilfsfunktion zur Erzeugung eines Attribut-Bytes
    unsigned char attribute (CGA::color bg, CGA::color fg, bool blink);
};

#endif

