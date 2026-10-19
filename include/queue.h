#ifndef QUEUE_H
#define QUEUE_H

#include "graph.h"

typedef struct QueueItem {
    int vertex;
    struct QueueItem* next; 
} QueueItem;

typedef struct Queue {
    QueueItem* head;
    QueueItem* tail;
} Queue;

ErrorCode enqueue(Queue*, int);
int dequeue(Queue*);
void free_queue(Queue*);

#endif