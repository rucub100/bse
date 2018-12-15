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

#ifndef __MemMgmt_include__
#define __MemMgmt_include__


#define MEM_START           0x200000        // Startadresse des Heaps
#define MEM_SIZE_DEF        4*1024*1024     // Default-Groesse des Heaps
#define MIN_FREE_BLOCK_SIZE 64              // min. Groesse eines freien Blocks (Strategie beim Allozieren)
#define MIN_USED_BLOCK_SIZE 12 //?


struct FreeBlockMeta {
    unsigned int len;
    FreeBlockMeta* next;
};

class MemMgmt {
    private:
        MemMgmt (const MemMgmt &copy);

        // zeigt an, ob die Speicherverwaötung initialisiert wurde.
        unsigned int mm_initialized;
        
        FreeBlockMeta* free_list;
        FreeBlockMeta* next_fit;

        // max. Groesse des Heaps
        unsigned int mm_heap_size;
        unsigned int mm_alloc_cnt;
     
    public:
        MemMgmt () : mm_initialized(0) {}

        // Initialisieren der Speicherverwaltung
        void mm_init();

        void* mm_alloc(unsigned int req_size);
        void mm_free(void *ptr);

        // Ausgabe des Heap-Zustands
        void mm_dump_free_list();

        void mm_usage(unsigned int& avail, unsigned int& meta, unsigned int& used) { 
            avail = 0;
            used = mm_heap_size;
            meta = mm_alloc_cnt * 4;
    
            if (free_list == 0) {
                used -= meta;
                return;
            }

            FreeBlockMeta* acc = free_list;

            do {
                avail += acc->len;
                acc = acc->next;
                meta += 4; // Zähle nicht 4Byte für next, damit Summe aufgeht!
            } while (acc >= (void*) MEM_START);

            used -= avail;
            used -= meta;
        }
};

#endif
