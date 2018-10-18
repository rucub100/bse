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

	// einfacher Test. Es sollte links oben nach dem Bootvorgang
	// ein kleines Rechteck in weiss erscheinen (kann gelöscht werden)
	int *ptr = (int *)0xb8000;
	*ptr = -1;
	while (1) ;

    /* Hier muess Code eingefuegt werden */

    // Bildschirm loeschen.

    // Startmeldung ausgeben

    // Bildschirmausgabe testen (Strings, hex, dec, ...)

    // PC-Lautsprecher testen

    return 0;
 }
