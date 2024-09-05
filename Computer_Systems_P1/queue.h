/**Code written by Nhat Hoang Nguyen 1346324, modified by 
 * Maheen Khan,1193813
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"

// node structure in queue
typedef struct Node {
    process_t* data; 
    struct Node* next;
} Node;

// queue structure to store processes
typedef struct {
    Node* front;
    Node* rear;
    int count;
} Queue;

/* 
* Purpose: Used to create a queue
*/
Queue* createQueue();

//Operations on contructed queue
void enqueue(Queue* q, process_t* data);
void dequeue(Queue* q);
void freeQueue(Queue* q);
void decrement_queue_count(Queue* q);
void send_front_to_rear(Queue* q);
bool transfer_process (Queue *q1, Queue *q2);


 
//Operations to get queue data
process_t* get_rear(Queue* q);
process_t* get_front(Queue* q);
int get_queue_count(Queue* q);
void freeQueue(Queue* q);
int isQueueEmpty(Queue* q);

#endif