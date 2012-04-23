#include <stdint.h>
#include <stdlib.h>
#include "relative_queue.h"


event_bin_t* relative_queue_create(event_t e) {
    event_bin_t* head = (event_bin_t*) malloc(sizeof(event_bin_t));

    head->e.rank = e.rank;
    head->e.handler = e.handler;
    head->next = (event_bin_t*) 0;

    return head;
}

void relative_queue_insert(event_bin_t* head, event_t e) {
    uint16_t rank_acc = 0;

    event_bin_t* new = (event_bin_t*) malloc(sizeof(event_bin_t));
    new->e.handler = e.handler;

    // first
    if((rank_acc + head->e.rank) > e.rank) { // insert as new head
        new->e.rank = e.rank;
        head->e.rank -= new->e.rank;
        new->next = head;
        return;
    }

    // between
    event_bin_t *pred, *succ;
    for(pred = head, succ = head->next; succ != NULL; pred = pred->next, succ = succ->next) {
        rank_acc += pred->e.rank;

        if((rank_acc + succ->e.rank) > e.rank) { // insert here
            new->e.rank = e.rank - rank_acc;
            succ->e.rank -= new->e.rank;
            pred->next = new;
            new->next = succ;
            return;
        }
    }

    // last
    rank_acc += pred->e.rank;
    new->e.rank = e.rank - rank_acc;
    pred->next = new;
    new->next = (event_bin_t*) 0;
}

