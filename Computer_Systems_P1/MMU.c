/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/


#include "MMU.h"
/* ==============================================  MAIN FUNCTIONS  ================================================================*/


/*
* Purpose: To allocate memory to a process based on the chosen memory mode.
* MMU_t* MMU: pointer to memory_manager
* memory_node_t* memory_list:  A linked list represent the state of physical memory (used in task 2)
* char *memory_mode: Chosen memory mode
* process_t *proces: The arriving process
* int time: current quantum
*/
bool allocate_memory (MMU_t* MMU, memory_node_t* memory_list, char *memory_mode, process_t *process, int time){
    
    //Memory allocation for task 1
    if(strcmp(memory_mode,INFINITE) == 0){
        return true;
    }

    //Memory allocation for task 2
    else if(strcmp(memory_mode,FIRST_FIT) == 0){
        if (process ->in_memory != true){
            if (first_fit_allocation(process,memory_list)){
                process -> in_memory = true;
                return true;
            }
            else {
                return false;
            }
        }
        return true;
    }
    
    //Memory allocation for task 3
    else if(strcmp(memory_mode,PAGED) == 0){
        if (allocate_memory_paged(MMU,process,time)){return true;}
    }

    //Memory allocation for task 4
    else if(strcmp(memory_mode,VIRTUAL) == 0){
        if (allocate_memory_virtual(MMU,process,time)){return true;}

    }
    return false;
}


/*
* Purpose: To deallocate memory assigned a process
* MMU_t* MMU: pointer to memory_manager
* memory_node_t* memory_list:  A linked list represent the state of physical memory (used in task 2)
* char *memory_mode: Chosen memory mode
* process_t *proces: The arriving process
* int time: current quantum
*/
bool evict_process(MMU_t* MMU, memory_node_t* memory_list, char* memory_mode, process_t* process, int time){
    
    //Memory deallocation for task 1
    if(strcmp(memory_mode,INFINITE) == 0){
        return true;
    }
    
    //Memory deallocation for task 2
    else if(strcmp(memory_mode,FIRST_FIT) == 0){
        if (process -> in_memory == true){
            return first_fit_deallocation(process,memory_list);
        }
    }
    //Memory deallocation for task 3 + 4
    else if((strcmp(memory_mode,PAGED) == 0) || (strcmp(memory_mode,VIRTUAL) == 0)){
        evict_frames(MMU,process,time,-1);
        return true;
    }
    return false;
}


/*
* Purpose: To print running process
* MMU_t* MMU: pointer to memory_manager
* memory_node_t* memory_list:  A linked list represent the state of physical memory (used in task 2)
* Queue *current_queue: All ready processes
* char *memory_mode: Chosen memory mode
* int time: current quantum
*/
void print_running(MMU_t* MMU, memory_node_t* memory_list, Queue *current_queue , char *memory_mode, int time){
    
    //Print running process task 1
    if(strcmp(memory_mode,INFINITE) == 0){
        printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", 
            time, get_process_name(get_front(current_queue)), get_remaining_time(get_front(current_queue)));
    }

    //Print running process task 2
    else if(strcmp(memory_mode,FIRST_FIT) == 0){
        double n_occupied = 0;
        memory_node_t* current = first_fit_memory_usage(&n_occupied,get_front(current_queue),memory_list);
        double percentage = ceil((n_occupied/(double)MAX_MEMORY_SIZE) * 100);
        printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", 
            time, get_process_name(get_front(current_queue)), get_remaining_time(get_front(current_queue)),(int)percentage,current->base);
    }

    //Print running process task 3 + 4
    else if ((strcmp(memory_mode,PAGED) == 0) || (strcmp(memory_mode,VIRTUAL) ==0)){
        int mem_usage =
        ceil((((MMU->memory_in_use) * 1.0) / (MAX_PAGES * 1.0)) * 100);
        printf("%d,RUNNING,process-name=%s,remaining-time=%d", 
            time, get_process_name(get_front(current_queue)), get_remaining_time(get_front(current_queue)));
        printf(",mem-usage=%d%%,mem-frames=[", mem_usage);

        int memory_pages[MAX_PAGES];
        int stored_index = 0;

        for (int i = 0; i < MAX_PAGES; i++) {
            if (MMU->pages[i].process_name != NULL) {
                if (strcmp(MMU->pages[i].process_name, get_process_name(get_front(current_queue))) == 0) {
                    memory_pages[stored_index] = i;
                    stored_index++;
                }
            }
        }
        print_frames(memory_pages, stored_index);
    }
}
