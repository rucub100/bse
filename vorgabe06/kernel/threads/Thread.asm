;*****************************************************************************
;*                                                                           *
;*                            C O R O U T I N E                              *
;*                                                                           *
;*---------------------------------------------------------------------------*
;* Beschreibung:    Assemblerdarstellung der struct ThreadState aus       *
;*                  ThreadState.h                                         *
;*                                                                           *
;*                  Die Reihenfolge der Registerbezeichnungen muss unbedingt *
;*                  mit der von struct ThreadState uebereinstimmen.       *
;*                                                                           *
;* Autor:           Olaf Spinczyk, TU Dortmund                               *
;*****************************************************************************

%include "kernel/threads/Thread.inc"

; EXPORTIERTE FUNKTIONEN

[GLOBAL Thread_switch]
[GLOBAL Thread_start]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]

; THREAD_START : Startet die erste Thread ueberhaupt.
;
; C Prototyp: void Thread_start (struct ThreadState* regs);

Thread_start:

mov eax, [esp + 4]
mov esp, [eax + esp_offset]
ret

; THREAD_SWITCH : Threadnumschaltung. Der aktuelle Registersatz wird
;                    gesichert und der Registersatz der neuen Thread
;                    wird in den Prozessor eingelesen.
;
; C Prototyp: void Thread_switch (struct ThreadState* regs_now,
;                                    struct ThreadState* reg_then);
;
; Achtung: Die Parameter werden von rechts nach links uebergeben.
;
Thread_switch:

mov eax, [esp + 4]
mov [eax + ebx_offset], ebx
mov [eax + esi_offset], esi
mov [eax + edi_offset], edi
mov [eax + ebp_offset], ebp
mov [eax + esp_offset], esp
mov eax, [esp + 8]
mov ebx, [eax + ebx_offset]
mov esi, [eax + esi_offset]
mov edi, [eax + edi_offset]
mov ebp, [eax + ebp_offset]
mov esp, [eax + esp_offset]
ret