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
#include "user/VBEdemo.h"
#include "user/VBEfonts.h"
#include "user/MemTest.h"
#include "user/PlayTetris.h"
#include "user/PlayAerodynamic.h"


const unsigned char attr_menu = kout.attribute(CGA::LIGHT_GREY, CGA::BLACK, false);
const unsigned char attr_menu2 = kout.attribute(CGA::BLACK, CGA::CYAN, false);
const unsigned char attr_menu3 = kout.attribute(CGA::CYAN, CGA::LIGHT_CYAN, true);
const unsigned char attr_menu4 = kout.attribute(CGA::BLACK, CGA::CYAN, true);
    

/*****************************************************************************
 * Methode:         Application::run                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Der Anwendungsthread erzeugt drei Threads die Zaehler    *
 *                  ausgeben und terminiert sich selbst.                     *
 *****************************************************************************/
void Application::run () {
    // Hauptmenu aufbauen
    kout.clear();
    showMainMenu();
                
    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            case 's':
                kout.print_pos(4, 4, "[s] Sound abspielen", attr_menu3);
                subMenuPlaySound();
            break;
            case 'g':
                kout.print_pos(4, 5, "[g] Grafikmodus", attr_menu3);
                subMenuVBE();
            break;
            case 'm':
                kout.print_pos(4, 6, "[m] Speicherverwaltung", attr_menu3);
            break;
            case 'b':
                kout.print_pos(4, 7, "[b] Bluescreentests", attr_menu3);
                subMenuBluescreentests();
            break;
            case 't':
                kout.print_pos(4, 8, "[t] Threads und Semaphore", attr_menu3);
                subMenuThreadsSemaphore();
            break;
            default:
            break;
        }
    }
}

void Application::showMainMenu () {
    clearCenter();
    for (int i = 0; i < kout.COLUMNS - 3; i++) {
        kout.print_pos(i, 0, " ", attr_menu);
    }
    kout.print_pos(0, 0, "HHUos 1.0, Ruslan Curbanov, BSE WiSe 2018/19", attr_menu);
    kout.print_pos(4, 4, "[s] Sound abspielen", attr_menu2);
    kout.print_pos(4, 5, "[g] Grafikmodus", attr_menu2);
    kout.print_pos(4, 6, "[m] Speicherverwaltung", attr_menu2);
    kout.print_pos(4, 7, "[b] Bluescreentests", attr_menu2);
    kout.print_pos(4, 8, "[t] Threads und Semaphore", attr_menu2);
}

void Application::clearCenter () {
    for (int i = 1; i < kout.ROWS - 1; i++) {
        kout.clear_line(i, kout.STD_ATTR);
    }
}

void Application::showSubMenuSeparator () {
    for (int i = 2; i < kout.ROWS - 2; i++) {
        kout.print_pos(kout.COLUMNS / 2, i, ">", attr_menu2);
    }
}

void Application::subMenuBluescreentests () {
    showSubMenuSeparator ();

    kout.print_pos((kout.COLUMNS / 2) + 4, 7, "[n] Null-Pointer", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 8, "[w] Write-Protection (Code 3)", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 9, "[p] Adresse ausserhalb (Code 2)", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 10, "[backspace] Zuruck", attr_menu2);

    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            case 'n':
                *((unsigned int*) 0x0) = 0;
            break;
            case 'w':
                pg_write_protect_page((unsigned int*) 0x203000);
                *((unsigned int*) 0x203000) = 0;
            break;
            case 'p':
                *((unsigned long long*) 0xFFFFFFFFFFFF) = 0;
            break;
            case '\b':
                goto back;
            break;
            default:
            break;
        }
    }
    back:

    showMainMenu();
}

void Application::subMenuPlaySound () {
    bool play_tetris = false;
    bool play_aerodynamic = false;
    PlayTetris* tetris;
    PlayAerodynamic* aerodynamic;
    unsigned int* stack_tetris = ((unsigned int *) mm.mm_alloc(1024));
    unsigned int* stack_aero = ((unsigned int *) mm.mm_alloc(1024));

    showSubMenuSeparator();
    kout.print_pos((kout.COLUMNS / 2) + 4, 4, "[1] Tetris", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 5, "[2] Aerodynamic", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 6, "[backspace] Zuruck", attr_menu2);

    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            case '1':
                if (play_tetris) {
                    scheduler.kill(*tetris);
                    pcspk.off();
                    delete tetris;
                    kout.print_pos((kout.COLUMNS / 2) + 4, 4, "[1] Tetris", attr_menu2);
                } else {
                    kout.print_pos((kout.COLUMNS / 2) + 4, 4, "[1] Tetris", attr_menu3);
                    if (play_aerodynamic) {
                        scheduler.kill(*aerodynamic);
                        pcspk.off();
                        delete aerodynamic;
                        play_aerodynamic = false;
                        kout.print_pos((kout.COLUMNS / 2) + 4, 5, "[2] Aerodynamic", attr_menu2);
                    }
                    tetris = new PlayTetris(&stack_tetris[1024]);
                    scheduler.ready(*tetris);
                }
                play_tetris = !play_tetris;
            break;
            case '2':
                if (play_aerodynamic) {
                    scheduler.kill(*aerodynamic);
                    pcspk.off();
                    delete aerodynamic;
                    kout.print_pos((kout.COLUMNS / 2) + 4, 5, "[2] Aerodynamic", attr_menu2);
                } else {
                    kout.print_pos((kout.COLUMNS / 2) + 4, 5, "[2] Aerodynamic", attr_menu3);
                    if (play_tetris) {
                        scheduler.kill(*tetris);
                        pcspk.off();
                        delete tetris;
                        play_tetris = false;
                        kout.print_pos((kout.COLUMNS / 2) + 4, 4, "[1] Tetris", attr_menu2);
                    }
                    aerodynamic = new PlayAerodynamic(&stack_aero[1024]);
                    scheduler.ready(*aerodynamic);
                }
                play_aerodynamic = !play_aerodynamic;
            break;
            case '\b':
                goto back;
            break;
            default:
            break;
        }
    }

    back:

    if (play_aerodynamic) {
        scheduler.kill(*aerodynamic);
        pcspk.off();
        delete aerodynamic;
        play_aerodynamic = false;
    }

    if (play_tetris) {
        scheduler.kill(*tetris);
        pcspk.off();
        delete tetris;
        play_tetris = false;
    }

    delete stack_tetris;
    delete stack_aero;

    showMainMenu();
}

void Application::subMenuVBE () {
    unsigned int* stack_demo = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack_fonts = (unsigned int *) mm.mm_alloc(1024);
    VBEdemo* vbe_demo = new VBEdemo(&stack_demo[1024]);
    VBEfonts* vbe_fonts = new VBEfonts(&stack_fonts[1024]);
    
    showSubMenuSeparator();
    kout.print_pos((kout.COLUMNS / 2) + 4, 5, "[b] Bitmap-Demo", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 6, "[f] Fonts", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 7, "[backspace] Zuruck", attr_menu2);

    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            case 'b':
                scheduler.ready(*vbe_demo);
                scheduler.exit();
            break;
            case 'f':
                scheduler.ready(*vbe_fonts);
                scheduler.exit();
            break;
            case '\b':
                goto back;
            break;
            default:
            break;
        }
    }

    back:

    delete stack_demo;
    delete stack_fonts;
    delete vbe_demo;
    delete vbe_fonts;

    showMainMenu();
}

void Application::subMenuThreadsSemaphore () {
    clearCenter();
    Semaphore* semaphore = new Semaphore(1);
    unsigned int* stack1 = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack2 = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack3 = (unsigned int *) mm.mm_alloc(1024);


    Loop* loop1 = new Loop(
        &(stack1)[1024],
        10,
        kout.ROWS / 2,
        semaphore);

    Loop* loop2 = new Loop(
        &(stack2)[1024],
        10 + (kout.COLUMNS / 2),
        kout.ROWS / 2,
        semaphore);

    PlayTetris* playTetris = new PlayTetris(&stack3[1024]);
    
    scheduler.ready(*loop1);
    scheduler.ready(*loop2);
    scheduler.ready(*playTetris);

    kout.print_pos(kout.COLUMNS - 20, kout.ROWS - 4, "[backspace] Zuruck", attr_menu4);

    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            case '\b':
                scheduler.kill(*loop1);
                scheduler.kill(*loop2);
                goto back;
            default:
            break;
        }
    }

    back:

    scheduler.kill(*playTetris);
    pcspk.off();
    delete playTetris;
    delete stack3;

    delete loop1;
    delete loop2;
    delete stack1;
    delete stack2;
    delete semaphore;

    showMainMenu();
}