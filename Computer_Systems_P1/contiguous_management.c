/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#include "contiguous_management.h"
/* ===============================================  Task 2 functions  ================================================================*/

/*
* Purpose: Create memory_node_t pointer that represents a memory section in physical memory
* int base: the start of the memory section
* int limit: the length of the section
* return memory_node_t*: pointer to memory_node_t
*/

memory_node_t* create_empty_memory_node(int base, int limit){
    memory_node_t *current = (memory_node_t *)calloc(1,sizeof(memory_node_t));
    
    current -> occupied = false;
    current -> process_name = NULL;
    current -> base = base;
    current -> limit = limit;
    current -> next = NULL;

    return current;
}


/*
* Purpose: Allocate a memory section to a process
* process_t* process: input process
* memory_node_t* memory_list: A linked list represent the state of physical memory
* return boolean: true if successful, false if not
*/


bool first_fit_allocation(process_t* process, memory_node_t* memory_list){
    memory_node_t* current = memory_list;
    
    while (current != NULL){
        if (current -> occupied == false){
            if (process -> memory_needed < current -> limit){

                int size = current -> limit;
                memory_node_t* next_node = current -> next; 

                current -> occupied = true;
                current -> process_name = get_process_name(process);
                current -> limit = process -> memory_needed;
                current -> next = create_empty_memory_node(current -> base + current -> limit,size - process -> memory_needed);
                current -> next -> next = next_node;
                return true;
            }
            else if (process -> memory_needed == current -> limit){
                current -> occupied = true;
                current -> process_name = get_process_name(process);
                current -> limit = process -> memory_needed;
                return true;

            }
        }
        current = current -> next;
    }
    return false;
}


/*
* Purpose: Deallocate a memory section
* process_t* process: input process
* memory_node_t* memory_list: A linked list represent the state of physical memory
* return boolean : true if successful, false if not
*/

bool first_fit_deallocation(process_t* process, memory_node_t* memory_list){
    memory_node_t* current = memory_list;
    memory_node_t* prev = NULL;
   
    while (current != NULL){
        if ((current ->process_name != NULL) && (strcmp(current ->process_name,get_process_name(process)) == 0)){
            current -> occupied = false;
            current -> process_name = NULL;
            
            if ((prev != NULL) && (prev -> occupied == false)){
    
                prev -> limit = prev -> limit + current -> limit;
                prev -> next = current -> next;

                free(current);

                current = prev;
            }
            memory_node_t* next_node = current -> next;
            if ((next_node != NULL) && (next_node -> occupied == false )){
                current -> limit = current -> limit + next_node -> limit;
                current -> next = next_node -> next;

                free(next_node);

            }            
            return true;
        }
        prev = current;
        current = current -> next;
    }
    return false;
}


/*
* Purpose: To find the memory section that belongs to a process and to determine
how many KB has been occupied
* double* n_occupied: 
* process_t* process: Process to find
* memory_node_t* memory_list: A linked list represent the state of physical memory
* return memory_node_t* : pointer to memory_node_t
*/

memory_node_t* first_fit_memory_usage(double* n_occupied, process_t* process, memory_node_t* memory_list){
    memory_node_t* current = memory_list;
    memory_node_t* return_node = NULL;

    while (current != NULL){
        if (current -> occupied){
            *n_occupied = *n_occupied + current -> limit;
        }
        if ((current -> process_name != NULL) && (strcmp(current ->process_name,get_process_name(process)) == 0)){
            return_node = current; 
        }
        current = current -> next;
    }
    return return_node;
}