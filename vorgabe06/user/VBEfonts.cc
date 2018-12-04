/*****************************************************************************
 *                                                                           *
 *                            V B E F O N T S                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Font demo.                                               *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 25.12.2016                      *
 *****************************************************************************/

#include "kernel/Globals.h"
#include "user/VBEfonts.h"
#include "devices/fonts/Fonts.h"


/*****************************************************************************
 * Methode:         VBEfonts::run                                            *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Der Anwendungsthread gibt Text mit den verfuegbaren      * 
 *                  Schriften aus.                                           *
 *****************************************************************************/
void VBEfonts::run () {
    // In den Grafikmodus schalten (32-Bit Farbtiefe)
    vesa.init(MODE_640_480_24BITS);

    vesa.drawString(std_font_8x8, 10,  50,  RGB_24(0,255,0), "std_font_8x8", 12 );
    vesa.drawString(std_font_8x16, 10,  70,  RGB_24(255,0,0), "std_font_8x16", 13 );
    vesa.drawString(acorn_font_8x8, 10,  100,  RGB_24(0,0,255), "acorn_font_8x8", 14 );
    vesa.drawString(pearl_font_8x8, 10,  120,  RGB_24(0,255,255), "pearl_font_8x8", 14 );
    vesa.drawString(sun_font_12x22, 10,  140,  RGB_24(255,0,255), "sun_font_12x22", 14 );
    vesa.drawString(sun_font_8x16, 10,  170,  RGB_24(255,255,0), "sun_font_8x16", 13 );

    while (1) ;
    
    // selbst terminieren
    //scheduler.exit ();
}
