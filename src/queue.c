#include "openlibc/queue.h"

/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 */

olc_queue_t *
olc_queue_middle(olc_queue_t *queue)
{
    olc_queue_t  *middle, *next;

    middle = olc_queue_head(queue);

    if (middle == olc_queue_last(queue)) {
        return middle;
    }

    next = olc_queue_head(queue);

    for ( ;; ) {
        middle = olc_queue_next(middle);

        next = olc_queue_next(next);

        if (next == olc_queue_last(queue)) {
            return middle;
        }

        next = olc_queue_next(next);

        if (next == olc_queue_last(queue)) {
            return middle;
        }
    }
}


/* the stable insertion sort */

void
olc_queue_sort(olc_queue_t *queue,
    olc_int_t (*cmp)(const olc_queue_t *, const olc_queue_t *))
{
    olc_queue_t  *q, *prev, *next;

    q = olc_queue_head(queue);

    if (q == olc_queue_last(queue)) {
        return;
    }

    for (q = olc_queue_next(q); q != olc_queue_sentinel(queue); q = next) {

        prev = olc_queue_prev(q);
        next = olc_queue_next(q);

        olc_queue_remove(q);

        do {
            if (cmp(prev, q) <= 0) {
                break;
            }

            prev = olc_queue_prev(prev);

        } while (prev != olc_queue_sentinel(queue));

        olc_queue_insert_after(prev, q);
    }
}
