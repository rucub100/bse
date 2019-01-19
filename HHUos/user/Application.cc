/*****************************************************************************
 *                                                                           *
 *                         A P P L I C A T I O N                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einstieg in eine Anwendung.                              *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 21.8.2016                       *
 *****************************************************************************/

#include "kernel/Globals.h"
#include "user/Application.h"
#include "user/Loop.h"

static unsigned int stack[2][1024];
static Semaphore sem(1);

/*****************************************************************************
 * Methode:         Application::run                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Der Anwendungsthread erzeugt drei Threads die Zaehler    *
 *                  ausgeben und terminiert sich selbst.                     *
 *****************************************************************************/
void Application::run () {
    // Hauptmenu aufbauen
    kout.clear();
    
    const unsigned char attr_menu = kout.attribute(CGA::LIGHT_GREY, CGA::BLACK, false);
    const unsigned char attr_menu2 = kout.attribute(CGA::BLACK, CGA::CYAN, false);
    
    kout.clear_line(0, attr_menu);
    kout.clear_line(24, attr_menu);

    kout.print_pos(0, 0, "HHUos 1.0, Ruslan Curbanov, BSE WiSe 2018/19", attr_menu);
    
    kout.print_pos(4, 4, "[s] Sound abspielen", attr_menu2);
    kout.print_pos(4, 5, "[g] Grafikmodus", attr_menu2);
    kout.print_pos(4, 6, "[m] Speicherverwaltung", attr_menu2);
    kout.print_pos(4, 7, "[b] Bluescreentests", attr_menu2);
    kout.print_pos(4, 8, "[t] Threads und Semaphore", attr_menu2);

    while(1);
}
