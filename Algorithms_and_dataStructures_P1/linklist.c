/*
 *
 *Project: dict1 dict2
 *linklist.h :
 *         = assisgnment 1 implementation of Footpath Data Structures
 * Linked list defs:
 *
 * Adapted from implementation by Alistair Moffat: Jianzhong Qi, April 2022
 * Edited by: Maheen Abdul Khaliq Khan, 1193813
 *Project: dict1 dict2
  footpath_data.c :
          = assisgnment 1 implementation of Footpath Data Structures
 * 
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "linklist.h"
#include "footpath_data.h"

#define MAX_ROW_LENGTH 512
#define MAX_LENGTH_OF_ADRESS 128

/**For nodes and linked lists. Mainly used in dict1**/

/*Structure for "node" of linklist*/
struct node {
	data_t *data;
	node_t *next;
};

/*Structure for "list"*/
struct list {
	node_t *head;
	node_t *foot;
};

/*Allocates memory and makes an empty list */
list_t
*make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

/*Checks if list is empty*/
int
is_empty_list(list_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}

/*frees list*/
void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free_footpath_data_space(prev->data); /*frees footpath data in list*/
		free(prev);
	}
	free(list);
}

/*frees node*/
void free_node(node_t* node){
	assert(node!=NULL);
	free(node);
}

/*Inserts data at the start of the list*/
list_t
*insert_at_head(list_t *list, data_t *value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = list->head;
	list->head = new;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->foot = new;
	}
	return list;
}

/*Inserts data at the end of the list*/
list_t
*insert_at_foot(list_t *list, data_t *value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}



/*returns node to the start of the list*/
node_t* get_head_pointer(list_t *list) {
	//assert(list!=NULL && list->head!=NULL);
	return list->head;
}

/*returns data from the start of the list*/
data_t* get_head(list_t *list) {
	assert(list!=NULL && list->head!=NULL);
	return list->head->data;
}

/*returns data from the end of the list*/
list_t
*get_tail(list_t *list) {
	node_t *oldhead;
	assert(list!=NULL && list->head!=NULL);
	oldhead = list->head;
	list->head = list->head->next;
	if (list->head==NULL) {
		/* the only list node just got deleted */
		list->foot = NULL;
	}
	free(oldhead);
	return list;
}

/*Assembles city_footpaths linklist*/
list_t *create_footpath_linkedlist(list_t*city_footpaths,FILE* dataFile){
	 char* each_row = (char*)malloc(MAX_ROW_LENGTH*sizeof(*each_row));

	data_t* each_footpath_data = allocae_footpath_data_space();

	while(fgets(each_row,MAX_ROW_LENGTH,dataFile)!=NULL){
        each_footpath_data =  read_footpath(each_row);
        insert_at_foot(city_footpaths,each_footpath_data);
    }

	free(each_row);
	return city_footpaths;
 }

/*Search for same adress as stdin file*/
void search_adresses(list_t* city_footpaths,FILE* inFile,FILE* outFile){

	char* current_adress;
    char* search = (char*)malloc(MAX_LENGTH_OF_ADRESS*sizeof(*search));

    node_t* node_for_search = create_empty_node();
    footpath_t* footpath_data_in_node = allocae_footpath_data_space();
    node_for_search = get_head_pointer(city_footpaths);
    char *search_adress;
    int found = 0;

    while(fgets(search,MAX_LENGTH_OF_ADRESS,inFile)){

		search_adress = (char*)malloc((MAX_LENGTH_OF_ADRESS+1)*
							sizeof(*search_adress));
        
		strcpy(search_adress,search);
		if(search_adress[(strlen(search_adress)-1)] == '\n'){
			search_adress[(strlen(search_adress)-1)] = '\0';
		}  
		//search_adress[st]
		fprintf(outFile,"%s\n",search_adress);      
         while(node_for_search != NULL){
            footpath_data_in_node = get_data(node_for_search);
            current_adress = get_adress(footpath_data_in_node);

            if(strcmp(current_adress,search_adress) == 0){
                print_footpath_into_file(footpath_data_in_node,outFile);
                found++;
            }
            node_for_search  = get_next_node(node_for_search);
        }
		if (found == 0){
			printf("%s --> NOTFOUND\n",search_adress);
		}
		else{
        	printf("%s --> %d\n",search_adress,found);
		}
        found = 0;
        node_for_search = get_head_pointer(city_footpaths);
		free(search_adress);
    }

	
	free(search);


}

/*Creates an empty node*/
node_t *create_empty_node(void){
	node_t *node;
	node = (node_t*)malloc(sizeof(*node));
	assert(node!=NULL);
	return node;
}
/**Functions to access internal structure, for where it is not 
accessible**/

/*Gets data from node, for where structure is not accessible*/
data_t *get_data(node_t* node){
	return node->data;
}

/*Gets next node from node structure, for where structure 
is not accessible*/
node_t* get_next_node(node_t *node){
	return node->next;
}

/*Inserts data in node structure, for where structure 
is not accessible*/
void insert_data_in_node(node_t *node, list_t *list){
	insert_at_foot(list,node->data);
}

/* =====================================================================
   Part of this Program written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */
