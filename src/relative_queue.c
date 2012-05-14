#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "relative_queue.h"


relative_queue_t* relative_queue_create(event_t e) {
    relative_queue_t* q = (relative_queue_t*) calloc(1, sizeof(relative_queue_t));
    event_bin_t* head = (event_bin_t*) calloc(1, sizeof(event_bin_t));

    head->e.rank = e.rank;
    head->e.handler = e.handler;
    head->e.arg_ptr = NULL;
    head->next = (event_bin_t*) 0;

    q->head = head;

    return q;
}


void relative_queue_insert(relative_queue_t* q, event_t e) {
    uint16_t rank_acc = 0;
    event_bin_t* head = q->head;

    event_bin_t* new = (event_bin_t*) calloc(1, sizeof(event_bin_t));
    new->e.handler = e.handler;
    new->e.arg_ptr = e.arg_ptr;

    // first
    if((rank_acc + head->e.rank) > e.rank) { // insert as new head
        new->e.rank = e.rank;
        new->next = head;
        head->e.rank -= new->e.rank;
        q->head = new;
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


void print_queue(FILE* out, relative_queue_t* q) {
    fprintf(out, "head_addr=(%p) {\n", q->head);

    event_bin_t* it = q->head;
    do {
        fprintf(out, "\trank=%d, handler=(%p), arg_ptr=(%p)\n", it->e.rank, it->e.handler, it->e.arg_ptr);
        it = it->next;
    } while(it != (event_bin_t*) 0);

    fprintf(out, "}\n");
}


void relative_queue_remove(relative_queue_t* q, event_t e) {
    if(q->head->e.handler == e.handler) { // remove first
        q->head->next->e.rank += q->head->e.rank;

        event_bin_t* new_head = q->head->next;
        free(q->head);
        q->head = new_head;
        return;
    }

    event_bin_t *pred, *succ;
    for(pred = q->head, succ = pred->next; succ->next != NULL; pred = pred->next, succ = succ->next) { // in between
        if(succ->e.handler == e.handler) { // found
            succ->next->e.rank += succ->e.rank;

            pred->next = succ->next;
            free(succ);
            return;
        }
    }

    if(succ->e.handler == e.handler) { // remove last
        free(succ);
        pred->next = NULL;
    }
}

event_t* relative_queue_head(relative_queue_t* q) {
    return &(q->head->e);
}


