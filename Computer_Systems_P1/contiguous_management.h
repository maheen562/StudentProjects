/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/
#ifndef _CONTIGUOUS_ALLOCATION_H_
#define _CONTIGUOUS_ALLOCATION_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "process.h"
#include "queue.h"

//A struct that represents memory sections in physical memory 
typedef struct memory_node {
    bool occupied;
    char *process_name;
    int base;
    int limit;
    struct memory_node *next;
}memory_node_t;

/*
* Purpose: Create memory_node_t pointer that represents a memory section in physical memory
* int base: the start of the memory section
* int limit: the length of the section
* return memory_node_t*: pointer to memory_node_t
*/
memory_node_t* create_empty_memory_node(int base, int limit);

/*
* Purpose: To find the memory section that belongs to a process and to determine
how many KB has been occupied
* double* n_occupied: 
* process_t* process: Process to find
* memory_node_t* memory_list: A linked list represent the state of physical memory
* return memory_node_t* : pointer to memory_node_t
*/
memory_node_t* first_fit_memory_usage(double* n_occupied, process_t* process, memory_node_t* memory_list);

/*
* Purpose: Allocate a memory section to a process
* process_t* process: input process
* memory_node_t* memory_list: A linked list represent the state of physical memory
* return boolean: true if successful, false if not
*/
bool first_fit_allocation(process_t* process, memory_node_t* memory_list);

/*
* Purpose: Deallocate a memory section
* process_t* process: input process
* memory_node_t* memory_list: A linked list represent the state of physical memory
* return boolean : true if successful, false if not
*/
bool first_fit_deallocation(process_t* process, memory_node_t* memory_list);

#endif