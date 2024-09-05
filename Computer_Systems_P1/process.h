/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#ifndef _PROCESS_COMPONENTS_H_
#define _PROCESS_COMPONENTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PROCES_NAME_LENGTH 9 /*max name characters*/

//A struct represents a process.
typedef struct process{
    int arrival_time;
    char name[PROCES_NAME_LENGTH]; // Max 8 characters + null terminator
    int time_needed;
    int remainingTime;
    int memory_needed;
    int memory_allocated;
    bool in_memory;
} process_t;

//Process Constructor
process_t* make_process(int arrival_time,char* name,int time_needed,int memory_needed);


//Process getters
char* get_process_name(process_t* process);
int get_arrival_time(process_t* process);
int get_process_size(process_t* process);
int get_remaining_time(process_t* process);
int get_time_needed(process_t* process);
int get_memory_allocated(process_t* process);

//Process setters
void update_remaining_time(process_t* process, int quantum);
void set_memory_allocated(process_t* process, int allocated);

//Operations on process
void free_process(process_t* process);
#endif