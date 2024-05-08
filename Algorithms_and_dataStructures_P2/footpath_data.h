#ifndef _FOOTPATH_H_
#define _FOOTPATH_H_

#include <stdio.h>
#include <stdlib.h>

#include "rectangle.h"

/*typedef structure for "point 2D"*/
typedef struct point2D point_t;

/*typedef structure for "rectangle2D"*/
typedef struct rectangle2D rectangle_t;

/*typedef structure for "footpath_t"*/
typedef struct footpath footpath_t;

/*skips first line of file "f", Function taken from 
skeleton/program compiled by the comp20003 teaching 
team, the university of Melbourne.*/
void skip_footpath_header(FILE *f);

/*reads in the data from each row of csv file from 
"footpath_row"*/
footpath_t* read_footpath(FILE* infile);

/*reads and returns char string from CSV file. Function 
written by Liam Silba, COMP2003 teaching team*/
char *readCSVstring(FILE* infile);

/*Space allocation of structure footpath_t*/
footpath_t* allocate_footpath_data_space(void);

/*Free space allocate to structure footpath_t*/
void free_footpath_data_space(footpath_t* footpath);


/**Functions to access specific fields from footpath structure, which 
                    cannot be accessed directly in other funcitons**/

/*Gets footpath ID*/
int get_footpath_id(footpath_t *footpath_info);
rectangle_t* get_rectangle(footpath_t* data);

/*Inserts corners information into node*/
footpath_t* insert_rectangle_coordinates_in_data(
            rectangle_t* coordinates, footpath_t* data);

            
/*Inserts footpath  id*/
void insert_footpath_id(footpath_t* footpath, int id);


/*Prints final output into File*/
void print_footpath_into_file(footpath_t* footpath,point_t* point,
                                    FILE *outputFile);

/*Personal assistance
void print_footpath_info(footpath_t *footpath_info);*/

#endif
