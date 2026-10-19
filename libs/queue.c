#include <stdlib.h>
#include "../include/queue.h"
#include "../include/graph.h"

ErrorCode enqueue(Queue* queue, int vertex) {
    if (!queue) {
        return ERR_NOT_INITIALISED;   
    }
    QueueItem* new = malloc(sizeof(QueueItem));
    if (!new) {
        return ERR_MEMORY;
    }
    new -> vertex = vertex;
    new -> next = NULL;
    if (!queue -> head){
        queue -> head = new;
    }
    else {
        queue -> tail-> next = new;
    }
    queue -> tail = new;
    return OK;
}

int dequeue(Queue* queue) {
    if (!queue || !queue -> head) {
        return -1;
    }
    
    QueueItem* del = queue -> head;
    int vertex = del -> vertex;
    queue -> head = queue -> head -> next;
    if (!queue -> head) {
        queue -> tail = NULL;
    }
    free(del);
    return vertex;
}

void free_queue(Queue* queue) {
    QueueItem* del;
    while (queue -> head) {
        del = queue -> head;
        queue -> head = queue -> head -> next;
        free(del);
    }
    free(queue);
}
