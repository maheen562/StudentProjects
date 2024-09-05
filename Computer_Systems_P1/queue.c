
/**Code written by Nhat Hoang Nguyen 1346324, modified by 
 * Maheen Khan,1193813
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#include "queue.h"

/* 
* Purpose: Used to create a queue
*/
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) {
        perror("Failed to create queue");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    q->count = 0;
    return q;
}

/* ==============================================  Queue operations functions  ================================================================*/


void enqueue(Queue* q, process_t* data) {

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Failed to enqueue");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    
    q->count++;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
    
}

void dequeue(Queue* q) {
    if (q->front == NULL) {
        perror("Queue is empty. Cannot dequeue.");
        exit(EXIT_FAILURE);
    }
    Node* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    q->count--;
}

bool transfer_process(Queue* q1, Queue* q2){
    if (q1->front == NULL) {
        printf("Queue is empty");
        return false;    
    }
    q1->count--;
    q2->count++;
    
    //Scenario where q1 has 1 process left
    if (q1->front->next ==NULL){
        if(q2->front == NULL){
            q2->front = q2->rear = q1->front;
            q2->front->next = NULL;
        }
        
        q2->rear->next = q1->front;
        q1->front = q1->rear = NULL;
        
        q2->rear = q2->rear->next;
        return true;
    }

    //Scenario where q1 has multiple processes
    Node* temp = q1->front;
    q1->front = q1->front->next;
    if(q2->front == NULL){
        q2->front = q2->rear = temp;
        q2->front->next = NULL;
    }
    else{
        q2->rear->next = temp;
        q2->rear = q2->rear->next;
        q2->rear->next = NULL;

    }
    return true;
}

void send_front_to_rear(Queue* q){
    if(q->front == NULL || q->front == q->rear){
        return;
    }
    q->rear->next = q->front;
    q->front = q->front->next;
    q->rear = q->rear->next;
    q->rear->next = NULL;
}


void freeQueue(Queue* q) {
    while (!isQueueEmpty(q)) {
        dequeue(q); // This will free all nodes
    }
    free(q); // Finally free the queue structure itself
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}


void decrement_queue_count(Queue* q){
    q->count--;
}

/* ==============================================  accessing queue nodes  ================================================================*/


process_t* get_rear(Queue* q){
    return q->rear->data;
}

process_t* get_front(Queue* q){
    return q->front->data;
}

int get_queue_count(Queue* q){
    return q->count;
}