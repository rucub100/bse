;******************************************************************************
;*                                                                            *
;*                        S T A R T U P . A S M                               *
;*                                                                            *
;*----------------------------------------------------------------------------*
;* Beschreibung:    'startup' ist der Eintrittspunkt des eigentlichen Systems.*
;*                  Die Umschaltung in den Protected-Mode ist bereits erfolgt.*
;*                  Es wird alles vorbereitet, damit so schnell wie moeglich  *
;*                  die weitere Ausfuehrung durch C-Code erfolgen kann.       *
;*                                                                            *
;*                  Hier erweitert, um BIOS callund Paging-Aktivierung sowie  *
;*                  Unterstuetzung des Bluescreens.                           *
;*                                                                            *
;* Autor:           Olaf Spinczyk, TU Dortmund                                *
;*                  Michael Schoettner, HHU, 11.12.2018                       *
;******************************************************************************

; Multiboot-Konstanten
MULTIBOOT_PAGE_ALIGN	equ	1<<0
MULTIBOOT_MEMORY_INFO	equ	1<<1

; Magic-Number fuer Multiboot
MULTIBOOT_HEADER_MAGIC	equ	0x1badb002
; Multiboot-Flags (ELF-spezifisch!)
MULTIBOOT_HEADER_FLAGS	equ	MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_HEADER_CHKSUM	equ	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
MULTIBOOT_EAX_MAGIC	equ	0x2badb002

;
;   System
;

[GLOBAL startup]
[GLOBAL idt]
[GLOBAL __cxa_pure_virtual]
[GLOBAL bios_call]
[GLOBAL get_thread_vars]
[GLOBAL paging_on]
[GLOBAL get_page_fault_address]
[GLOBAL get_int_esp]

; Michael Schoettner:
; Nachfolgender label steht fuer das 'delete', welches jetzt implementiert
; wird. Damit der Linker nicht wegen doppelter Definition "meckert"
; nun auskommentieren!
; [GLOBAL _ZdlPv]


[EXTERN main]
[EXTERN int_disp]

[EXTERN ___BSS_START__]
[EXTERN ___BSS_END__]
[EXTERN __init_array_start]
[EXTERN __init_array_end]
[EXTERN __fini_array_start]
[EXTERN __fini_array_end]
	
[SECTION .text]

startup:
	jmp skip_multiboot_hdr

multiboot_header:
	align 4
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_HEADER_CHKSUM

skip_multiboot_hdr:
; GCC-kompilierter Code erwartet das so.

	cld

	cmp eax,MULTIBOOT_EAX_MAGIC
	jne floppy_boot
;
; GDT setzen (notwendig, falls wir durch GRUB geladen wurden)
;
	lgdt	[gdt_48]

floppy_boot:

; Globales Datensegment

	mov	ax,0x10
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	gs,ax

; Stack festlegen
	
	mov	ss,ax
	mov	esp,init_stack+4096

; Unterbrechungsbehandlung sicherstellen

	call	setup_idt
	call	reprogram_pics
	
; BSS loeschen
	mov	edi, ___BSS_START__
clear_bss:
	mov	byte [edi], 0
	inc	edi
	cmp	edi, ___BSS_END__
	jne	clear_bss

; Aufruf des C-Codes

	call	_init	; Konstruktoren globaler Objekte ausfuehren
	call	main	; C/C++ Level System
	call	_fini	; Destruktoren
	hlt

;; Ausfuehrung der Konstruktoren globaler Objekte
_init:
	mov	edi, __init_array_start
_init_loop:
	cmp	edi, __init_array_end
	je	_init_done
	mov	eax, [edi]
	call	eax
	add	edi, 4
	ja	_init_loop
_init_done:
	ret

;; Ausfuehrung der Destruktoren globaler Objekte
_fini:
	mov	edi, __fini_array_start
_fini_loop:
	cmp	edi, __fini_array_end
	je	_fini_done
	mov	eax, [edi]
	call	eax
	add	edi, 4
	ja	_fini_loop
_fini_done:
	ret

; Auslesen von 'int_esp'
; wird im Bluescreen benoetigt, um den Stacks zuzugreifen
;
; C Prototyp: void get_int_esp (unsigned int** esp);
get_int_esp:
    mov	eax,[4+esp]     ; esp
    mov ebx, int_esp
    mov [eax], ebx
    ret

; Default Interrupt Behandlung

; Spezifischer Kopf der Unterbrechungsbehandlungsroutinen

%macro wrapper 1
wrapper_%1:
    pushad              ; alle Register des Threads sichern
    mov ecx, int_esp    ; stack-zeiger sichern, fuer Zugriff in Bluescreen
    mov [ecx], esp
    mov	al,%1
    jmp	wrapper_body
%endmacro


; ... wird automatisch erzeugt.
%assign i 0
%rep 256
wrapper i
%assign i i+1
%endrep

; Gemeinsamer Rumpf
wrapper_body:
    cld             ; das erwartet der gcc so.
    push	ecx		; Sichern der fluechtigen Register
    push	edx
    and	eax,0xff	; Der generierte Wrapper liefert nur 8 Bits
    push	eax		; Nummer der Unterbrechung uebergeben
    call	int_disp; Interrupt-Dispatcher aufrufen
    add	esp,4		; Nummer der Unterbrechung vom Stack entfernen
    pop	edx         ; fluechtige Register wieder herstellen
    pop	ecx
    popad	        ; alle Register wiederherstellen
    iret            ; fertig!

;
; setup_idt
;
; Relokation der Eintraege in der IDT und Setzen des IDTR

setup_idt:
	mov	eax,wrapper_0	; ax: niederwertige 16 Bit
	mov	ebx,eax
	shr	ebx,16      ; bx: hoeherwertige 16 Bit
	mov	ecx,255     ; Zaehler
.loop:	add	[idt+8*ecx+0],ax
	adc	[idt+8*ecx+6],bx
	dec	ecx
	jge	.loop

	lidt	[idt_descr]
	ret

;
; bios_call
;
; BIOS-Aufruf (siehe BIOS.cc)
;
bios_call:
    lidt    [idt16_descr]
    pushf
    pusha
    call  0x18:0
    popa
    popf
    lidt	[idt_descr]
    ret


;
; Paging aktivieren
;
; (siehe Paging.cc)
;
paging_on:
    mov eax,[4+esp]     ; Parameter Addr. Page-Dir. ins eax Register
    mov ebx, cr4
    or  ebx, 0x10       ; 4 MB Pages aktivieren
    mov cr4, ebx        ; CR4 schreiben
    mov cr3, eax        ; Page-Directory laden
    mov ebx, cr0
    or  ebx, 0x80010000 ; Paging aktivieren
    mov cr0, ebx
    ret

get_page_fault_address:
    pop ecx     ; raddr speichern
    mov eax,cr2
    push eax
    push ecx    ; raddr wieder auf Stack
    ret
;
; reprogram_pics
;
; Neuprogrammierung der PICs (Programmierbare Interrupt-Controller), damit
; alle 15 Hardware-Interrupts nacheinander in der idt liegen.

reprogram_pics:	
	mov	al,0x11     ; ICW1: 8086 Modus mit ICW4
	out	0x20,al   
	call	delay
	out	0xa0,al
	call	delay
	mov	al,0x20     ; ICW2 Master: IRQ # Offset (32)
	out	0x21,al
	call	delay
	mov	al,0x28     ; ICW2 Slave: IRQ # Offset (40)
	out	0xa1,al
	call	delay
	mov	al,0x04     ; ICW3 Master: Slaves an IRQs
	out	0x21,al
	call	delay
	mov	al,0x02     ; ICW3 Slave: Verbunden mit IRQ2 des Masters
	out	0xa1,al
    call	delay
	mov	al,0x03     ; ICW4: 8086 Modus und automatischer EIO
	out	0x21,al
	call	delay
	out	0xa1,al
	call	delay

	mov	al,0xff     ; Hardware-Interrupts durch PICs
	out	0xa1,al     ; ausmaskieren. Nur der Interrupt 2,
    call	delay	; der der Kaskadierung der beiden
	mov	al,0xfb     ; PICs dient, ist erlaubt.
	out	0x21,al

	ret

; delay
;
; Kurze Verzoegerung fuer in/out Befehle.

delay:
	jmp	.L2
.L2:	ret

; Die Funktion wird beim abarbeiten der globalen Konstruktoren aufgerufen
; (unter Linux). Das Label muss definiert sein (fuer den Linker). Die
; Funktion selbst kann aber leer sein, da bei StuBs keine Freigabe des 
; Speichers erfolgen muss.

__cxa_pure_virtual:
; Michael Schoettner:
; Nachfolgender label steht fuer das 'delete', welches jetzt implementiert
; wird. Damit der Linker nicht wegen doppelter Definition "meckert"
; nun auskommentieren!
;_ZdlPv:
	ret

[SECTION .data]

;  'interrupt descriptor table' mit 256 Eintraegen.

idt:

%macro idt_entry 1
	dw	(wrapper_%1 - wrapper_0) & 0xffff
	dw	0x0008
	dw	0x8e00
	dw	((wrapper_%1 - wrapper_0) & 0xffff0000) >> 16
%endmacro

; ... wird automatisch erzeugt.

%assign i 0
%rep 256
idt_entry i
%assign i i+1
%endrep

idt_descr:
	dw	256*8-1     ; idt enthaelt 256 Eintraege
	dd	idt

;   Stack und interrupt descriptor table im BSS Bereich

[SECTION .bss]

init_stack:
	resb	4096

[SECTION .data]
;
; Descriptor-Tabellen
;
gdt:
	dw	0,0,0,0		; NULL Deskriptor

	dw	0xFFFF		; 4Gb - (0x100000*0x1000 = 4Gb)
	dw	0x0000		; base address=0
	dw	0x9A00		; code read/exec
	dw	0x00CF		; granularity=4096, 386 (+5th nibble of limit)

	dw	0xFFFF		; 4Gb - (0x100000*0x1000 = 4Gb)
	dw	0x0000		; base address=0
	dw	0x9200		; data read/write
	dw	0x00CF		; granularity=4096, 386 (+5th nibble of limit)

    dw  0xFFFF      ; 4Gb - (0x100000*0x1000 = 4Gb)
    dw	0x4000      ; 0x4000 -> base address=0x24000 (siehe BIOS.cc)
    dw  09A02h      ; 0x2 -> base address =0x24000 (siehe BIOS.cc) und code read/exec;
    dw  0008Fh      ; granularity=4096, 16-bit code

gdt_48:
	dw	0x18		; GDT Limit=24, 3 GDT Eintraege
	dd	gdt         ; Physikalische Adresse der GDT

[SECTION .data]
;
; IDT des Realmode ;
; (Michael Schoettner)
;

idt16_descr:
    dw	1024    ; idt enthaelt max. 1024 Eintraege
    dd	0       ; Adresse 0

;
; Stack-Zeiger fuer Bluescreen
; (genauerer Stack-Aufbau siehe Bluescreen.cc)
;
; |-------------|
; |    EFLAGS   |
; |-------------|
; |      CS     |
; |-------------|
; |     EIP     |
; |-------------|
; | [ErrorCode] |
; |-------------|
; | alle Regs.  |
; | (PUSHAD)    |
; |-------------| <-- int_esp
int_esp:
    db 0,0,0,0
