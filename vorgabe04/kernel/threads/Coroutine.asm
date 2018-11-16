;*****************************************************************************
;*                                                                           *
;*                            C O R O U T I N E                              *
;*                                                                           *
;*---------------------------------------------------------------------------*
;* Beschreibung:    Assemblerdarstellung der struct CoroutineState aus       *
;*                  CoroutineState.h                                         *
;*                                                                           *
;*                  Die Reihenfolge der Registerbezeichnungen muss unbedingt *
;*                  mit der von struct CoroutineState uebereinstimmen.       *
;*                                                                           *
;* Autor:           Olaf Spinczyk, TU Dortmund                               *
;*****************************************************************************

%include "kernel/threads/Coroutine.inc"

; EXPORTIERTE FUNKTIONEN

[GLOBAL Coroutine_switch]
[GLOBAL Coroutine_start]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]

; COROUTINE_START : Startet die erste Coroutine ueberhaupt.
;
; C Prototyp: void Coroutine_start (struct CoroutineState* regs);

Coroutine_start:

mov eax, [esp + 4]
mov esp, [eax + esp_offset]
ret

; COROUTINE_SWITCH : Coroutinenumschaltung. Der aktuelle Registersatz wird
;                    gesichert und der Registersatz der neuen Coroutine
;                    wird in den Prozessor eingelesen.
;
; C Prototyp: void Coroutine_switch (struct CoroutineState* regs_now,
;                                    struct CoroutineState* reg_then);
;
; Achtung: Die Parameter werden von rechts nach links uebergeben.
;
Coroutine_switch:

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