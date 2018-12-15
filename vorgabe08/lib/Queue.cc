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
            head = head->next;
        } else if (head == *tail) {
            head = 0;
        } else {
            head = head->next;
        }
        
    }

    return removed;
}

void Queue::remove (Chain* item) {
    if (item == head) {
        dequeue();
    } else if (item == 0 || head == 0 || head == *tail) {
        return;
    }

    Chain* tmp = head;

    while(tmp->next != *tail && tmp->next != item) {
        tmp = tmp->next;
    }

    // Item gefunden?!
    if (tmp->next == item) {
        if (tmp->next != *tail) {
            tmp->next = item->next;
        } else {
            Chain* p = head;
            while(p->next != tmp) 
                p = p->next;
            tail = &p->next;
            tmp->next = 0;
        }
    }
}
