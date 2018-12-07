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

#ifndef __LFBgraphics_include__
#define __LFBgraphics_include__

#include "devices/fonts/Fonts.h"


// Hilfsfunktionen um Farbwerte fuer einen Pixel zu erzeugen
#define RGB_24(r,g,b) (unsigned int) ((r << 16) + (g << 8) + b )


class LFBgraphics {
    
private:
    LFBgraphics (const LFBgraphics &copy);  // Verhindere Kopieren

    // Hilfsfunktion fuer drawString
    void drawMonoBitmap   (int x,  int y, unsigned int width, unsigned int height, unsigned char* bitmap, int col);

public:
    int xres, yres;         // Aufloesung in Pixel
    int bpp;                // Farbtiefe (Bits per Pixel)
    int lfb;                // Adresse des Linearen Framebuffers

    
    LFBgraphics () {}
   
    void drawPixel        (int x,  int y,  int col);
    void drawString       (Font &fnt, int x,  int y,  int col, char* str, int len);
    void drawRect         (int x, int y, int width, int height, int col);
    void drawLine         (int x1, int y1, int x2, int y2, int coll);
    void drawSprite       ();
    void drawCircle       (int x, int y, int r, int col);
};

#endif
