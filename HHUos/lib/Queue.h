/*****************************************************************************
 *                                                                           *
 *                              Q U E U E                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung einer einfach verketteten Liste von      *
 *                  Chain Objekten. Es wird immer am Ende eingefuegt und     *
 *                  Anfang ausgetragen.                                      *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/

#ifndef __Queue_include__
#define __Queue_include__

#include "lib/Chain.h"

class Queue {
    
private:
    Queue(const Queue &copy); // Verhindere Kopieren

protected:
    Chain* head;
    Chain** tail;

public:
    Queue () { head = 0; tail = &head; }
    void enqueue (Chain* item);
    Chain* dequeue ();
    bool remove (Chain*);

    unsigned int size() {
        unsigned int cnt = 0;
        Chain *tmp = head;

        while(tmp != 0) {
            tmp = tmp->next;
            cnt++;
        }

        return cnt;
    }
 };

#endif

