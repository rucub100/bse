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
#include "devices/CGA.h"


/*****************************************************************************
 * Methode:         CGA::setpos                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Setzen des Cursors in Spalte x und Zeile y.              *
 *****************************************************************************/
void CGA::setpos (int x, int y) {
    if (0 <= x && x < COLUMNS) {
        cursor_pos_x = x;
    }

    if (0 <= y && y < ROWS) {
        cursor_pos_y = y;
    }

    flush_cursor_pos();
    //TODO: error screen for x else case?!; scroll for y? buff size?
}

/*****************************************************************************
 * Methode:         CGA::getpos                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Abfragem der Cursorposition                              *
 *                                                                           *
 * Rückgabewerte:   x und y                                                  *
 *****************************************************************************/
void CGA::getpos (int &x, int &y) {
    x = cursor_pos_x;
    y = cursor_pos_y;
}


/*****************************************************************************
 * Methode:         CGA::show                                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Anzeige eines Zeichens mit Attribut an einer bestimmten  *
 *                  Stelle auf dem Bildschirm.                               *
 *                                                                           *
 * Parameter:                                                                *
 *      x,y         Position des Zeichens                                    *
 *      character   Das auszugebende Zeichen                                 *
 *      attrib      Attributbyte fuer das Zeichen                            *
 *****************************************************************************/
void CGA::show (int x, int y, char character, unsigned char attrib) {
    char *pos = (char*)(CGA_START + 2*(x + y*COLUMNS));

    *pos = character;
    *(pos + 1) = attrib;
}


/*****************************************************************************
 * Methode:         CGA::print                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Anzeige mehrerer Zeichen ab der aktuellen Cursorposition *
 *                  '\n' fuer Zeilenvorschub.                                *
 *                                                                           *
 * Parameter:                                                                *
 *      string      Auszugebende Zeichenkette                                *
 *      n           Laenger der Zeichenkette                                 *
 *      attrib      Attributbyte fuer alle Zeichen der Zeichenkette          *
 *****************************************************************************/
void CGA::print (char* string, int n, unsigned char attrib) {
    for (int i = 0; i < n; i++) {
        char character = *(string + i);

        if (character == '\n') {
            cursor_pos_y++;
            cursor_pos_x = 0;
        } else {
            show(cursor_pos_x++, cursor_pos_y, character, attrib);
        }

        // Cursor position anpassen (ohne scroll)
        if (cursor_pos_x >= COLUMNS) {
            cursor_pos_x = 0;
            cursor_pos_y++;
        }

        while (cursor_pos_y >= ROWS) {
            cursor_pos_y--;
            scrollup();
        }

        flush_cursor_pos();
    }
}


/*****************************************************************************
 * Methode:         CGA::scrollup                                            *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Verschiebt den Bildschirminhalt um eine Zeile nach oben. *
 *                  Die neue Zeile am unteren Bildrand wird mit Leerzeichen  *
 *                  gefuellt.                                                *
 *****************************************************************************/
void CGA::scrollup () {
    char *pos = (char*)CGA_START;
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS - 1; j++) {
            *(pos + 2*((j * COLUMNS) + i)) = *(pos + 2*(((j + 1) * COLUMNS) + i));
        }

        *(pos + 2*(((ROWS - 1) * COLUMNS) + i)) = '\0';
    }
}


/*****************************************************************************
 * Methode:         CGA::clear                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Lösche den Textbildschirm.                               *
 *****************************************************************************/
void CGA::clear () {
    char *pos = (char*)CGA_START;
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS; j++) {
            *(pos + 2*((j * COLUMNS) + i)) = '\0';
        }
    }
}


/*****************************************************************************
 * Methode:         CGA::attribute                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Hilfsfunktion zur Erzeugung eines Attribut-Bytes aus     *
 *                  Hintergrund- und Vordergrundfarbe und der Angabe, ob das *
 *                  Zeichen blinkend darzustellen ist.                       *
 *                                                                           *
 * Parameter:                                                                *
 *      bg          Background color                                         *
 *      fg          Foreground color                                         *
 *      blink       ywa/no                                                   *
 *****************************************************************************/
unsigned char CGA::attribute (CGA::color bg, CGA::color fg, bool blink) {
    unsigned char a = (char)fg;
    a = a << 4;
    a = a | (bg << 1);

    if (blink) {
        a = a | 1;
    }

    return a;
}
