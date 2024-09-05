/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "queue.h"
#include "MMU.h"
#include "process.h"

/* ==============================================  MAIN FUNCTIONS  ================================================================*/

/*
* Purpose: Main function to run RR
* FILE *data_file: pointer to file that contains info of processes
* int quantum: Round - Robin quantum
* char *memory_type: Selected memory management mode
*/
void run_process_manager(FILE *data_file, int quantum, char *memory_type);

/* ==============================================  HELPER FUNCTIONS  ================================================================*/

/*
* Purpose: Enqueue all processes that arrive at the current quantum
* Queue *all_process_queue: queue contains all processeses in input file
* Queue *current_queue: queue contains all running processes
* char *memory_mode: selected memory allocation mode
* int time: current quantum
*/
void enqueue_processes_arriving(Queue *all_process_queue, Queue *current_queue, char *memory_mode, int time);

/*
* Purpose: To switch to the next process in the current_queue 
* Queue *current_queue: queue contains all running processes
*/
void switch_processes(Queue *current_queue);

/*Reads a file
* return: the total number of processes that are read
*/
int read_file(FILE *f, Queue *process_queue);

/*
* Stats related functions
*/
void calculate_overhead(double *maxoverhead, double *average_overhead, int time, int process_arrival_time, int time_needed);
void print_overhead(double max_overhead,double average_overhead, int total_processes);
#endif