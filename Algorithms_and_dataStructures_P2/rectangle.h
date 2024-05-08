#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include <stdio.h>
#include <stdlib.h>

/*typedef structure for "point_t"*/
typedef struct point2D point_t;
/*typedef structure for "rectangle_t"*/
typedef struct rectangle2D rectangle_t;
/*typedef structure for "footpath_t"*/
typedef struct footpath footpath_t;

/*Space Allocation*/
rectangle_t* allocate_rectangle_space();
point_t* allocate_point_space();


/*Calculates mid point*/
point_t* mid_point(rectangle_t* rectangle);
//int determine_quadrant(rectangle_t* parent_node,point_t* point_to_insert,point_t*  mid_point);

//prints std out
void print_point(point_t* point);

/**Functions to access or insert specific fields from or to rectangle structure,
                 which cannot be accessed directly in other funcitons**/
void insert_rectangle_coordinates(rectangle_t* coordinates,
        long double x_top,long double y_top,long double x_bottom,
                    long double y_bottom);

rectangle_t* create_rectangle_from_coordinates(rectangle_t* coordinates,
    double lon_top, double lat_top, double lon_bottom, double lat_bottom);

point_t* insert_point(long double lat,long double lon);
void insert_start_lat(rectangle_t* rect, long double coordinate);
void insert_start_lon(rectangle_t* rect, long double coordinate);
void insert_end_lat(rectangle_t* rect, long double coordinate);
void insert_end_lon(rectangle_t* rect, long double coordinate);

long double get_start_lat(rectangle_t* rect);
long double get_start_lon(rectangle_t* rect);
long double get_end_lat(rectangle_t* rect);
long double get_end_lon(rectangle_t* rect);

point_t* get_start_point(rectangle_t* rect);
point_t* get_end_point(rectangle_t* rect);

long double get_lat(point_t* point);
long double get_lon(point_t* point);

/*Frees space*/
void free_rectangle_space(rectangle_t* rectangle);
void free_point_space(point_t* point);

/*modulus for close point comparisons*/
long double modulus(long double point);

/*compares two points*/
int compare_point(point_t* point1, point_t* point2);

/*gets point from file*/
point_t* get_point_from_file(FILE* inFile);

/*Printing Personal assistance*/
void print_rectangle(rectangle_t* coordinates);
void print_point(point_t* point);
#endif
