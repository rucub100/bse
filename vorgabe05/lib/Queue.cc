/*****************************************************************************
 *                                                                           *
 *                              Q U E U E                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung einer einfach verketteten Liste von      *
 *                  Chain Objekten. Es wird immer am Ende eingefuegt und     *
 *                  Anfang ausgetragen.                                      *
 *                                                                           *
 * Autor:           Ruslan Curbanov, HHU Duesseldorf                         *
 *****************************************************************************/

#include "lib/Queue.h"

void Queue::enqueue (Chain* item) {
    if (*tail != 0) {
        (*tail)->next = item;
        tail = &(*tail)->next;
    } else {
        *tail = item;
    }
}

Chain* Queue::dequeue () {
    Chain* removed = head;
    
    if (head != 0) {
        if (head->next == *tail) {
            tail = &head;
        } 
        
        head = head->next;
    }

    return removed;
}

void Queue::remove (Chain* item) {
    if (item == 0) {
        return;
    }
    else if (item == head) {
        dequeue();
    }

    Chain* tmp = head;

    while(tmp->next != item && tmp->next != 0) {
        tmp = tmp->next;
    }

    // Item gefunden?!
    if (tmp->next == item) {
        tmp->next = item->next;
    }
}
