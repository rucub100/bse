/*****************************************************************************
 *                                                                           *
 *                              M E M M G M T                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einfache Speicherverwaltung. 'new' und 'delete' werden   *
 *                  durch Ueberladen der entsprechenden Operatoren           *
 *                  realisiert.                                              *
 *                                                                           *
 * Memory-Laylout                                                            *
 *                                                                           *
 *                  boot.asm                                                 *
 *                      0x07c0: Bootsector vom BIOS geladen                  *
 *                      0x0060: Boot-Code verschiebt sich hier hin           *
 *                      0x9000: Setup-Code (max. 64K inkl. Stack) vom        *
 *                              Bootsector-Code geladen                      *
 *                  setup.asm                                                *
 *                      0x1000: System-Code (max. 512K) geladen              *
 *                  BIOS-Aufruf                                              *
 *                     0x24000: Parameter fuer BIOS-Aufurf                   *
 *                     0x25000: Altes ESP sichern, vor BIOS-Aufruf           *
 *                     0x26000: 16-Bit Code-Segment fuer BIOS-Aufurf         *
 *                  System-Code                                              *
 *                    0x100000:	System-Code, kopiert nach Umschalten in      *
 *                              den Protected Mode kopiert (GRUB kann nur    *
 *                              an Adressen >1M laden)                       *
 *           Globale Variablen: Direkt nach dem Code liegen die globalen     *
 *                              Variablen.                                   *
 *                        Heap:                                              *
 *                    0x200000:	Start-Adresse der Heap-Verwaltung            *
 *                        Ende: Letzte Adresse des phys. Speichers           *
 *                                                                           *
 * Achtung:         Benötigt einen PC mit mindestens 6 MB RAM!               *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 27.12.2016                      *
 *****************************************************************************/

#include <stddef.h>
#include "kernel/Globals.h"
#include "kernel/MemMgmt.h"

/*****************************************************************************
 * Prozedur:        mm_init                                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Liste der Freispeicherbloecke intitialisieren.           *
 *                  Anker zeigt auf ein Dummy-Element. Danach folgt          *
 *                  ein Block der den gesamten freien Speicher umfasst.      *
 *                                                                           *
 *                  Wird automatisch aufgerufen, sobald eine Funktion der    *
 *                  Speicherverwaltung erstmalig gerufen wird.               *
 *****************************************************************************/

void MemMgmt::mm_init() {
    // mem_size ist in Globals definiert!
    total_mem = MEM_SIZE_DEF;
    
    // Speicherausbau abfragen
    BC_params->AX = 0xE801;
    BC_params->CX = 0;
    BC_params->DX = 0;
    bios.Int(0x15);

    // Problem?
    if ( (BC_params->AX & 0xFF) == 0x86 || (BC_params->AX & 0xFF) == 0x80) {
        kout << "Kernel panic: Speicherausbau konnte vom BIOS nicht erfragt werden." << endl;
        kout << "(Verwende Default-Heap-Groesse: )" << (MEM_SIZE_DEF/(1024*1024)) << " MB" << endl;
        mm_heap_size = MEM_SIZE_DEF;
    } else {
        // verfuegbaren Speicher insgesamt berechnen
        total_mem = 1024 * 1024;                     // 1. MB
        total_mem += (BC_params->CX * 1024);         // 2 - 16MB
        total_mem += (BC_params->DX * 64 * 1024);    // >16MB
        
        // Startadresse abziehen
        mm_heap_size = total_mem - MEM_START;

        mm_alloc_cnt = 0;
/*
        kout << "BIOS call war erfolgteich" << endl;
        kout << "   CX=" << BC_params->CX << endl;
        kout << "   DX=" << BC_params->DX << endl;
        kout << "   mem_size=" << mem_size << endl;
*/
    }
 
    // Frei-Liste initialisieren
    free_list = (FreeBlockMeta*)MEM_START;
    free_list->len = mm_heap_size - sizeof(unsigned int); // Speicherplatz für next kann belegt werden!
    free_list->next = 0;
    next_fit = free_list;

    // Speicherverwaltung ist initialisiert
    mm_initialized = 1;
}


/*****************************************************************************
 * Prozedur:        mm_dump_free_list                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ausgabe der Freispeicherliste. Zu Debuggingzwecken.      *
 *****************************************************************************/
void MemMgmt::mm_dump_free_list() {
    FreeBlockMeta* tmp = free_list;
    unsigned int i = 0;
    while (tmp != 0) {
        kout << dec << i << ": @" << tmp << "; len=" << tmp->len << endl;
        i++;
        tmp = tmp->next;
    }
}

void MemMgmt::mm_dump_used_list() {
    FreeBlockMeta* f = free_list;
    FreeBlockMeta* s = (FreeBlockMeta*)MEM_START;
    FreeBlockMeta* u = 0;

    if (f == s) {
        while (f->next != 0 && (char*)f->next == (char*)f + f->len + 4) {
            f = f->next;
        }
        
        if (f == 0) {
            return;
        } else {
            u = (FreeBlockMeta*)((char*)f + f->len + 4);
            f = f->next;
        }
    } else {
        u = s;
    }

    unsigned int i = 0;
    while (u != 0) {
        kout << dec << i << ": @" << u << "; len=" << u->len << endl;
        i++;
        if ((char*)u + u->len + 4 < (char*)(MEM_START + mm_heap_size)) {
            if ((char*)u + u->len + 4 < (char*)f || f == 0) {
                u = (FreeBlockMeta*)((char*)u + u->len + 4);
            } else {
                while (f->next != 0 && (char*)f->next == (char*)f + f->len + 4) {
                    f = f->next;
                }

                if (f == 0) {
                    u = 0;
                } else {
                    u = (FreeBlockMeta*)((char*)f + f->len + 4);
                    f = f->next;
                }
            }
        } else {
            u = 0;
        }
    }
}


/*****************************************************************************
 * Funktion:        mm_alloc                                                 *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen neuen Speicherblock allozieren.                    * 
 *****************************************************************************/
void* MemMgmt::mm_alloc(unsigned int req_size) {
    void* mem = 0;
        
    // Noch nicht initialisiert?
    if (mm_initialized==0)
        mm_init();

    // 4Byte alignierte Belegung!
    if (req_size < MIN_USED_BLOCK_SIZE) {
        req_size = MIN_USED_BLOCK_SIZE; // Damit nach delete min sizeof(FreeBlockMeta) 
        /**
         * Bei 4 Byte + 4Byte für len wird nach delete ein FreeBlockMeta Objekt
         * mit len=0 erzeugt! Deshalb könnte man bei delete die anliegenden freien
         * Blöcke verschmelzen. Sollten keine freien Blöcke direkt anliegen,
         * so kann man eine Defragmentierung implementieren (z.B. im IDLE Zustand).
        */
    } else {
        if (req_size % 4 != 0) {
            req_size += (4 - (req_size % 4));
        }
    }
 
    /* Next Fit Implementierung */
    // Finde nächsten passenden freien Block
    while (next_fit->len < req_size && 
        (unsigned int)next_fit->next >= MEM_START) {
            next_fit = next_fit->next;
    }

    // Falls nicht gefunden, wiederhole Suche einmalig ab Anfang
    if (next_fit->len < req_size) {
        next_fit = free_list;
        while (next_fit->len < req_size && 
        (unsigned int)next_fit->next >= MEM_START) {
            next_fit = next_fit->next;
        }
    }

    // Gefunden?
    if (next_fit->len >= req_size) {
        // Genug platz um aufzuspalten?
        if (next_fit->len >= MIN_FREE_BLOCK_SIZE + (req_size + 4)) {
            // belege Speicher
            next_fit->len -= (req_size + 4);
            mem = (char*)next_fit + next_fit->len + 4;
            *((unsigned int*)mem) = req_size; // setze allozierte Länge
            mem = (unsigned int*)mem + 1; // überspringe Längenangabe
        } else { // Nein -> Freier Block wird komplett belegt => Zeiger umbiegen
            if (next_fit == free_list) {
                free_list = free_list->next;
            } else {
                // Finde parent
                FreeBlockMeta* parent = free_list;
                
                while (parent->next != next_fit) {
                    parent = parent->next;
                }

                parent->next = next_fit->next;
            }

            // Freien Block in belegten Block "umwandeln"
            // next_fit->len += 4; NICHT ervorderlich, da len in FreeBlockMeta den Platz für next mitberechnet
            mem = (unsigned int*)next_fit + 1;
        }
    } else {
        // fatal error -> no ram space
        cpu.disable_int();
        kout.clear();
        kout << "FATAL: NO RAM SPACE LEFT!!!" << endl;
        cpu.halt();
    }

    // Update next_fit
    if ((unsigned int)next_fit->next >= MEM_START) {
        next_fit = next_fit->next;
    } else {
        next_fit = free_list;
    }   

    mm_alloc_cnt++;
    return mem;
}


/*****************************************************************************
 * Funktion:        mm_free                                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen Speicherblock freigeben.                           *
 *****************************************************************************/
void MemMgmt::mm_free(void *ptr) {
    if ((char*)ptr < (char*)MEM_START) {
        // TODO: Bluescreen?!
    }

    // Finde letzten freien Block VOR dem belegten Block
    FreeBlockMeta* prev = free_list;

    while (prev < ptr && 
        (unsigned int)prev->next >= MEM_START &&
        prev->next < ptr) {
            prev = prev->next;
    }

    FreeBlockMeta* tmp = (FreeBlockMeta*)((unsigned int*)ptr - 1);

    if (prev == free_list) {
        // Belegter Block VOR dem ersten FREE Eintrag?
        if (prev < ptr) {
            // Verschmelze mit vorangegangenen Freien Block, 
            // falls sich keine weiteren belegten Blöcke dazwischen befinden
            if (ptr == (char*)prev + prev->len + 8) {
                prev->len += *((unsigned int*)ptr - 1) + 4;
                // Falls nun der nächste freie Block ebenfalls anliegt,
                // dann verschmelze auch diesen
                if ((char*)prev->next == (char*)prev + prev->len + 4) {
                    prev->len += prev->next->len + 4;
                    prev->next = prev->next->next;
                }
            } else {
                tmp->next = prev->next;
                prev->next = tmp;

                if ((char*)tmp->next == (char*)tmp + tmp->len + 4) {
                    tmp->len += tmp->next->len + 4;
                    tmp->next = tmp->next->next;
                }
            }
        } else { // Neuer Anfang der free_list
            tmp->next = prev;
            free_list = tmp;
        }
    } else {
        // Unterscheiden, ob prev am Ende der free_list ist
        if ((char*)prev->next < (char*)ptr) {
            // Falls ptr direkt an prev anliegt, dann vergroessere prev
            if ((char*)ptr == (char*)prev + prev->len + 8) {
                prev->len += tmp->len + 4;
            } else {
                tmp->next = 0;
                prev->next = tmp;
            }
        } else { // Block liegt zwischen prev und prev->next
            // Falls ptr direkt an prev anliegt, dann vergroessere prev
            if ((char*)ptr == (char*)prev + prev->len + 8) {
                prev->len += tmp->len + 4;

                if ((char*)prev->next == (char*)prev + prev->len + 4) {
                    prev->len += prev->next->len + 4;
                    prev->next = prev->next->next;
                }
            } else if ((char*)tmp + tmp->len + 4 == (char*)prev->next) { // oder falls an ptr an prev->next anliegt
                tmp->len += prev->next->len + 4;
                tmp->next = prev->next;
                prev->next = tmp;
            } else {
                tmp->next = prev->next;
                prev->next = tmp;
            }
        }
    }

    mm_alloc_cnt--;
}


/*****************************************************************************
 * Nachfolgend sind die Operatoren von C++, die wir hier ueberschreiben      *
 * und entsprechend 'mm_alloc' und 'mm_free' aufrufen.                       *
 *****************************************************************************/
void* operator new ( size_t size ) {
     return mm.mm_alloc(size);
}

void* operator new[]( size_t count ) {
    return mm.mm_alloc(count);
}

void operator delete ( void* ptr )  {
    mm.mm_free(ptr);
}
 
void operator delete[] ( void* ptr ) {
    mm.mm_free(ptr);
}
