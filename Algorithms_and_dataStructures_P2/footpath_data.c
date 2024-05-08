#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "footpath_data.h"
#include "rectangle.h"

#define ADRESS_MAX_LENGTH 128 /*Max length of adress*/
#define ASSET_TYPE_MAX_LENGTH 128 /*Max length of 
                                        asset_type*/
#define CLUE_SA_MAX_LENGTH 128 /*Max length of chars in
                                    footpath*/
#define SEGSIDE_MAX_LENGTH 128
#define MAX_LINE_LEN 128



/* structure for "footpath_t"*/
struct footpath{

    int footpath_id;
    char* adress;
    char* clue_sa;
    char* asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char* segside;
    int statusid;
    int streetid;
    int street_group;
    rectangle_t* coordinates;

};



/*skips first line of file "f", Function taken from 
skeleton/program compiled by the comp20003 teaching 
team, the university of Melbourne.*/
void skip_footpath_header(FILE *f){
    while (fgetc(f) != '\n');
}

/*reads in the data from each row of csv file from 
"footpath_row"*/
footpath_t* read_footpath(FILE* infile){

     footpath_t *temp_f = allocate_footpath_data_space();
     assert(temp_f);
     if(fscanf(infile," %d, ",&temp_f->footpath_id)!=1){
         return NULL;
     }
     temp_f->adress = readCSVstring(infile);
     temp_f->clue_sa = readCSVstring(infile);
     temp_f->asset_type = readCSVstring(infile);
     fscanf(infile," %lf, ",&temp_f->deltaz);
     fscanf(infile," %lf, ",&temp_f->distance);
     fscanf(infile," %lf, ",&temp_f->grade1in);

     double temp=0.0;
     fscanf(infile," %lf, ",&temp);
     temp_f->mcc_id = temp;
     fscanf(infile," %lf, ",&temp);
     temp_f->mccid_int = temp;
     fscanf(infile," %lf, ",&temp_f->rlmax);
     fscanf(infile," %lf, ",&temp_f->rlmin);
     temp_f->segside = readCSVstring(infile);

     fscanf(infile," %lf, ",&temp);
     temp_f->statusid = temp;
     fscanf(infile," %lf, ",&temp);
     temp_f->streetid = temp;
     fscanf(infile," %lf, ",&temp);
     temp_f->street_group = temp;

    temp_f->coordinates = allocate_rectangle_space();
     long double double_temp;
     fscanf(infile," %Lf, ",&double_temp);
     insert_start_lat(temp_f->coordinates,double_temp);
     fscanf(infile," %Lf, ",&double_temp);
     insert_start_lon(temp_f->coordinates,double_temp);
     fscanf(infile," %Lf, ",&double_temp);
     insert_end_lat(temp_f->coordinates,double_temp);
     fscanf(infile," %Lf, ",&double_temp);
     insert_end_lon(temp_f->coordinates, double_temp);

     //print_footpath_info(temp_f);
     return temp_f;

}

/*reads and returns char string from CSV file. Function
     written by Liam Silba, COMP2003 teaching team*/
char *readCSVstring(FILE* infile){
    char res[MAX_LINE_LEN+1];
    if(fscanf(infile," \"%[^\"]\",", res) == 1 || 
                fscanf(infile," %[^,],",res) == 1){
        return strdup(res);
    }
    if(fscanf(infile," %[,]", res) == 1){
        return strdup("");
    }
    return NULL;
}

/*Allocates footpath space*/
footpath_t* allocate_footpath_data_space(void){
    footpath_t* data= (footpath_t*)malloc(sizeof(*data));
    assert(data);
    return data;

}

/*Free space allocate to structure footpath_t*/
void free_footpath_data_space(footpath_t* footpath){
    free(footpath->adress);
    free(footpath->clue_sa);
    free(footpath->asset_type);
    free(footpath->segside);
    free_rectangle_space(footpath->coordinates);
    free(footpath);
}

/**Functions to access specific fields from footpath structure, which 
                    cannot be accessed directly in other funcitons**/

/*Inserts footpath  id*/
void insert_footpath_id(footpath_t* footpath, int id){
    footpath->footpath_id = id;
    
}

/*Gets footpath ID*/
int get_footpath_id(footpath_t *footpath_info){
    return(footpath_info->footpath_id);
}
/*Gets footpath rectangle*/
rectangle_t* get_rectangle(footpath_t* data){
    return data->coordinates;
}


/*Inserts corners information into node*/
footpath_t* insert_rectangle_coordinates_in_data(rectangle_t*   
                             coordinates, footpath_t* data){    
    
    data->coordinates = coordinates;

    return data;

}

/*Prints final output into File*/
void print_footpath_into_file(footpath_t* footpath,point_t* point,
                                    FILE *outputFile){

    fprintf(outputFile,"%.14Lf %.15Lf\n",get_lon(point),
        get_lat(point));
    
    fprintf(outputFile,"--> footpath_id: %d || address: %s "
    "|| clue_sa: %s || asset_type: %s || deltaz: %g || "
    "distance: %g || grade1in: %g || mcc_id: %d || "
    "mccid_int: %d || rlmax: %g || rlmin: %g || segside: %s "
    "|| statusid: %d || streetid: %d || street_group: %d ||"
    " start_lat: %.14Lf || start_lon: %.14Lf|| end_lat: %.14Lf "
    "|| end_lon: %.14Lf || \n",
        footpath->footpath_id,footpath->adress,footpath->clue_sa,
        footpath->asset_type,footpath->deltaz,footpath->distance,
        footpath->grade1in,footpath->mcc_id,footpath->mccid_int,
        footpath->rlmax,footpath->rlmin,footpath->segside,
        footpath->statusid,footpath->streetid,footpath->street_group,
        get_start_lat(footpath->coordinates),get_start_lon(footpath->coordinates),
        get_end_lat(footpath->coordinates),
        get_end_lon(footpath->coordinates));

}



/*Personal assistance
void print_footpath_info(footpath_t *footpath_info){

    printf(  "%d\n  ",footpath_info->footpath_id);
    printf(  "%s\n  ",footpath_info->adress);
    printf(  "%s\n  ",footpath_info->clue_sa);
    printf(  "%s\n  ",footpath_info->asset_type); 
    printf(  "%lf  ",(footpath_info->deltaz));
    printf(  "%lf  ",(footpath_info->distance));
    printf(  "%lf  ",(footpath_info->grade1in));
    printf(  "%d ",(footpath_info->mcc_id));
    printf(  "%d ",(footpath_info->mccid_int));
    printf(  "%lf  ",(footpath_info->rlmax));
    printf(  "%lf  ",(footpath_info->rlmin));
    printf(  "%s  ",footpath_info->segside); 
    printf(  "%d  ",(footpath_info->statusid));
    printf(  "%d  ",(footpath_info->streetid));
    printf(  "%d  ",(footpath_info->street_group));
    printf(  "%Lf  ",get_start_lat(footpath_info->coordinates));
    printf(  "%Lf  ",get_start_lon(footpath_info->coordinates));
    printf(  "%Lf  ",get_end_lat(footpath_info->coordinates));
    printf(  "%Lf  ",get_end_lon(footpath_info->coordinates));
       // head_of_list = head_of_list->next;
    //}
}


*/

