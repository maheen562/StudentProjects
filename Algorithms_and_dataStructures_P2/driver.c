/*
    A PR quadtree that allows us to search a point

   Assignment 2,COMP2003 Algorithms and Data Structures
 * Maheen Abdul Khaliq Khan, September 2022

*/
/* Assignment 1, Algorithms and Data Structures
 * Maheen Abdul Khaliq Khan, August 2022
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "footpath_data.h"
#include "quadtree.h"
#include "rectangle.h"

#define MAX_ROW_LENGTH 512

int main(int argc, char *argv[]){

    if (atoi(argv[1]) == 3)
    {

        char *dataFileName = argv[2];
        char *outputFileName = argv[3];

        FILE *inFile = stdin;
        assert(inFile);
        FILE *dataFile = fopen(dataFileName, "r");
        assert(dataFile);
        FILE *outFile = fopen(outputFileName, "w");
        assert(outFile);
        skip_footpath_header(dataFile);
        footpath_t *footpath = allocate_footpath_data_space();

        rectangle_t *root_coordinates = NULL;

        root_coordinates = allocate_rectangle_space();
        assert(root_coordinates);

        long double top_right_lon = strtold(argv[6], NULL);
        long double top_right_lat = strtold(argv[7], NULL);
        long double end_left_lon = strtold(argv[4], NULL);
        long double end_left_lat = strtold(argv[5], NULL);

        root_coordinates = create_rectangle_from_coordinates(root_coordinates,
                     top_right_lon, top_right_lat, end_left_lon, end_left_lat);

        quadTreenode_t *pr_quadtree = make_empty_quadTree(root_coordinates);

        quadTreenode_t *root = pr_quadtree;

        footpath = read_footpath(dataFile);

        // creates tree
        while (get_footpath_id(footpath) != 0)
        {

            insert_rectangle_coordinates_in_tree(footpath, pr_quadtree);
            footpath = read_footpath(dataFile);

            pr_quadtree = root;

            if (footpath == NULL)
            {
                break;
            }
        }

        int found = 0;
        char *output_string;
        output_string = strdup("\0");

        //binary search and output
        point_t *point_to_search = allocate_point_space();
        quadTreenode_t* data_to_print=allocate_node_space();
        point_to_search = get_point_from_file(inFile);

        while (point_to_search)
        {
            output_string = strdup("\0");
            found = point_binary_search(pr_quadtree, point_to_search, found, 
                                                    output_string,data_to_print);
            if (found == 1)
            {   
                
                print_output(point_to_search, output_string);
                print_footpath_into_file(get_data(data_to_print),point_to_search,
                                    outFile);
                
            }
            point_to_search = get_point_from_file(inFile);
            if (get_lon(point_to_search) == 0)
            {
                break;
            }
            free(output_string);
        }


        fclose(dataFile);
        fclose(outFile);
    }
    else if (atoi(argv[1]) == 4){
        
    }
    else
    {
        printf("-- ERROR, READS IN WRONG FILES --");
    }
    return 0;
}

