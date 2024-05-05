/*-------------------------------------------------------------- 
.Project: dict1 dict2
  linklist.h :
          = assisgnment 1 implementation of Footpath Data Structures

 * Adapted from implementation by Alistair Moffat: Jianzhong Qi, April 2022
 * Edited by: Maheen Abdul Khaliq Khan, 1193813
 *Project: dict1 dict2
  footpath_data.c :
          = assisgnment 1 implementation of Footpath Data Structures

----------------------------------------------------------------*/
#ifndef _LINKLIST_H_
#define _LINKLIST_H_
#include <assert.h>
#include <stdlib.h>


#include "footpath_data.h"

typedef footpath_t data_t;/*typedef for data which contains footpath_t
                                    structure*/
typedef struct node node_t;/*typedef for nodes*/
typedef struct list list_t;/*typedef for list*/


/*Allocates memory and makes an empty list */
list_t *make_empty_list(void);

/*Checks if list is empty*/
int is_empty_list(list_t *list);

/*frees list*/
void free_list(list_t *list);

/*frees node*/
void free_node(node_t* node);

/*Inserts data at the start of the list*/
list_t *insert_at_head(list_t *list, data_t *value);

/*Inserts data at the end of the list*/
list_t *insert_at_foot(list_t *list, data_t *value);

/*returns data from the start of the list*/
data_t* get_head(list_t *list); 

/*returns data from the end of the list*/
list_t *get_tail(list_t *list);

/*Creates an empty node*/
node_t *create_empty_node(void);

/*Assembles city_footpaths linklist*/
list_t *create_footpath_linkedlist(list_t*city_footpaths,FILE* dataFile);

/*Search for same adress as stdin file*/
void search_adresses(list_t* city_footpaths,FILE* inFile,FILE*outFile);

/**Functions to access internal structure, for where it is not 
accessible**/

/*Gets data from node, for where structure is not accessible*/
data_t *get_data(node_t* node);

/*Gets next node from node structure, for where structure 
is not accessible*/
node_t* get_next_node(node_t *node);

/*Inserts data in node structure, for where structure 
is not accessible*/
void insert_data_in_node(node_t *node, list_t *list);

/*returns node to the start of the list*/
node_t* get_head_pointer(list_t *list);

#endif

/* =====================================================================
   Part of this Program is written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */
