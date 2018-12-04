/*****************************************************************************
 *                                                                           *
 *                            L F B G R A P H I C S                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zeichenfunktionen fuer Grafikmodi, die auf einem         *
 *                  linearen Framebuffer basieren. Verwendet in VESA und     *
 *                  QemuVGA.                                                 *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 19.9.2016                       *
 *****************************************************************************/

#include "devices/LFBgraphics.h"

/*****************************************************************************
 * Methode:         LFBgraphics::drawMonoBitmap                              *
 *---------------------------------------------------------------------------*
 * Parameter:       x,y     Startpunkt ab dem Text ausgegeben wird.          *
 *                  width   Breite in Pixel                                  *
 *                  height  Hoehe in Pixel                                   *
 *                  bitmap  Zeiger auf Pixel der monochromen Rastergrafik    *
 *                  col     Farbe der Pixel                                  *
 *                                                                           *
 * Beschreibung:    Gibt die gegebene monochrome Rastergrafik an der Position*
 *                  x,y zeilenweise aus. (x,y) ist der linke obere Punkt;    *
 *                  ist in der bitmap eine '1', so wird ein Pixel mit der    *
 *                  Farbe col ausgegeben, ansonsten bei '0' nichts.          *
 *                  Diese Funktion basiert auf dem Format der Fonts, welche  *
 *                  mit cpi2fnt (AmigaOS) erzeugt wurden. Das Format erklaert*
 *                  sich in den C-Dateien in fonts/ von selbst.              *
 *****************************************************************************/
inline void LFBgraphics::drawMonoBitmap(int x, int y,
                                        unsigned int width,
                                        unsigned int height,
                                        unsigned char* bitmap, int color) {
    // Breite in Bytes
    unsigned short width_byte = width/8 + ((width%8 != 0) ? 1 : 0);
    
    for(unsigned int yoff=0; yoff<height; ++yoff) {
        int xpos=x;
        int ypos=y+yoff;
        for(unsigned int xb=0; xb < width_byte; ++xb) {
            for(int src=7; src>=0; --src) {
                if ((1 << src) & *bitmap) {
                    drawPixel(xpos, ypos, color);
                }
                xpos++;
            }
            bitmap++;
        }
    }
}


/*****************************************************************************
 * Methode:         LFBgraphics::drawString                                  *
 *---------------------------------------------------------------------------*
 * Parameter:       fnt     Schrift                                          *
 *                  x,y     Startpunkt ab dem Text ausgegeben wird.          *
 *                  col     Farbe des Textes                                 *
 *                  str     Zeiger auf Zeichenkette                          *
 *                  len     Laenge der Zeichenkette                          *
 *                                                                           *
 * Beschreibung:    Gibt eine Zeichenkette mit gewaehlter Schrift an der     *
 *                  Position x,y aus.                                        *
 *****************************************************************************/
void LFBgraphics::drawString(Font &fnt, int x,  int y,  int col,
                             char* str, int len) {
    
    for(int i = 0; i < len; ++i) {
        drawMonoBitmap(x, y, fnt.get_char_width(), fnt.get_char_height(),
                       fnt.getChar( *(str+i) ), col);
        x += fnt.get_char_width();
    }
}


/*****************************************************************************
 * Methode:         LFBgraphics::drawPixel                                   *
 *---------------------------------------------------------------------------*
 * Parameter:       x, y    Koordinaten des Pixels                           *
 *                  col     Farbe                                            *
 *                                                                           *
 * Beschreibung:    Zeichnen eines Pixels.                                   *
 *****************************************************************************/
void LFBgraphics::drawPixel(int x, int y, int col) {
    unsigned char *ptr;
    
    // Pixel ausserhalb des sichtbaren Bereichs?
    if (x<0 || x>=xres || y<0 || y>yres)
        return;
    
    // Adresse des Pixels berechnen und Inhalt schreiben
    switch (bpp) {
        case 8:
            ptr = (unsigned char *)lfb+x+y*xres;
            *ptr = col;
            return;
        case 15:
        case 16:
            ptr = (unsigned char *)lfb+2*x+2*y*xres;
            *ptr = col;
            return;
        case 24:
            ptr = (unsigned char *)lfb+3*x+3*y*xres;
            *ptr = (col & 0xFF); ptr++;
            *ptr = ((col>>8) & 0xFF); ptr++;
            *ptr = ((col>>16) & 0xFF); ptr++;
            return;
        case 32:
            ptr = (unsigned char *)lfb+4*x+4*y*xres;
            *ptr = (col & 0xFF); ptr++;
            *ptr = ((col>>8) & 0xFF); ptr++;
            *ptr = ((col>>16) & 0xFF); ptr++;
            return;
    }
}
