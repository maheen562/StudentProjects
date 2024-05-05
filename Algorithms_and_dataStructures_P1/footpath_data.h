#ifndef _FOOTPATH_H_
#define _FOOTPATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*typedef structure for "footpath_t"*/
typedef struct footpath footpath_t;

/*skips first line of file "f", Function taken from 
skeleton/program compiled by the comp20003 teaching 
team, the university of Melbourne.*/
void skip_footpath_header(FILE *f);

/*reads in the data from each row of csv file from 
"footpath_row"*/
footpath_t* read_footpath(char* footpath_row);

/*returns data of "required_col" from csv file row. returns data 
in normalized form*/
char* fetch_field_from_row(char* input_line, int required_col);

/*prints footpath information into file "outputFile"*/
void print_footpath_into_file(footpath_t* footpath,FILE *outputFile);

/*returns tokenized strings that include blank spaces*/
char* get_string(int type, char* input_line,int* index);

/*Space allocation of structure footpath_t*/
footpath_t* allocae_footpath_data_space(void);

/*Free space allocate to structure footpath_t*/
void free_footpath_data_space(footpath_t* footpath);


/**Functions to access specific fields from footpath structure, which 
                    cannot be accessed directly in other funcitons**/

/*Gets footpath ID*/
int get_footpath_id(footpath_t *footpath_info);

/*Gets footpath adress*/
char* get_adress(footpath_t* footpath_data);

/*Gets footpath grade1in*/
float get_grade1in(footpath_t* footpath_data);


char* fetch_field_from_row2(char*, int, char*);

/**Funtions that deal with arrays : dict2**/

/*Recieves grade1in "data" value from linklist and enters it 
into a sorted array*/
void create_sorted_array(footpath_t* data,footpath_t* 
                        footpath_array[],int* index);

/*Shifts all array elements one index down "current" to create
 sorted array*/
void shift_array_elements(footpath_t* footpath_array[],
                         int current,int end);

/**Grade1in functions : dict2**/

/*compares value of input file grade1in with the values 
of footpath array*/
void compare_grade1in(footpath_t* footpath_array[],int index,
                        FILE* outFile,FILE* inFile);

/*searches for closest value to "current_grade1in" in "footpath_array"*/                        
int closest_value_in_arr(double current_grade1in,
 footpath_t* footpath_array[],int index);

/*prints closest grade1in info to stdout and file*/ 
void print_closet_grade1in(double current_value,
footpath_t* footpath_data,FILE *outFile);

/**Personal assistance**/
//void print_array(footpath_t* arr[], int index);
//void print_footpath_info(footpath_t *footpath_info);
#endif
