#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <stdio.h>
#include <stdlib.h>

#include "footpath_data.h"
#include "rectangle.h"

typedef footpath_t data_t;/*typedef for data which contains footpath_t
                                    structure*/
typedef struct quadTree_node quadTreenode_t;/*typedef for nodes*/

/*Allocates node space including footpath,node_coordinate
and rectangle space*/
quadTreenode_t* allocate_node_space();

/*Makes the root node and add coordinates*/
quadTreenode_t* make_empty_quadTree(rectangle_t* root_coordiantes);

/*Adds point to PR Tree*/
void addpoint(data_t* data_of_point,point_t* point_to_insert,
     quadTreenode_t* tree_node,int quadrant, point_t* mid,
     rectangle_t* corners);

/*inserts rectangle coordinates of start and end point, has call
         to recursice function*/
void insert_rectangle_coordinates_in_tree(data_t* data, quadTreenode_t *root);

/*Inserts data of point into tree*/
quadTreenode_t* insert_point_in_tree(data_t* data,quadTreenode_t* node,
        int quadrant,point_t* point_to_insert,rectangle_t* corners,
        char* quadrant_name);


/*Recursive function to find place of insertion, calls addpoint*/
quadTreenode_t* recursuve_insert_of_point(data_t* data_of_point,
        point_t* point_to_insert, quadTreenode_t* parent_node);

/*Moves point of node down, used when two points are located in the same quadrant*/
void move_point_down(data_t* data_of_point, point_t* point_to_insert,
 quadTreenode_t* parent_node);

/*Gets required quadrant of node*/
quadTreenode_t* get_quadrant_node(quadTreenode_t* parent_node,
                    int quadrant);

/*Takes modulus*/
long double modulus(long double point);

//checks if node is leaf node
int check_if_leaf_node(quadTreenode_t *node);

/*determines in which quadrant does the point to insert go to*/
void insert_node_in_quadrant(data_t* data,quadTreenode_t* parent_node,int quadrant);

/*gets data from node*/
data_t* get_data(quadTreenode_t* node);

/*determines in which quadrant does the point to insert go to*/
int determine_quadrant(rectangle_t* parent_node,
        point_t* point_to_insert,point_t*  mid_point,rectangle_t* corners);

/*returns corners of node*/
rectangle_t* get_corners(quadTreenode_t* node);

/*Is supposed to free unwanted node data when point pushed down. */
void free_unwanted_node_data(quadTreenode_t* node);
/*finding point with binary search*/
int point_binary_search(quadTreenode_t *parent_node, point_t *point_to_search,
    int found, char *return_string,quadTreenode_t* data_to_print);

/*For std out*/
void print_output(point_t* point,char* output_string);

/**Functions to determine which quadrant the point belongs to.
 Compares points with end point of each quadrant**/
int in_quadrant_SW(point_t* start,point_t* end, point_t* point_to_insert);
int in_quadrant_NW(point_t* start,point_t* end, point_t* point_to_insert);
int in_quadrant_NE(point_t* start,point_t* end, point_t* point_to_insert);
int in_quadrant_SE(point_t* start,point_t* end, point_t* point_to_insert);

//Personal use: printing statement
//void print_node(quadTreenode_t *root);

#endif
