/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#include "processor.h"
/* ==============================================  MAIN FUNCTIONS  ================================================================*/


/*
* Purpose: Main function to run RR
* FILE *data_file: pointer to file that contains info of processes
* int quantum: Round - Robin quantum
* char *memory_type: Selected memory management mode
*/

void run_process_manager(FILE *data_file, int quantum, char *memory_type) {

    MMU_t *memory_manager = allocate_MMU_space(memory_type); // memory manager reponsible for paged memory allocation + virtual memory allocation mode
    memory_node_t *memory_list = create_empty_memory_node(0,MAX_MEMORY_SIZE); //memory manager responsible for contiguous memory allocation mode

    Queue *all_processes =  createQueue();   //queue contains all processeses in input file
    Queue *current_queue = createQueue();    //queue contains all running processes

    //Processor stats
    int total_processes = read_file(data_file, all_processes);
    double turnaround = 0.0;
    int time = 0;
    double max_overhead = 0.0;
    double average_overhead = 0.0;

    //Flags
    bool context_switch = false;


    while (get_queue_count(current_queue) != 0 || get_queue_count(all_processes) != 0){
        
        //Enqueue all processes arrive
        if (get_queue_count(all_processes) > 0) {
            enqueue_processes_arriving(all_processes,current_queue,memory_type, time);
        }
        
        if (get_queue_count(current_queue)){
            
            //Process has finished in this quantum
            if (get_remaining_time(get_front(current_queue)) <= 0){

                //Attempt to deallocate the memory section belonging to a process
                if (evict_process(memory_manager,memory_list,memory_type,get_front(current_queue), time)){
                    
                    //If deallocation successful
                    printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, get_process_name(get_front(current_queue)), 
                                                                            get_queue_count(current_queue)-1);
                
                    turnaround +=(time - get_arrival_time(get_front(current_queue)));
                    calculate_overhead(&max_overhead, &average_overhead, time,
                            get_arrival_time(get_front(current_queue)),
                            get_time_needed(get_front(current_queue)));
                
        
                    free_process(get_front(current_queue));
                    dequeue(current_queue);
                    context_switch = true;
                }
            }
            
            //Process hasn't finished in this quantum, move to the next process
            else if (get_queue_count(current_queue) > 0 && time !=0) {                  
                    switch_processes(current_queue);
                    if (get_queue_count(current_queue) > 1){
                        context_switch = true;
                    }
            }

            if (get_queue_count(current_queue) != 0) {

                //Perform context switch
                if((context_switch || (get_time_needed(get_front(current_queue)) == get_remaining_time(get_front(current_queue))))){

                    //Check if memory is allocated, if not attempt to do so
                    if(allocate_memory(memory_manager,memory_list, memory_type,get_front(current_queue),time)){
                        
                        //If successful, run process
                        print_running(memory_manager,memory_list,current_queue,memory_type,time);
                        context_switch = false;
                    }
                    else{
                        //If not, then perform context switch on the next loop
                        continue;
                    }
                }
                update_remaining_time(get_front(current_queue),quantum);
            }
        }
        time += quantum;
    }

    //Stats related output
    turnaround = turnaround / total_processes;
    int rounded_turnaround = ceil(turnaround);
    printf("Turnaround time %d\n", rounded_turnaround);
    print_overhead(max_overhead,average_overhead,total_processes);
    printf("Makespan %d\n", time - quantum);
    
    //Free memory space
    free_MMU_space(memory_manager);
    free(memory_list);
    freeQueue(all_processes);
    free(current_queue);

}

/* ==============================================  HELPER FUNCTIONS  ================================================================*/
/*Reads a file
return: the total number of processes that are read*/
int read_file(FILE *f, Queue *process_queue) {

    int arrival_time, time_needed, memory_needed;
    char process_name[PROCES_NAME_LENGTH];
    int total_processes = 0;

    while (fscanf(f, "%d %s %d %d", &arrival_time, process_name, &time_needed,
                  &memory_needed) == 4) {

        process_t *process = make_process(arrival_time, process_name,
                                          time_needed, memory_needed);
        total_processes++;
        enqueue(process_queue, process);
    }
    return total_processes;
}


/*
* Purpose: Enqueue all processes that arrive at the current quantum
* Queue *all_process_queue: queue contains all processeses in input file
* Queue *current_queue: queue contains all running processes
* char *memory_mode: selected memory allocation mode
* int time: current quantum
*/

void enqueue_processes_arriving(Queue *all_process_queue, Queue *current_queue, char *memory_mode, int time){  
    
    bool transfer_success = true;
    process_t *current_front = (process_t *)get_front(all_process_queue);
    assert(current_front);
    
    while (get_arrival_time(current_front) <= time && get_queue_count(all_process_queue) && transfer_success) {
        transfer_success = transfer_process(all_process_queue, current_queue);
        if (get_queue_count(all_process_queue) > 0) {
            current_front = (process_t *)get_front(all_process_queue);

        }
    }
}




/*
* Purpose: To switch to the next process in the current_queue 
* Queue *current_queue: queue contains all running processes
*/

void switch_processes(Queue *current_queue) {
    send_front_to_rear(current_queue);
};

/*Stats related function*/

/*Calculates the overhead after each process is completed*/
void calculate_overhead(double *maxoverhead, double *average_overhead, int time,
                        int process_arrival_time, int time_needed) {
    double process_overhead =
        ((time - process_arrival_time) * 1.0) / (time_needed * 1.0);
    if (process_overhead > *maxoverhead) {
        *maxoverhead = process_overhead;
    }
    *average_overhead += process_overhead;
}

/*prints the overhead*/
void print_overhead(double max_overhead,double average_overhead, int total_processes){
  max_overhead = round(max_overhead*100.0) / 100.0;
  average_overhead = round(average_overhead*100.0)/ 100.0;
  printf("Time overhead %.2f %.2f\n", max_overhead,
           (average_overhead / (total_processes * 1.0)));
}