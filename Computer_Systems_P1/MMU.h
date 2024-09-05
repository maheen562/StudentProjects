/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/


#ifndef _MMU_H_
#define _MMU_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "process.h"
#include "queue.h"
#include "paged_management.h"
#include "contiguous_management.h"

#define INFINITE "infinite" /*infinite memory type string*/
#define FIRST_FIT "first-fit" /*first fit memory type string*/


/* ==============================================  Main functions  ================================================================*/

/*
* Purpose: To allocate memory to a process based on the chosen memory mode.
* MMU_t* MMU: pointer to memory_manager
* memory_node_t* memory_list:  A linked list represent the state of physical memory (used in task 2)
* char *memory_mode: Chosen memory mode
* process_t *proces: The arriving process
* int time: current quantum
*/
bool allocate_memory (MMU_t* MMU, memory_node_t* memory_list, char *memory_mode, process_t *proces, int time);

/*
* Purpose: To deallocate memory assigned a process
* MMU_t* MMU: pointer to memory_manager
* memory_node_t* memory_list:  A linked list represent the state of physical memory (used in task 2)
* char *memory_mode: Chosen memory mode
* process_t *proces: The arriving process
* int time: current quantum
*/
bool evict_process(MMU_t* MMU, memory_node_t* memory_list, char* memory_mode, process_t* process, int time);

/*
* Purpose: To print running process
* MMU_t* MMU: pointer to memory_manager
* memory_node_t* memory_list:  A linked list represent the state of physical memory (used in task 2)
* Queue *current_queue: All ready processes
* char *memory_mode: Chosen memory mode
* int time: current quantum
*/
void print_running(MMU_t* MMU, memory_node_t* memory_list, Queue *current_queue , char *memory_mode, int time);


#endif