#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "rectangle.h"

#define EPSILON 10e-12/*Epsilon value for comparison*/

/*typedef structure for "point_t"*/
struct point2D{
    long double lon;
    long double lat;
};

/*typedef structure for "rectangle_t"*/
struct rectangle2D{
    point_t start;
    point_t end;
};

/*Compares two point*/
int compare_point(point_t* point1, point_t* point2){

    if(modulus(point1->lat - point2->lat) <EPSILON && modulus(point1->lon - point2->lon) <EPSILON ){
        return 1;
    }
    return 0;
}
/**Functions to access or insert specific fields from or to rectangle structure,
                 which cannot be accessed directly in other funcitons**/

long double get_lat(point_t* point){
    return point->lat;
}

long double get_lon(point_t* point){
    return point->lon;
}

point_t* get_start_point(rectangle_t* rect){
    return &rect->start;
}

point_t* get_end_point(rectangle_t* rect){
    return &rect->end;
}

void insert_start_lat(rectangle_t* rect,long double coordinate){

    rect->start.lat = coordinate;
}

void insert_start_lon(rectangle_t* rect, long double coordinate){

    rect->start.lon = coordinate;
}

void insert_end_lat(rectangle_t* rect, long double coordinate){

    rect->end.lat = coordinate;
}

void insert_end_lon(rectangle_t* rect, long double coordinate){

    rect->end.lon = coordinate;
}

long double get_start_lat(rectangle_t* rect){

    return rect->start.lat;
}

long double get_start_lon(rectangle_t* rect){

    return rect->start.lon;
}

long double get_end_lat(rectangle_t* rect){

    return rect->end.lat;
}


long double get_end_lon(rectangle_t* rect){

    return rect->end.lon;
}


/*Creates rectangle from points*/
rectangle_t* create_rectangle_from_coordinates(rectangle_t* coordinates,
        double lon_top, double lat_top, double lon_bottom, double lat_bottom){

    coordinates->start.lon = lon_top;
    coordinates->start.lat = lat_top;
    coordinates->end.lon = lon_bottom;
    coordinates->end.lat = lat_bottom;

    return coordinates;

}



point_t* insert_point(long double lat,long double lon){
    point_t* point = allocate_point_space();
    point->lat = lat;
    point->lon = lon;
    
    return point;
}




/*Functions for Space allocation*/

rectangle_t* allocate_rectangle_space(){

    rectangle_t* rectangle = (rectangle_t*)malloc(sizeof(*rectangle));

    assert(rectangle);

    return rectangle;
}
point_t* allocate_point_space(){
    point_t* point = (point_t*)malloc(sizeof(*point));
    assert(point);

    return point;
}

/*Functions for free space*/
void free_rectangle_space(rectangle_t* rectangle){
    free(rectangle);
}

void free_point_space(point_t* point){

    free(point);
}
/*Prints std out*/
void print_point(point_t* point){
    printf("%Lf %Lf --> ",point->lon,point->lat);
}

/*Calculates mid points*/
point_t* mid_point(rectangle_t* rectangle){


    point_t* mid = (point_t*)malloc(sizeof(*mid));
    assert(mid);
    
    mid->lat = (rectangle->start.lat +  rectangle->end.lat)/2.0;
    mid->lon = (rectangle->start.lon +  rectangle->end.lon)/2.0;

    return mid;



}

/*Modulus of point, used for comparison*/
long double modulus(long double point){
    if (point<0){
        return -1.0*point;
    }

    return point;

}

/*Gets file from point*/
point_t* get_point_from_file(FILE* inFile){

    point_t* point = allocate_point_space();
    fscanf(inFile,"%Lf %Lf\n",&point->lon,&point->lat);

    return point;
}



/*Personal use
void print_rectangle(rectangle_t* coordinates){

    printf("x_top = %Lf",coordinates->start.lat);
    printf(" y_top = %Lf",coordinates->start.lon);
    printf(" x_bottom = %Lf",coordinates->end.lat);
    printf(" y_bottom = %Lf\n",coordinates->end.lon);

}
*/


