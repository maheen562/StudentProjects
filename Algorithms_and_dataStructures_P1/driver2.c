/* Assignment 1, Algorithms and Data Structures
 * Maheen Abdul Khaliq Khan, August 2022
 */
#include <stdio.h>
#include "footpath_data.h"
#include "linklist.h"
#include <string.h>

#define MAX_RECORD_ROWS 1000
int
main(int argc, char *argv[]) {
    
    if(atoi(argv[1])==2){
        list_t* city_footpaths = make_empty_list();
        char *dataFileName = argv[2];
        char *outputFileName = argv[3];

        FILE* inFile = stdin;
        assert(inFile);
        
        FILE *dataFile = fopen(dataFileName,"r");
        assert(dataFile);
        
        FILE *outFile = fopen(outputFileName,"w");
        assert(outFile);
        
        skip_footpath_header(dataFile);

        city_footpaths =  create_footpath_linkedlist(city_footpaths,dataFile);
        
        footpath_t* footpath_array[MAX_RECORD_ROWS];
        assert(footpath_array);
        
        int index=0;
        node_t* access_footpath_linklist;/*= create_empty_node();*/
        
        access_footpath_linklist = get_head_pointer(city_footpaths);
        
        footpath_t* node_data = allocae_footpath_data_space();
        assert(node_data);

        while(access_footpath_linklist!=NULL){
        
        node_data = get_data(access_footpath_linklist);
            create_sorted_array(node_data,footpath_array,&index);
            access_footpath_linklist = get_next_node(
                                        access_footpath_linklist);
        
        }

        
        compare_grade1in(footpath_array,index,outFile,inFile);

        
        free_list(city_footpaths);
        fclose(dataFile);
        fclose(outFile);
    }
    else{
        printf("-- ERROR, READS IN WRONG FILES --");
    }
    return 0;
}
