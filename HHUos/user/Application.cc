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
#include "user/TaskManager.h"
#include "user/Application.h"
#include "user/Loop.h"
#include "user/VBEdemo.h"
#include "user/VBEfonts.h"
#include "user/MemTest.h"
#include "user/PlayTetris.h"
#include "user/PlayAerodynamic.h"

// Task Manager
static unsigned int *tm_stack;
static TaskManager* taskManager;

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
    tm_stack = (unsigned int *) mm.mm_alloc(1024);
    taskManager = new TaskManager(&tm_stack[1024]);
    scheduler.ready(*taskManager);

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
                subMenuMemMgmt();
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
                scheduler.yield();
            break;
        }
    }
}

void Application::showMainMenu () {
    clearCenter();
    for (int i = 0; i < kout.COLUMNS - 3; i++) {
        kout.print_pos(i, 0, "=", attr_menu);
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

unsigned int Application::mmHelperSize (unsigned int size) {
    switch (size) {
        case 0:
            return 4;
        break;
        case 1:
            return 10;
        break;
        case 2:
            return 100;
        break;
        case 3:
            return 1000;
        break;
        case 4:
            return 4000;
        break;
        case 5:
            return 500000;
        break;
        case 6:
            return 5000000;
        break;
        default:
            return 100;
        break;
    }
}

void Application::subMenuMemMgmt_2 (int pos, unsigned int size) {
    char* s_pos = " ";
    s_pos[0] = pos + '0';
    char* s_size = "XXXYY";

    switch (size) {
        case 0:
            s_size[0] = ' ';
            s_size[1] = ' ';
            s_size[2] = '4';
            s_size[3] = 'B';
            s_size[4] = ' ';
        break;
        case 1:
            s_size[0] = ' ';
            s_size[1] = '1';
            s_size[2] = '0';
            s_size[3] = 'B';
            s_size[4] = ' ';
        break;
        case 2:
            s_size[0] = '1';
            s_size[1] = '0';
            s_size[2] = '0';
            s_size[3] = 'B';
            s_size[4] = ' ';
        break;
        case 3:
            s_size[0] = '1';
            s_size[1] = '0';
            s_size[2] = '0';
            s_size[3] = '0';
            s_size[4] = 'B';
        break;
        case 4:
            s_size[0] = ' ';
            s_size[1] = ' ';
            s_size[2] = '4';
            s_size[3] = 'K';
            s_size[4] = 'B';
        break;
        case 5:
            s_size[0] = '5';
            s_size[1] = '0';
            s_size[2] = '0';
            s_size[3] = 'K';
            s_size[4] = 'B';
        break;
        case 6:
            s_size[0] = ' ';
            s_size[1] = ' ';
            s_size[2] = '5';
            s_size[3] = 'M';
            s_size[4] = 'B';
        break;
        default:
            s_size[0] = ' ';
            s_size[1] = ' ';
            s_size[2] = '?';
            s_size[3] = 'B';
            s_size[4] = ' ';
        break;
    }

    kout.print_pos(kout.COLUMNS - 20, kout.ROWS - 4, "[backspace] Zuruck", attr_menu2);
    kout.print_pos(kout.COLUMNS - 30, 4, "[f] Freispeicherliste", attr_menu2);
    kout.print_pos(kout.COLUMNS - 30, 5, "[b] Belegte Bloecke", attr_menu2);
    kout.print_pos(kout.COLUMNS - 30, 8, "[0-9] Position: ", attr_menu2);
    kout.print_pos(kout.COLUMNS - 14, 8, s_pos, attr_menu2);
    kout.print_pos(kout.COLUMNS - 30, 9, "[i] Speicher anlegen", attr_menu2);
    kout.print_pos(kout.COLUMNS - 30, 10, "[d] Speicher freigeben", attr_menu2);
    kout.print_pos(kout.COLUMNS - 30, 11, "[s] Groesse: ", attr_menu2);
    kout.print_pos(kout.COLUMNS - 17, 11, s_size, attr_menu2);
}

void Application::subMenuMemMgmt () {
    clearCenter();

    bool show_free = false;
    unsigned int s = 0;
    unsigned int* p = (unsigned int*) mm.mm_alloc(sizeof(unsigned int*) * 10);
    for (int i = 0; i < 10; i++) { p[i] = 0; }
    int pos = 0;

    subMenuMemMgmt_2(pos, s);

    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            print_free:
            case 'f':
                clearCenter();
                kout.get_lock()->acquire();
                kout.setpos(0, 1);
                kout << "Freispeicherliste:" << endl;
                mm.mm_dump_free_list();
                kout.get_lock()->free();
                subMenuMemMgmt_2(pos, s);
                show_free = true;
            break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                pos = last_key - '0';
                subMenuMemMgmt_2(pos, s);
            break;
            print_used:
            case 'b':
                clearCenter();
                kout.get_lock()->acquire();
                kout.setpos(0, 1);
                kout << "Belegte Bloecke:  " << endl;
                mm.mm_dump_used_list();
                kout.get_lock()->free();
                subMenuMemMgmt_2(pos, s);
                show_free = false;
            break;
            case 'i':
                if (p[pos] != 0) {
                    delete (void*) p[pos];
                }

                p[pos] = (unsigned int) mm.mm_alloc(mmHelperSize(s));
                if (show_free) {
                    goto print_free;
                } else {
                    goto print_used;
                }
            break;
            case 'd':
                if (p[pos] != 0) {
                    delete (void*) p[pos];
                    p[pos] = 0;
                }
                if (show_free) {
                    goto print_free;
                } else {
                    goto print_used;
                }
            break;
            case 's':
                s = (s + 1) % 7;
                subMenuMemMgmt_2(pos, s);
            break;
            case '\b':
                goto back;
            default:
            break;
        }
    }

    back:

    for (int i = 0; i < 10; i++) {
        if (p[i] != 0) {
            delete (void*) p[i];
            p[i] = 0;
        } 
    }

    delete p;

    showMainMenu();
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
    showSubMenuSeparator();
    kout.print_pos((kout.COLUMNS / 2) + 4, 4, "[s] Synchronisiert (Semaphore)", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 5, "[a] Asynchron", attr_menu2);
    kout.print_pos((kout.COLUMNS / 2) + 4, 6, "[backspace] Zuruck", attr_menu2);

    while (1) {
        char last_key = kb.pop_last_key();
        switch (last_key) {
            case 's':
                subMenuThreadsSemaphore_sync();
                goto back;
            break;
            case 'a':
                subMenuThreadsSemaphore_async();
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

void Application::subMenuThreadsSemaphore_sync () {
    clearCenter();
    unsigned int* stack1 = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack2 = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack3 = (unsigned int *) mm.mm_alloc(1024);
    Semaphore* sem = new Semaphore(1);

    
    Loop* loop1 = new Loop(
        &(stack1)[1024],
        10,
        kout.ROWS / 2,
        sem);

    Loop* loop2 = new Loop(
        &(stack2)[1024],
        10 + (kout.COLUMNS / 2),
        kout.ROWS / 2,
        sem);

    PlayTetris* playTetris = new PlayTetris(&stack3[1024]);
    
    scheduler.ready(*loop1);
    scheduler.ready(*loop2);
    //scheduler.ready(*playTetris);

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

    //scheduler.kill(*playTetris);
    pcspk.off();
    delete playTetris;
    delete stack3;

    delete loop1;
    delete loop2;
    delete stack1;
    delete stack2;
    delete sem;
}

void Application::subMenuThreadsSemaphore_async () {
    scheduler.kill(*taskManager);
    clearCenter();
    unsigned int* stack1 = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack2 = (unsigned int *) mm.mm_alloc(1024);
    unsigned int* stack3 = (unsigned int *) mm.mm_alloc(1024);

    
    Loop* loop1 = new Loop(
        &(stack1)[1024],
        10,
        kout.ROWS / 2,
        0);

    Loop* loop2 = new Loop(
        &(stack2)[1024],
        10 + (kout.COLUMNS / 2),
        kout.ROWS / 2,
        0);

    PlayTetris* playTetris = new PlayTetris(&stack3[1024]);
    
    scheduler.ready(*loop1);
    scheduler.ready(*loop2);
    //scheduler.ready(*playTetris);

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

    //scheduler.kill(*playTetris);
    pcspk.off();
    delete playTetris;
    delete stack3;

    delete loop1;
    delete loop2;
    delete stack1;
    delete stack2;

    scheduler.ready(*taskManager);
    showMainMenu();
}