/*****************************************************************************
 *                                                                           *
 *                         A P P L I C A T I O N                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einstieg in eine Anwendung.                              *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 15.8.2016                       *
 *****************************************************************************/
#ifndef __application_include__
#define __application_include__


class Application {
    
private:
    Application (const Application &copy); // Verhindere Kopieren

public:
                 
    Application () {}
         
    void main ();
 };

#endif
