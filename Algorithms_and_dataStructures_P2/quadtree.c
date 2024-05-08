#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "footpath_data.h"
#include "rectangle.h"
#include "quadtree.h"

#define NULL_QUADRANT 0 /*check if quadrant of node is NULL*/
#define NOT_NULL_QUADRANT 1 /*check if quadrant of node is NOT NULL*/
#define NO_OF_QUADRANTS 4 /*total number of quadrants*/
#define OUT_OF_RANGE 0 /*check if out of range*/
#define QUADRANT_SW 1   /*1st quadrant*/
#define QUADRANT_NW 2   /*2nd quadrant*/
#define QUADRANT_NE 3   /*3rd quadrant*/
#define QUADRANT_SE 4   /*4th quadrant*/
#define QUADRANT_SW_CH "SW" /*1st quadrant str value*/
#define QUADRANT_NW_CH "NW" /*2nd quadrant str value*/
#define QUADRANT_NE_CH "NE" /*3rd quadrant str value*/
#define QUADRANT_SE_CH "SE" /*4th quadrant str value*/
#define EPSILON 10e-12

/*Structure for quadtree node, which store PR tree 
information*/

struct quadTree_node
{
    data_t *data;
    point_t *node_coordinate;
    rectangle_t *corners;
    quadTreenode_t *SW;
    quadTreenode_t *NW;
    quadTreenode_t *NE;
    quadTreenode_t *SE;
    int quadrant_info[4];
    char quadrant_name[2];
};

/*Allocates node space including footpath,node_coordinate
and rectangle space*/
quadTreenode_t *allocate_node_space()
{
    quadTreenode_t *node = (quadTreenode_t *)malloc(sizeof(*node));
    assert(node);
    node->data = allocate_footpath_data_space();
    node->node_coordinate = allocate_point_space();
    node->corners = allocate_rectangle_space();

    assert(node->node_coordinate);
    assert(node->data);
    assert(node->corners);

    for (int i = 0; i < NO_OF_QUADRANTS; i++)
    {
        node->quadrant_info[i] = NULL_QUADRANT;
    }
    node->SW = node->NW = node->NE = node->SE = NULL;

    return node;
}


/*Makes the root node and add coordinates*/
quadTreenode_t *make_empty_quadTree(rectangle_t *root_coordinates)
{

    quadTreenode_t *empty_tree = allocate_node_space();
    empty_tree->corners = root_coordinates;

    return empty_tree;
}

/*Adds point to PR Tree*/
void addpoint(data_t *data_of_point, point_t *point_to_insert, 
    quadTreenode_t *parent_node, int quadrant, point_t *mid, rectangle_t *corners)
{

    parent_node->quadrant_info[quadrant - 1] = NOT_NULL_QUADRANT;
    if (quadrant == QUADRANT_SW)
    {
        parent_node->SW = insert_point_in_tree(data_of_point, parent_node->SW, 
            quadrant, point_to_insert, corners, QUADRANT_SW_CH);
    }
    else if (quadrant == QUADRANT_NW)
    {
        parent_node->NW = insert_point_in_tree(data_of_point, parent_node->NW, 
            quadrant, point_to_insert, corners, QUADRANT_NW_CH);
    }
    else if (quadrant == QUADRANT_NE)
    {
        parent_node->NE = insert_point_in_tree(data_of_point, parent_node->NE,
             quadrant, point_to_insert, corners, QUADRANT_NE_CH);
    }
    else if (quadrant == QUADRANT_SE)
    {
        parent_node->SE = insert_point_in_tree(data_of_point, parent_node->SE, 
            quadrant, point_to_insert, corners, QUADRANT_SE_CH);
    }   
}

/*inserts rectangle coordinates of start and end point, has call
         to recursice function*/
void insert_rectangle_coordinates_in_tree(data_t *data, quadTreenode_t *root)
{

    root = recursuve_insert_of_point(data, get_start_point(get_rectangle(data)), root);

    root = recursuve_insert_of_point(data, get_end_point(get_rectangle(data)), root);
}

/*Inserts data of point into tree*/
quadTreenode_t *insert_point_in_tree(data_t *data, quadTreenode_t *node, 
int quadrant, point_t *point_to_insert, rectangle_t *corners, char *quadrant_name)
{

    node = allocate_node_space();
    assert(node);
    node->data = data;
    node->node_coordinate = allocate_point_space();
    node->node_coordinate = point_to_insert;
    node->corners = corners;

    strcpy(node->quadrant_name, quadrant_name);


    return node;
}

/*finding point with binary search*/
int point_binary_search(quadTreenode_t *parent_node, point_t *point_to_search,
         int found, char *return_string,quadTreenode_t* data_to_print)
{

    point_t *mid = mid_point(parent_node->corners);
    rectangle_t *corners = allocate_rectangle_space();

    int quadrant = determine_quadrant(parent_node->corners,
                                         point_to_search, mid, corners);
    if (quadrant != 0)
    {

        quadTreenode_t *quadrant_node = get_quadrant_node(
                                parent_node, quadrant);
        if (quadrant_node == NULL)
        {
            return 0;
        }
        if (compare_point(point_to_search, quadrant_node->node_coordinate))
        {
            data_to_print->data = quadrant_node->data;
            data_to_print->corners = quadrant_node->corners;
            strcat(return_string, quadrant_node->quadrant_name);
            point_to_search = quadrant_node->node_coordinate;
            return 1;
        }
        else if (check_if_leaf_node(quadrant_node) == 1)
        {
            return 0;
        }
        else
        {
            strcat(return_string, quadrant_node->quadrant_name);
            point_binary_search(quadrant_node, point_to_search, found, return_string,data_to_print);
        }
    }
    return found;
}

/*For std out*/
void print_output(point_t *point, char *output_string)
{
    print_point(point);
    printf("%s\n", output_string);
}


/*returns corners of node*/
rectangle_t* get_corners(quadTreenode_t* node){
    return node->corners;
}

void copy_node(quadTreenode_t *node_to, quadTreenode_t *node_from)
{

    node_to = node_from;
    node_to->corners = node_from->corners;
    node_to->node_coordinate = node_from->node_coordinate;
    node_to->data = node_from->data;
}


/*Is supposed to free unwanted node data when point pushed down. */
void free_unwanted_node_data(quadTreenode_t *node)
{
    free_footpath_data_space(node->data);
    free_point_space(node->node_coordinate);
}


/*Recursive function to find place of insertion, calls addpoint*/

quadTreenode_t *recursuve_insert_of_point(data_t *data_of_point,
             point_t *point_to_insert, quadTreenode_t *parent_node)
{

    point_t *mid = mid_point(parent_node->corners);
    rectangle_t *corners = allocate_rectangle_space();

    int quadrant = determine_quadrant(parent_node->corners, 
    point_to_insert, mid, corners);
    if (quadrant != 0)
    {

        if (parent_node->quadrant_info[quadrant - 1] == NULL_QUADRANT)
        {

            addpoint(data_of_point, point_to_insert, parent_node,
             quadrant, mid, corners);
        }
        else
        {
            quadTreenode_t *temp_quadrant_node = get_quadrant_node(
                                                parent_node, quadrant);
            move_point_down(temp_quadrant_node->data,
                 temp_quadrant_node->node_coordinate, temp_quadrant_node);

            parent_node = recursuve_insert_of_point(data_of_point,
            point_to_insert, get_quadrant_node(parent_node, quadrant));

        }
    }

    return parent_node;
}

/*Moves point of node down, used when two points are located in the same quadrant*/
void move_point_down(data_t *data_of_point, point_t *point_to_insert, quadTreenode_t *parent_node)
{
    point_t *mid = mid_point(parent_node->corners);
    rectangle_t *corners = allocate_rectangle_space();

    int quadrant = determine_quadrant(parent_node->corners, point_to_insert, mid, corners);

    addpoint(data_of_point, point_to_insert, parent_node, quadrant, mid, corners);
}


/*Gets required quadrant of node*/
quadTreenode_t *get_quadrant_node(quadTreenode_t *parent_node, int quadrant)
{
    if (quadrant == 1)
    {
        return parent_node->SW;
    }
    else if (quadrant == 2)
    {
        return parent_node->NW;
    }
    else if (quadrant == 3)
    {
        return parent_node->NE;
    }
    else if (quadrant == 4)
    {
        return parent_node->SE;
    }
    return NULL;
}


/*determines in which quadrant does the point to insert go to*/
int determine_quadrant(rectangle_t *parent_node, point_t *point_to_insert,
         point_t *mid_point, rectangle_t *corners)
{

    int quadrant = OUT_OF_RANGE;

    point_t *SW_start_point = insert_point(get_lat(mid_point), get_lon(mid_point));
    point_t *SW_end_point = insert_point(get_end_lat(parent_node), get_end_lon(parent_node));

    point_t *NW_start_point = insert_point(get_start_lat(parent_node), get_lon(mid_point));
    point_t *NW_end_point = insert_point(get_lat(mid_point), get_end_lon(parent_node));
    point_t *NE_start_point = insert_point(get_start_lat(parent_node), get_start_lon(parent_node));
    point_t *NE_end_point = insert_point(get_lat(mid_point), get_lon(mid_point));
    point_t *SE_start_point = insert_point(get_lat(mid_point), get_start_lon(parent_node));
    point_t *SE_end_point = insert_point(get_end_lat(parent_node), get_lon(mid_point));

    if (in_quadrant_SW(SW_start_point, SW_end_point, point_to_insert))
    {
        corners = create_rectangle_from_coordinates(corners, get_lon(SW_start_point),
         get_lat(SW_start_point), get_lon(SW_end_point), get_lat(SW_end_point));

        quadrant = 1;
    }
    else if (in_quadrant_NW(NW_start_point, NW_end_point, point_to_insert))
    {
        corners = create_rectangle_from_coordinates(corners, get_lon(NW_start_point), 
        get_lat(NW_start_point), get_lon(NW_end_point), get_lat(NW_end_point));
        quadrant = 2;
    }
    else if (in_quadrant_NE(NE_start_point, NE_end_point, point_to_insert))
    {
        corners = create_rectangle_from_coordinates(corners, get_lon(NE_start_point),
         get_lat(NE_start_point), get_lon(NE_end_point), get_lat(NE_end_point));
        quadrant = 3;
    }
    else if (in_quadrant_SE(SE_start_point, SE_end_point, point_to_insert))
    {
        corners = create_rectangle_from_coordinates(corners, get_lon(SE_start_point), 
        get_lat(SE_start_point), get_lon(SE_end_point), get_lat(SE_end_point));
        quadrant = 4;
    }

    return quadrant;
}

/*gets data from node*/
data_t *get_data(quadTreenode_t *node)
{
    return node->data;
}

/**Functions to determine which quadrant the point belongs to.
 Compares points with end point of each quadrant**/
int in_quadrant_SW(point_t *start, point_t *end, point_t *point_to_insert)
{

    long double start_lat = get_lat(point_to_insert) - get_lat(start);
    long double end_lat = get_lat(point_to_insert) - get_lat(end);
    long double start_lon = get_lon(point_to_insert) - get_lon(start);
    long double end_lon = get_lon(point_to_insert) - get_lon(end);


    if (start_lat < 0.0 &&
        end_lat > 0.0 &&
        start_lon <= 0.0 &&
        end_lon >= 0.0)
    {
        return 1;
    }
    return 0;
}

int in_quadrant_NW(point_t *start, point_t *end, point_t *point_to_insert)
{


    long double start_lat = get_lat(point_to_insert) - get_lat(start);
    long double end_lat = get_lat(point_to_insert) - get_lat(end);
    long double start_lon = get_lon(point_to_insert) - get_lon(start);
    long double end_lon = get_lon(point_to_insert) - get_lon(end);

    if (start_lat <= 0.0 &&
        end_lat >= 0.0 &&
        start_lon <= 0.0 &&
        end_lon >= 0.0)
    {
        return 1;
    }
    return 0;
}

int in_quadrant_NE(point_t *start, point_t *end, point_t *point_to_insert)
{

    long double start_lat = get_lat(point_to_insert) - get_lat(start);
    long double end_lat = get_lat(point_to_insert) - get_lat(end);
    long double start_lon = get_lon(point_to_insert) - get_lon(start);
    long double end_lon = get_lon(point_to_insert) - get_lon(end);

    if (start_lat <= 0.0 &&
        end_lat >= 0.0 &&
        start_lon < 0.0 &&
        end_lon > 0.0)
    {
        return 1;
    }
    return 0;
}

int in_quadrant_SE(point_t *start, point_t *end, point_t *point_to_insert)
{

    long double start_lat = get_lat(point_to_insert) - get_lat(start);
    long double end_lat = get_lat(point_to_insert) - get_lat(end);
    long double start_lon = get_lon(point_to_insert) - get_lon(start);
    long double end_lon = get_lon(point_to_insert) - get_lon(end);


    if (start_lat < 0.0 &&
        end_lat > 0.0 &&
        start_lon < 0.0 &&
        end_lon > 0.0)
    {
        return 1;
    }
    return 0;
}

//checks if node is leaf node
int check_if_leaf_node(quadTreenode_t *node)
{
    if (node->SW == NULL && node->NW == NULL &&
             node->NE == NULL && node->SE == NULL)
    {
        return 1;
    }
    return 0;
}

/*Personal help node print out

void print_node(quadTreenode_t *top)
{
    // printf("Yoo");
    printf("parent = %p\n", top);
    print_rectangle(top->corners);
    printf("\n");
    print_point(top->node_coordinate);

    for (int i = 0; i < 4; i++)
    {
        printf(" %d ", top->quadrant_info[i]);
    }
    printf("\n");
    printf("SW = %p\n", top->SW);
    printf("NW = %p\n", top->NW);
    printf("NE = %p\n", top->NE);
    printf("SE = %p\n", top->SE);
}*/


