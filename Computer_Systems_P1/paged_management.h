/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#ifndef _PAGED_ALLOCATION_H_
#define _PAGED_ALLOCATION_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "process.h"
#include "queue.h"

#define MAX_PAGES (MAX_MEMORY_SIZE/PAGE_SIZE) /*max pages in memoty*/
#define MAX_MEMORY_SIZE 2048 /*max memory*/
#define PAGE_SIZE 4 /*page size*/
#define NEEDED_VIRTUAL_PAGES 4 /*minimum pages needed in memory 
                                for virtual memroy allocation*/

#define PAGED "paged" /*paged memory type string*/
#define VIRTUAL "virtual" /*virtual memory type string*/

//A struct that represents a paged memory
typedef struct page{
    int in_use;
    char* process_name;
    int last_accessed;
}page_t;

//A struct that represents
typedef struct MMU {
    page_t pages[MAX_MEMORY_SIZE/PAGE_SIZE];
    char* memory_type;
    int memory_in_use;
}MMU_t;

/*
* Purpose: To allocate space to the MMU (memory_manager)
* char* memory_type: Selected memory mode
* return MMU_t*: Pointer to allocated MMU_t struct 
*/
MMU_t* allocate_MMU_space(char* memory_type);

/*
* Purpose: To free allocated MMU_t struct
*/
void free_MMU_space(MMU_t* MMU);

/*
* Purpose: Check if process pages have been allocated to memory frames. If not attempt allocate process pages to memory frames
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* return bool: always return true
*/
bool allocate_memory_paged(MMU_t* MMU, process_t *process,int time);

/*
* Purpose: Check if process pages (virtual memory) have been allocated to memory frames. If not attempt to do that
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* return bool: always return true
*/
bool allocate_memory_virtual(MMU_t* MMU, process_t *process,int time);

/*
* Purpose: To find least recently used frame and evict it 
* process_t *process: input process
* int time: current quantum
* int pages_needed: page memory needed for a process 
*/
void evict_frames(MMU_t* MMU, process_t* process_name, int time,int pages_needed);
/*
* Purpose: helper function to evict_frames, finds the frames to be evicted
* int evict_frames[]: Array contains all evicted frames
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* int pages_needed: page memory needed for a process 
* return int: number of frames evited
*/
int frames_to_evict(int evict_frames[],MMU_t* MMU, process_t* process, int time, int pages_needed);

/* 
* Purpose: To checks if a process, along with it's pages 
is present in memory when they accessed. If they are in memory, updates the time accessed
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* return bool: true if found, false if not
*/
bool find_process(MMU_t* MMU, process_t *process,int time);

/*
* Purpose: To allocates the frames of a process to memory
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* int pages_needed: page memory needed for a process 
*/
void allocate_frames(MMU_t* MMU, process_t *process,int time,int pages_needed);

/*
* Helper function, prints frames where needed
*/
void print_frames(int memory_pages[],int stored_index);

#endif