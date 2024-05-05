/* Assignment 1, Algorithms and Data Structures
 * Maheen Abdul Khaliq Khan, August 2022
 */
#include <stdio.h>
#include "footpath_data.h"
#include "linklist.h"
#include <string.h>

#define MAX_ROW_LENGTH 512
int
main(int argc, char *argv[]) {

    if(atoi(argv[1])==1){
    
        list_t* city_footpaths = make_empty_list();
        //char* stage = argv[1];
        char *dataFileName = argv[2];
        char *outputFileName = argv[3];
        FILE* inFile = stdin;
        FILE *dataFile = fopen(dataFileName,"r");
        assert(dataFile);
        FILE *outFile = fopen(outputFileName,"w");
        assert(outFile);
        skip_footpath_header(dataFile);


        city_footpaths =  create_footpath_linkedlist(city_footpaths,dataFile);
        search_adresses(city_footpaths,inFile,outFile);
        

        free_list(city_footpaths);
        fclose(dataFile);
        fclose(outFile);

    }
    else{
        printf("-- ERROR, READS IN WRONG FILES --");
    }
    return 0;
}
