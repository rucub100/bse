/*****************************************************************************
 *                                                                           *
 *                       T H R E A D S T A T E                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Struktur ThreadState dient dazu, bei einem           *
 *                  Koroutinenwechsel die Werte der nicht-fluechtigen        *
 *                  Register zu sichern. Beim GNU C Compiler sind eax, ecx   *
 *                  und edx fluechtige Register, die bei Funktionsaufrufen   * 
 *                  und somit auch bei einem Koroutinenwechsel keine spaeter *
 *                  noch benoetigten Werte haben duerfen. Daher muss in der  * 
 *                  Struktur ThreadState auch kein Platz fuer sie bereit-    *
 *                  gestellt werden.                                         *
 *                                                                           *
 * Achtung:         Fuer den Zugriff auf die Elemente von                    *
 *                  struct ThreadState aus einer Assemblerfunktion           *
 *                  heraus werden in der Datei Thread.inc Namen fuer die     *
 *                  benoetigten Abstaende der einzelnen Elemente zum Anfang  * 
 *                  der Struktur definiert. Damit dann auch auf die richtigen*
 *                  Elemente zugegriffen wird, sollten sich die Angaben von  * 
 *                  ThreadState.h und Thread.inc exakt entsprechen.          *
 *                  Wer also ThreadState.h aendert, muss auch                *
 *                  Thread.inc anpassen (und umgekehrt.)                     *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/

#ifndef __ThreadState_include__
#define __ThreadState_include__

struct ThreadState {
    void *ebx;
    void *esi;
    void *edi;
    void *ebp;
    void *esp;
};

#endif

