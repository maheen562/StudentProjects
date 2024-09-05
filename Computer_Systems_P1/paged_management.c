/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#include "paged_management.h"

/* ==============================================  Task 3 + 4 functions  ================================================================*/


/*
* Purpose: To allocate space to the MMU (memory_manager)
* char* memory_type: Selected memory mode
* return MMU_t*: Pointer to allocated MMU_t struct 
*/
MMU_t *allocate_MMU_space(char *memory_type) {
    MMU_t *MMU = malloc(sizeof(MMU_t));
    for (int i = 0; i < MAX_PAGES; i++) {
        MMU->pages[i].in_use = 0;
        MMU->pages[i].last_accessed = -1;
        MMU->pages[i].process_name = NULL;

    }

    MMU->memory_type = strdup(memory_type);
    MMU->memory_in_use = 0;
    return MMU;
}


/*
* Purpose: To free allocated MMU_t struct
*/
void free_MMU_space(MMU_t *MMU) {
    free(MMU->memory_type);
    for (int i = 0; i < MAX_PAGES; i++) {
        if (MMU->pages[i].process_name != NULL) {
            free(MMU->pages[i].process_name);
            MMU->pages[i].process_name = NULL;
        }
    }
    free(MMU);
}


/*
* Purpose: To find least recently used frame and evict it 
* process_t *process: input process
* int time: current quantum
* int pages_needed: page memory needed for a process 
*/
void evict_frames(MMU_t *MMU, process_t *process, int time, int pages_needed) {
    int evicted_frames[MAX_PAGES];
    int evicted_frames_no =
        frames_to_evict(evicted_frames, MMU, process, time, pages_needed);
    printf("%d,EVICTED,evicted-frames=[", time);
    for (int i = 0; i < evicted_frames_no; i++) {
        if (MMU->pages[evicted_frames[i]].process_name != NULL) {
            MMU->pages[evicted_frames[i]].in_use = 0;
            MMU->pages[evicted_frames[i]].last_accessed = -1;
            free(MMU->pages[evicted_frames[i]].process_name);
            MMU->pages[evicted_frames[i]].process_name = NULL;
            MMU->memory_in_use--;
        }
    }

    print_frames(evicted_frames, evicted_frames_no);
}

/*
* Purpose: helper function to evict_frames, finds the frames to be evicted
* int evict_frames[]: Array contains all evicted frames
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* int pages_needed: page memory needed for a process 
* return int: number of frames evited
*/
int frames_to_evict(int evicted_frames[], MMU_t *MMU, process_t *process,
                    int time, int pages_needed) {
    int evicted_frames_no = 0;

    // evict frames when process is completed
    if (pages_needed == -1) {
        for (int i = 0; i < MAX_PAGES; i++) {
            if (MMU->pages[i].process_name != NULL) {
                if (strcmp(MMU->pages[i].process_name,
                           get_process_name(process)) == 0) {
                    evicted_frames[evicted_frames_no] = i;
                    evicted_frames_no++;
                }
            }
        }
    } else {

        // evict frames to free space for other processes
        int free_pages = (MAX_PAGES - MMU->memory_in_use);
        while (free_pages < pages_needed) {
            int lru_time = 1000000;

            for (int i = 0; i < MAX_PAGES; i++) {
                if (MMU->pages[i].last_accessed != -1 &&
                    MMU->pages[i].last_accessed <= lru_time) {
                    lru_time = MMU->pages[i].last_accessed;
                }
            }
            for (int i = 0; i < MAX_PAGES; i++) {
                if (free_pages >= pages_needed) {
                    break;
                }
                if (MMU->pages[i].last_accessed == lru_time) {
                    evicted_frames[evicted_frames_no] = i;
                    evicted_frames_no++;
                    free_pages++;
                }
            }
        }
    }
    return evicted_frames_no;
}


/* 
* Purpose: To checks if a process, along with it's pages 
is present in memory when they accessed. If they are in memory, updates the time accessed
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* return bool: true if found, false if not
*/
bool find_process(MMU_t *MMU, process_t *process, int time) {

    int found = 0;
    int found_location[MAX_PAGES];

    for (int i = 0; i < MAX_PAGES; i++) {
        if (MMU->pages[i].process_name != NULL) {
            if (strcmp(MMU->pages[i].process_name, get_process_name(process)) ==
                0) {
                found_location[found] = i;
                found++;
            }
        }
    }

    if ((strcmp(MMU->memory_type, PAGED) == 0 && (found > 0)) ||
        (strcmp(MMU->memory_type, VIRTUAL) == 0 &&
         found >= NEEDED_VIRTUAL_PAGES)) {
        for (int i = 0; i < found; i++) {
            MMU->pages[found_location[i]].last_accessed = time;
        }
        return true;
    }

    return false;
}


/*
* Purpose: To allocates the frames of a process to memory
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* int pages_needed: page memory needed for a process 
*/
void allocate_frames(MMU_t *MMU, process_t *process, int time, int pages_needed) {
    while (pages_needed) {
        for (int i = 0; i < MAX_PAGES; i++) {
            if (!MMU->pages[i].in_use) {
                MMU->pages[i].in_use = 1;
                MMU->pages[i].process_name = strdup(get_process_name(process));
                (MMU->memory_in_use)++;
                MMU->pages[i].last_accessed = time;
                pages_needed--;

                break;
            }
        }
    }
}


/*
* Purpose: Check if process pages (virtual memory) have been allocated to memory frames. If not attempt to do that
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* return bool: always return true
*/
bool allocate_memory_virtual(MMU_t *MMU, process_t *process, int time) {

    if (find_process(MMU, process, time)) {
        return true;
    }
    set_memory_allocated(process, 0);
    int pages_needed =
        ceil((get_process_size(process) * 1.0) / (PAGE_SIZE * 1.0));
    int avalaible_pages = MAX_PAGES - MMU->memory_in_use;

    if (avalaible_pages < pages_needed) {
        if (avalaible_pages < NEEDED_VIRTUAL_PAGES) {
            evict_frames(MMU, process, time, NEEDED_VIRTUAL_PAGES);
        }
        pages_needed = MAX_PAGES - MMU->memory_in_use;
    }
    allocate_frames(MMU, process, time, pages_needed);
    return true;
}

/*
* Purpose: Check if process pages have been allocated to memory frames. If not attempt allocate process pages to memory frames
* MMU_t* MMU: pointer to memory_manager
* process_t *process: input process
* int time: current quantum
* return bool: always return true
*/
bool allocate_memory_paged(MMU_t *MMU, process_t *process, int time) {

    if (find_process(MMU, process, time)) {
        return true;
    }

    set_memory_allocated(process, 0);

    int pages_needed =
        ceil((get_process_size(process) * 1.0) / (PAGE_SIZE * 1.0));

    if (pages_needed > (MAX_PAGES - MMU->memory_in_use)) {
        evict_frames(MMU, process, time, pages_needed);
    }

    allocate_frames(MMU, process, time, pages_needed);
    set_memory_allocated(process, 1);

    return true;
}


/*
* Helper function, prints frames where needed
*/
void print_frames(int memory_pages[], int stored_index) {

    for (int i = 0; i < stored_index - 1; i++) {
        printf("%d,", memory_pages[i]);
    }

    printf("%d]\n", memory_pages[stored_index - 1]);
}