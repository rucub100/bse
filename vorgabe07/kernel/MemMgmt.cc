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


// zeigt an, ob die Speicherverwaötung initialisiert wurde.
unsigned int mm_initialized=0;

// max. Groesse des Heaps
unsigned int mm_heap_size=0;


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
void mm_init() {
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
/*
        kout << "BIOS call war erfolgteich" << endl;
        kout << "   CX=" << BC_params->CX << endl;
        kout << "   DX=" << BC_params->DX << endl;
        kout << "   mem_size=" << mem_size << endl;
*/
    }
 
    
    // Speicherverwaltung ist initialisiert
    mm_initialized = 1;
}


/*****************************************************************************
 * Prozedur:        mm_dump_free_list                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ausgabe der Freispeicherliste. Zu Debuggingzwecken.      *
 *****************************************************************************/
void mm_dump_free_list() {

    /* hier muss Code eingefuegt werden */
    
}


/*****************************************************************************
 * Funktion:        mm_alloc                                                 *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen neuen Speicherblock allozieren.                    * 
 *****************************************************************************/
void * mm_alloc(unsigned int req_size) {
   
    kout << "   mm_alloc: req_size=" << dec << req_size << "B";
    
    
    // Noch nicht initialisiert?
    if (mm_initialized==0)
        mm_init();
    
 
    /* hier muss Code eingefuegt werden */

    return (void*) 0;
}


/*****************************************************************************
 * Funktion:        mm_free                                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen Speicherblock freigeben.                           *
 *****************************************************************************/
void mm_free(void *ptr) {
    kout << "   mm_free: ptr= " << hex << (unsigned int)ptr << endl;

    /* hier muss Code eingefuegt werden */

}


/*****************************************************************************
 * Nachfolgend sind die Operatoren von C++, die wir hier ueberschreiben      *
 * und entsprechend 'mm_alloc' und 'mm_free' aufrufen.                       *
 *****************************************************************************/
void* operator new ( size_t size ) {
     return mm_alloc(size);
}

void* operator new[]( size_t count ) {
    return mm_alloc(count);
}

void operator delete ( void* ptr )  {
    mm_free(ptr);
}
 
void operator delete[] ( void* ptr ) {
    mm_free(ptr);
}
