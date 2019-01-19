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

    unsigned char default_attr;

    // Copy Konstrutkor unterbinden
    CGA(const CGA &copy);

public:
    const char *CGA_START;  // Startadresse des Buldschirmspeichers
    
    // Konstruktur mit Initialisierung der Ports
    CGA () :
    index_port (0x3d4),
    data_port (0x3d5),
    default_attr (STD_ATTR) {
        CGA_START = (const char*)0xb8000;

        // Schalte Attribut Modus für Blinken ein statt mehr Hintergrundfarben
        IOport vga_adr (0x3c0);
        IOport vga_arr (0x3c1);
        IOport vga_is1r (0x3da);
        vga_is1r.inb();
        unsigned char tmp = vga_adr.inb();
        vga_adr.outb(0x10);
        unsigned char v = vga_adr.inb();
        vga_adr.outb(v | 0x8);
        vga_adr.outb(tmp);
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

    unsigned char get_default_attr () { return default_attr; }
    
    void set_default_attr (unsigned char attr) { default_attr = attr; }

    void set_default_attr (CGA::color bg, CGA::color fg, bool blink) { 
        default_attr = attribute(bg, fg, blink); 
    }
};

#endif

