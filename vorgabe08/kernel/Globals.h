/*****************************************************************************
 *                                                                           *
 *                               G L O B A L S                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Globale Variablen des Systems.                           *
 *                                                                           *
 * Autor:           Michael Schoettner, 27.12.2016                           *
 *****************************************************************************/
#ifndef __Globals_include__
#define __Globals_include__

#include "kernel/CPU.h"
#include "kernel/interrupts/PIC.h"
#include "kernel/threads/Scheduler.h"
#include "kernel/interrupts/IntDispatcher.h"
#include "kernel/BIOS.h"
#include "kernel/MemMgmt.h"
#include "devices/PCSPK.h"
#include "devices/CGA_Stream.h"
#include "devices/Keyboard.h"
#include "devices/VESA.h"


//
// Kernel-Klassen
//
extern CPU              cpu;        // CPU-spezifische Funktionen
extern PIC              pic;        // Interrupt-Controller
extern IntDispatcher    intdis;     // Unterbrechungsverteilung
extern Scheduler        scheduler;  // Scheduler
extern BIOS             bios;       // Schnittstelle zum 16-Bit BIOS
extern unsigned int     total_mem;  // RAM total
extern MemMgmt          mm;         // Speicherverwaltung

//
// Geräte-Treiber-Klassen
//
extern PCSPK            pcspk;      // PC-Lautsprecher
extern CGA_Stream       kout;       // Ausgabe-Strom fuer Kernel
extern Keyboard         kb;         // Tastatur
extern VESA             vesa;       // VESA-Treiber

#endif
