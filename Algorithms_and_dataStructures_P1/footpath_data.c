/*-------------------------------------------------------------- 
.Project: dict1 dict2
  footpath_data.c :
          = assisgnment 1 implementation of Footpath Data Structures
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define ADRESS_MAX_LENGTH 128 /*Max length of adress*/
#define ASSET_TYPE_MAX_LENGTH 128 /*Max length of 
                                        asset_type*/
#define CLUE_SA_MAX_LENGTH 128 /*Max length of clue_sa*/
#define SEGSIDE_MAX_LENGTH 128 /*Max length of segside*/
#define ERROR "INVALID COLUMN INPUT\n"/*error to insert
                                            invalid col*/
#define MAX_COLUMNS 19 /*Max no of records in csv file*/
#define MAX_RECORD_ROWS 10000 /*Max no of rows in csv file*/
#define LENGTH_OF_ID_TENTHS 5 /*string length of id*/


/*Structure for "footpath_t"*/
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
    double start_lat;
    double start_lon;
    double end_lat;     
    double end_lon;

};
#include "footpath_data.h"

/*skips first line of file "f", Function taken from 
skeleton/program compiled by the comp20003 teaching 
team, the university of Melbourne.*/
void skip_footpath_header(FILE *f){
    while (fgetc(f) != '\n');
}

/*reads in the data from each row of csv file from 
"footpath_row"*/

footpath_t* read_footpath(char* footpath_row){

    footpath_t *temp_f = allocae_footpath_data_space();
    assert(temp_f);

    char* temp=NULL;
    
    temp = fetch_field_from_row(footpath_row,1);
    temp_f->footpath_id = atoi(temp);
    free(temp);

    temp_f->adress  = (char*)malloc((ADRESS_MAX_LENGTH+1)*
                                                sizeof(char));
    char* x=NULL;
    x=fetch_field_from_row(footpath_row,2);
    strcpy(temp_f->adress,x);
    free(x);


    temp_f->clue_sa  = (char*)malloc((CLUE_SA_MAX_LENGTH+1)*sizeof(char));
    x=fetch_field_from_row(footpath_row,3);
    strcpy(temp_f->clue_sa,x);
    free(x);

    temp_f->asset_type  = (char*)malloc((ASSET_TYPE_MAX_LENGTH+1)*sizeof(char));
    x=fetch_field_from_row(footpath_row,4);
    strcpy(temp_f->asset_type,x);
    free(x);
    
    temp=NULL;
    temp = fetch_field_from_row(footpath_row,5);
    temp_f->deltaz = atof(temp);
    free(temp);

    
    temp = fetch_field_from_row(footpath_row,6);
    temp_f->distance = atof(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,7);
    temp_f->grade1in = atof(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,8);
    temp_f->mcc_id = atoi(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,9);
    temp_f->mccid_int = atoi(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,10);
    temp_f->rlmax = atof(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,11);
    temp_f->rlmin = atof(temp);
    free(temp);

    temp_f->segside  = (char*)malloc((SEGSIDE_MAX_LENGTH+1)*sizeof(char));
    strcpy(temp_f->segside,fetch_field_from_row(footpath_row,12));

    temp = fetch_field_from_row(footpath_row,13);                                    
    temp_f->statusid = atoi(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,14);
    temp_f->streetid = atoi(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,15);
    temp_f->street_group = atoi(temp);
    

    temp = fetch_field_from_row(footpath_row,16);
    temp_f->start_lat = atof(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,17);
    temp_f->start_lon = atof(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,18);
    temp_f->end_lat = atof(temp);
    free(temp);

    temp = fetch_field_from_row(footpath_row,19);
    temp_f->end_lon = atof(temp);
    free(temp);
    
    //free(temp);
    return temp_f;
}



/*returns data of "required_col" from csv file row. returns data 
in normalized form*/



/*returns data of "required_col" from csv file row. returns data 
in normalized form*/
char* fetch_field_from_row(char* input_line, int required_col){
    
    const char* delimeter = ",";
    int current_col=1;
    char *field_value = 
        (char*)malloc(ADRESS_MAX_LENGTH*sizeof(*field_value));
    int found=0;
    int index=0;
    assert(field_value);
    int length_of_input_line = strlen(input_line);
    if(required_col == 1) {
        strncpy(field_value,input_line,LENGTH_OF_ID_TENTHS);
        field_value[LENGTH_OF_ID_TENTHS] = '\0';
        return field_value;
    }

    free(field_value);

    for(index=0;index<length_of_input_line && found == 0
         && required_col<=MAX_COLUMNS ;index++){
        field_value=NULL;     
        if(input_line[index] == delimeter[0]){
           index++;
            if(input_line[index] == '\"'){
                index++;
                field_value = get_string(1,input_line,&index);
            }
            else{
                if(input_line[index] == delimeter[0]){                    
                    field_value = strdup("\0");
                    index--;
                }
                else{
                    field_value = get_string
                            (2,input_line,&index);
                    index--;                  
                }
            }
            current_col++;
        }
        if(current_col == required_col){
            found = 1;
        }
        else if(field_value!=NULL){
            free(field_value);
        }
    }
    if(found==1){
        return field_value;
    }
    else{
        return ERROR;
    }
}


/*returns tokenized strings that include blank spaces*/
char* get_string(int type, char* input_line,
                                         int* index){   
    int current_index = *index;
    char* field_value= (char*)malloc(ADRESS_MAX_LENGTH*
                                    sizeof(*field_value));

    int field_index=0;
    char delimeter = '\0';
    char new_line= '\n';
    if(type == 1){
        delimeter = '\"';
    }
    else if (type == 2){
        delimeter = ',';
    }

    //printf("\n--INSIDE get string--\n");
        while((input_line[current_index] != delimeter)
             && (input_line[current_index] != new_line)){
            field_value[field_index] = input_line[current_index];
            field_index++;            
            current_index++;


        }
    field_value[field_index] = '\0';    
    *index = current_index;
    return field_value;

}

/*prints footpath information into file "outputFile"*/
void print_footpath_into_file(footpath_t* footpath,
                                    FILE *outputFile){
    
    fprintf(outputFile,"--> footpath_id: %d || address: %s "
    "|| clue_sa: %s || asset_type: %s || deltaz: %g || "
    "distance: %g || grade1in: %g || mcc_id: %d || "
    "mccid_int: %d || rlmax: %g || rlmin: %g || segside: %s "
    "|| statusid: %d || streetid: %d || street_group: %d ||"
    " start_lat: %.6lf || start_lon: %.6lf|| end_lat: %.6lf "
    "|| end_lon: %.6lf || \n",
        footpath->footpath_id,footpath->adress,footpath->clue_sa,
        footpath->asset_type,footpath->deltaz,footpath->distance,
        footpath->grade1in,footpath->mcc_id,footpath->mccid_int,
        footpath->rlmax,footpath->rlmin,footpath->segside,
        footpath->statusid,footpath->streetid,footpath->street_group,
        footpath->start_lat,footpath->start_lon,footpath->end_lat,
        footpath->end_lon);

}

/*Space allocation of structure footpath_t*/
footpath_t* allocae_footpath_data_space(void){
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
    free(footpath);
}

/**Functions to access specific fields from footpath structure, which 
                    cannot be accessed directly in other funcitons**/

/*Gets footpath ID*/
int get_footpath_id(footpath_t *footpath_info){
    return(footpath_info->footpath_id);
}

/*Gets footpath adress*/
char* get_adress(footpath_t* footpath_data){
    return footpath_data->adress;
}

/*Gets footpath grade1in*/
float get_grade1in(footpath_t* footpath_data){
    return footpath_data->grade1in;
}


/**Funtions that deal with arrays : dict2**/

/*Recieves grade1in "data" value from linklist and enters it 
into a sorted array*/
void create_sorted_array(footpath_t* data,
                    footpath_t* footpath_array[],int* index){
    
    int index_in_array = 0;
    int insert = 0;

    if(*index == 0){
        footpath_array[0] = data;
        *(index)+=1;
        insert = 1;
    }
    for(index_in_array=0;index_in_array<(*index) && 
                            (insert == 0);index_in_array++){

        if(get_grade1in(data) < get_grade1in
                        (footpath_array[index_in_array])){
            *(index)+=1;
            if(*index < MAX_RECORD_ROWS){                 
                shift_array_elements
                        (footpath_array,index_in_array,*index);
                footpath_array[index_in_array] = data;
                insert = 1;
            }

        }
    }

    if(insert == 0){ //inserts at very end if curr gratest
        *(index)+=1;
        if(*index<MAX_RECORD_ROWS){
            footpath_array[index_in_array] = data;
        }
    }
}

/*Shifts all array elements one index down "current" to create
 sorted array*/

void shift_array_elements(footpath_t* footpath_array[],
                                     int current,int end){

    for(int i=end;i>current;i--){
        footpath_array[i] = footpath_array[i-1];
    }

}

/*Grade1in functions : dict2*/

/*compares value of input file grade1in with the values 
of footpath array*/
void compare_grade1in(footpath_t* footpath_array[],int index,
                        FILE* outFile,FILE* inFile){

    char* current_grade1in = (char*)malloc(1000*sizeof(*current_grade1in));
    double current_grade1in_float = 0.0;
    int closest_value_in_arr_index = 0;

    while(fgets(current_grade1in,100,inFile)){

        current_grade1in_float = atof(current_grade1in);

        closest_value_in_arr_index =  closest_value_in_arr(
                current_grade1in_float,footpath_array,index);
        print_closet_grade1in(current_grade1in_float,
                    footpath_array[closest_value_in_arr_index],outFile);
        
    }

    free(current_grade1in);
    
}


/*searches for closest value to "current_grade1in" in "footpath_array"*/
int closest_value_in_arr(double current_grade1in, 
                                    footpath_t* footpath_array[],int index){
    double difference= 0.0;
    double min_element_difference = get_grade1in(footpath_array[index-1]);
    double min_element_index=0;
    
    
    for(int i=0;i<index;i++){
        difference = get_grade1in(footpath_array[i]) - current_grade1in;
        
        if(difference<0.0){
            difference = difference*(-1.0);
        }

        if(difference < min_element_difference){
            min_element_difference = difference;
            min_element_index = i;
        }
       /* else if(difference >min_element_difference){
            break;
        }*/
    }

    return min_element_index;

}

/*prints closest grade1in info to stdout and file*/ 
void print_closet_grade1in(double current_value,
                        footpath_t* footpath_data,FILE *outFile){

    printf("%g --> %g\n",current_value,get_grade1in(footpath_data));
    fprintf(outFile,"%g\n",current_value);
    print_footpath_into_file(footpath_data,outFile);
}

/**Personal assistance**/

/*prints grade1in array*/
/*void print_array(footpath_t* arr[], int index){
    //printf(in)
    for(int i=0;i<index;i++){
        printf("%lf\n",get_grade1in(arr[i]));
    }
}*/


/*
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
    printf(  "%lf  ",(footpath_info->start_lat));
    printf(  "%lf  ",(footpath_info->start_lon));
    printf(  "%lf  ",(footpath_info->end_lat));
    printf(  "%lf  ",(footpath_info->end_lon));
       // head_of_list = head_of_list->next;
    //}
}*/


