#include <stdlib.h>
#include "../src/relative_queue.h"


int main(void) {
    event_t e1, e2, e3, e4, e5;

    e1.rank = 10;
    e1.handler = NULL;
    e2.rank = 12;
    e2.handler = NULL;
    e3.rank = 27;
    e3.handler = NULL;
    e4.rank = 24;
    e4.handler = NULL;
    e5.rank = 7;
    e5.handler = NULL;

    relative_queue_t* q = relative_queue_create(e1);

    relative_queue_insert(q, e1);
    relative_queue_insert(q, e2);
    relative_queue_insert(q, e3);
    relative_queue_insert(q, e4);
    relative_queue_insert(q, e5);


    return 0;
}

