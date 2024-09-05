/**Code written by Maheen Khan,1193813 and Nhat Hoang Nguyen 1346324, 
 * April 2024, for COMP 30023 Project 1: Process and Memory Managment
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "processor.h"
int main(int argc, char* argv[]) {
    if (argc != 7) {
        printf("Invalid input");
        exit(EXIT_FAILURE);
    }

    char* filename = NULL;
    int quantum = 0;
    char* memoryManagement = NULL;


    // Parse command-line arguments
    for (int i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
        } 
        else if (strcmp(argv[i], "-q") == 0) {
            quantum = atoi(argv[i + 1]);
            if (quantum < 1 || quantum > 3) {
                printf("Quantum must be between 1 and 3.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (strcmp(argv[i], "-m") == 0) {
            memoryManagement = argv[i+1];
        } 
    }

    if (filename == NULL) {
        printf("Filename must be provided.\n");
        exit(EXIT_FAILURE);
    }

    // Open the file
    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    // run the process manager
    run_process_manager(file_ptr,quantum, memoryManagement);

    //Close the file
    fclose(file_ptr);
    return 0;
}