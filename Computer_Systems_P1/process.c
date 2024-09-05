/**Code modified by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment.
 * code written by Maheen Khan,1193813, the previous time I took this subject
*/

#include "process.h"

#define MAX_ROW_LENGTH 128


/*Creates a process*/
process_t* make_process(int arrival_time,char* name,int time_needed,int memory_needed){
  
  process_t* process = (process_t*)malloc(sizeof(process_t));
  
  process->arrival_time = arrival_time;
  strcpy(process->name,name);
  process->time_needed = time_needed;
  process->memory_needed =memory_needed;
  process->remainingTime = time_needed;

  return process;
}

/*frees a process*/
void free_process(process_t* process){
  free(process);
}


/* ==============================================  Getters  ================================================================*/

char* get_process_name(process_t* process){
  return process->name;
}


int get_process_size(process_t* process){
  return process->memory_needed;
}

int get_arrival_time(process_t* process){
  return process->arrival_time;
}

int get_time_needed(process_t* process){
  return process->time_needed;
}

int get_remaining_time(process_t* process){
  return process->remainingTime;
}


int get_memory_allocated(process_t* process){
  return process->memory_allocated;
}


/* ==============================================  Setters  ================================================================*/

void update_remaining_time(process_t* process, int quantum){
    process->remainingTime -= quantum;
}


void set_memory_allocated(process_t* process,int allocated){
  process->memory_allocated = allocated;
}