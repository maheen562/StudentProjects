f/* Restaurant recommender:
 *
 * Skeleton code written by Jianzhong Qi, April 2022
 * Edited by: Maheen Abdul Khaliq Khan, 1193813
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* stage heading */
#define HEADING "==========================Stage %d==========================\n"
#define STAGE_NUM_ONE 1 /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4

#define END_OF_LIST '#' /*to indicate end of restaurants in text file*/
#define MAX_LENGTH_CUISINE 20 /*max length of cuisine name*/
#define MAX_LENGTH_RESTAURANT_NAME 100 /*max length of restaurant name*/
#define MAX_LENGTH_CUSTOMER_NAME 100 /*max length of customer name*/
#define MAX_NUM_OF_RESTAURANTS 99 /*max number of resturants*/
#define ID_LENGTH 6  /*length of customer ID*/
#define COORDINATE_RANGE_TENTHS 5  /*max length of coordinates string*/
#define CHAR_LENGTH_TIME 20  /*max length of time string*/
#define MAX_PRICE_TENTHS 5  /*max length of price string*/
#define MAX_PRICE_PER_HEAD 1000  /*max average price per head*/

/* typedefs */

/*structure for restaurants*/
typedef struct{
    char id[ID_LENGTH+1];
    char x_coordinate[COORDINATE_RANGE_TENTHS+1],
            y_coordinate[COORDINATE_RANGE_TENTHS+1];
    char avg_price_per_head[MAX_PRICE_TENTHS+1];
    char cuisine[MAX_LENGTH_CUISINE + 1];
    char restaurant_name[MAX_LENGTH_RESTAURANT_NAME + 1];

}restaurant_t;

/*structure for customers*/
typedef struct customer customer_t;
struct customer{
	char id[ID_LENGTH+1];
    char name[MAX_LENGTH_CUSTOMER_NAME+1];
    int restaurant_visit[MAX_NUM_OF_RESTAURANTS];
};

#include "listops.c"
/*struct node from listops.c modified*/

/****************************************************************/
/* function prototypes */

void stage_one(restaurant_t restaurants[],int* number_of_restaurants);
void stage_two(list_t* customer_list, int* number_of_restaurants,
                   restaurant_t restaurants[]);
void stage_three(list_t* customer_list, int number_of_restaurants,
                   restaurant_t restaurants[]);
void stage_four(list_t* customer_list, int number_of_restaurants,
                   restaurant_t restaurants[]);
void print_stage_header(int stage);
int one_restaurant_read(restaurant_t *one_restaurant);
int search_linkedlist(node_t* customer_list,customer_t one_customer);
int one_customer_id_read(customer_t* customer,int number_of_restaurants);
void alter_customer_restaurant_visit(restaurant_t restaurant_list[],
                   node_t* customer_list, customer_t one_customer, 
                   int number_of_restaurants);
void find_similar_cuisine(node_t* customer_list,restaurant_t restaurants[], 
                   char cuisine[], int number_of_restaurants);
void print_list(node_t* head, int number_of_restaurants);
void distance_between_restaurants(node_t*  one_customer,
                   restaurant_t restaurants[], int index,
                   int number_of_restaurants);
void price_per_head_difference(node_t*  one_customer,restaurant_t restaurants[],
                   int index, int number_of_restaurants);
void similar_customers(node_t* one_customer,node_t* customer_list,
                   int number_of_restaurants,int customer_index);
int calculate_similarity_score(int target_visit_restaurant[],
                   int visit_restaurant[],int number_of_restaurants);
void update_restaurant_visit_list_with_for_similar_customers(
                    node_t* target_customer,node_t* customer_list_head,
                    int similar_customer,int number_of_restaurants);


/****************************************************************/

/* main program controls all the action */
int
main(int argc, char *argv[]) {
    
    restaurant_t restaurants[MAX_NUM_OF_RESTAURANTS];
    list_t* customer_list = make_empty_list();
    int number_of_restaurants = 0;
    
    stage_one(restaurants,&number_of_restaurants);
    stage_two(customer_list,&number_of_restaurants,restaurants);
    stage_three(customer_list,number_of_restaurants,restaurants);
    stage_four(customer_list,number_of_restaurants,restaurants);
    
    free_list(customer_list);
    
    return 0;
}


/*stage 1 :reads in all readaurants and outputs smallest average price*/
void stage_one(restaurant_t restaurants[], int* number_of_restaurants) {

    print_stage_header(STAGE_NUM_ONE);
    
    int i=0;
    while (one_restaurant_read(&restaurants[i])){
        i++;
        *number_of_restaurants += 1;       
    }
    
    printf("Number of restaurants: %d\n",*number_of_restaurants);
    
    restaurant_t *min_price_restaurant = &restaurants[0];
    
    for(i=1;i<(*number_of_restaurants)-1;i++){
        if(atoi(restaurants[i].avg_price_per_head) 
            < atoi(min_price_restaurant->avg_price_per_head)){
             min_price_restaurant = &restaurants[i];
        }
    }
   
    printf("Restaurant with the smallest average price: %s\n",
        min_price_restaurant->restaurant_name);
    printf("\n");
}

/*Stage 2: creates and reads in customer list*/
void stage_two(list_t *customer_list, int* number_of_restaurants,
               restaurant_t restaurants[]){
    int search_for_name = 0;
    customer_t one_customer = {"","",{1}};
        
    print_stage_header(STAGE_NUM_TWO);
    while(one_customer_id_read(&one_customer, *number_of_restaurants)){ 
        search_for_name = search_linkedlist(customer_list->head,one_customer);
        if(search_for_name == 0){
                insert_at_foot(customer_list,one_customer);
            }
         alter_customer_restaurant_visit(restaurants,customer_list->head,
                                         one_customer,*number_of_restaurants);
      }
      
      print_list(customer_list->head,*number_of_restaurants);
      printf("\n");
}

/*Stage 3: Recommend restaurant visits based on restaurant similarity*/
void stage_three(list_t* customer_list, int number_of_restaurants,
                 restaurant_t restaurants[]){
    
    print_stage_header(STAGE_NUM_THREE);
    
    int i=0;
    node_t* iter_list = customer_list->head;
    while (iter_list != NULL){
        for (i=0;i<number_of_restaurants;i++){
            if(iter_list->data.restaurant_visit[i] != 0 
                   && iter_list->data.restaurant_visit[i] != -1){
                find_similar_cuisine(iter_list,restaurants,
                                 restaurants[i].cuisine,number_of_restaurants);
                distance_between_restaurants(iter_list,restaurants,i,
                                             number_of_restaurants);
                price_per_head_difference(iter_list,restaurants,i,
                                             number_of_restaurants);
            }
        }        
        iter_list = iter_list->next;
    }
    
    print_list(customer_list->head,number_of_restaurants);
    printf("\n");
}

/*Stage 4: Recommended based on user similarity*/
void stage_four(list_t* customer_list, int number_of_restaurants,
                restaurant_t restaurants[]){

    print_stage_header(STAGE_NUM_FOUR);
    node_t* iter_list = customer_list-> head;
    int customer_index_in_list=0;
    
     while (iter_list != NULL){
         similar_customers(iter_list,customer_list->head,number_of_restaurants,
                           customer_index_in_list);
         iter_list = iter_list -> next;
         customer_index_in_list++;
     }
    
    print_list(customer_list->head,number_of_restaurants);
    

}


/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(HEADING, stage_num);
}

/*For stage 1: reads in all the restaurants*/
int one_restaurant_read(restaurant_t *one_restaurant){
    
    scanf("%s",one_restaurant->id);
    getchar();
    if(one_restaurant->id[0] != '#'){
        scanf("%s",one_restaurant->x_coordinate);
        getchar();
        scanf("%s",one_restaurant->y_coordinate);
        getchar();
        scanf("%s",one_restaurant->avg_price_per_head);
        getchar();
        scanf("%s",one_restaurant->cuisine);
        getchar();
        scanf("%s",one_restaurant->restaurant_name);
        return 1;
    }
    
    return 0;
    
}

/*For stage 2: reads in customers*/
int one_customer_id_read(customer_t* customer,int number_of_restaurants){
    char time[CHAR_LENGTH_TIME+1]; 
    char price[MAX_PRICE_TENTHS + 1];
    scanf("%s",time);
    if (strlen(time) != 19){
        return 0;
    }
    getchar();
    scanf("%s",customer->name);
    getchar();
    scanf("%s",customer->id);
    getchar();    
    scanf("%s",price);
    getchar();
    
    for(int i=0;i<number_of_restaurants;i++){
        customer->restaurant_visit[i] = 0;
    }
 
    return 1;
}

/*For Stage 2: Search if customer s already present in linked list*/
int search_linkedlist(node_t* customer_list, customer_t one_customer){
    int found = 0;
    while(customer_list != NULL){
        if(strcmp(customer_list->data.name,one_customer.name) == 0){
            found = 1; 
            break;
        }
        customer_list = customer_list->next;
   }

   return found;
}

/*For Stage 2: Alters restaurant visit array if restaurant visited*/
void alter_customer_restaurant_visit(restaurant_t restaurant_list[], 
         node_t*customer_list, customer_t one_customer, 
         int number_of_restaurants){
       int i=0;
       int found = 1;
       
        for (i=0;i<number_of_restaurants;i++){
           if(atoi(one_customer.id) == atoi(restaurant_list[i].id)){         
               break;
           }
        }
        
        while(customer_list != NULL){
            found = strcmp(customer_list->data.name,one_customer.name);
            if(found == 0){
                customer_list->data.restaurant_visit[i] +=1;   
                break;
            }
            customer_list = customer_list->next;
        }
}

/*prints linked list for each stage*/
void print_list(node_t* head, int number_of_restaurants){

    node_t* one_customer = head;
    //printf("\n");
      while(one_customer != NULL){
          printf("%s:",one_customer->data.name);
          for(int i=0;i<(number_of_restaurants);i++){
              if(one_customer->data.restaurant_visit[i] == -1){
                   printf("  -");
              }
              else if(one_customer->data.restaurant_visit[i] == -2){
                  printf("  +");
              }
              else if(one_customer->data.restaurant_visit[i] == -3){
                  printf("  *");
              }
              else{
                  printf("  %d",one_customer->data.restaurant_visit[i]);
              }
          }
          one_customer = one_customer->next;
          printf("\n");
      }

}


/*For Stage 3: alters restaurant visit array for similar cuisine*/
void find_similar_cuisine(node_t* one_customer,restaurant_t restaurants[],
                          char cuisine[],int number_of_restaurants){

    int i=0;

    for(i=0;i<number_of_restaurants;i++){
        if(strcmp(restaurants[i].cuisine,cuisine) == 0){   
            if(one_customer->data.restaurant_visit[i] == 0){
                one_customer->data.restaurant_visit[i] = -1;
            }
        }
    }
}

/*For Stage 3: alters resetaurant visit array for similar prices*/
void price_per_head_difference(node_t*  one_customer,restaurant_t restaurants[],
                               int index, int number_of_restaurants){

    int price_difference = 0;
    int avg_price;
    int i=0;
    
    avg_price = atoi(restaurants[index].avg_price_per_head);
    
    for (i=0;i<number_of_restaurants;i++){
        price_difference = avg_price - atoi(restaurants[i].avg_price_per_head);
        if(pow(price_difference,2) <=400) {
            if(one_customer->data.restaurant_visit[i] == 0){
                one_customer->data.restaurant_visit[i] = -1;
            }        
        }
    }
}
    

/*For stage 3:alters restaurant visits for restaurants within coordinate range*/
void distance_between_restaurants(node_t* one_customer,
         restaurant_t restaurants[], int index, int number_of_restaurants){
    
    float distance = 0.0,distance_x_square = 0.0,distance_y_square = 0.0;
    int target_restaurant_index = 0;
    int i=0;

    target_restaurant_index = index;
    for (i=0;i<number_of_restaurants;i++){
        if(i != target_restaurant_index){
            distance_x_square = pow((atof(restaurants[i].x_coordinate)-
                atof(restaurants[target_restaurant_index].x_coordinate)),2);
            distance_y_square = pow((atof(restaurants[i].y_coordinate)-
                atof(restaurants[target_restaurant_index].y_coordinate)),2);
            distance = sqrt(distance_x_square+distance_y_square);
            
            if(distance<=30.0){
                if(one_customer->data.restaurant_visit[i] == 0){
                    one_customer->data.restaurant_visit[i] = -1;
                }
            }
        } 
    }
  
}    

/*For stage 4: Alters the restaurant visits for top two similar customers*/
void similar_customers(node_t* one_customer,node_t* customer_list_head,
                       int number_of_restaurants,int customer_index){
    
    node_t* iter_list;
    int first_customer_index=0,second_customer_index=0;
    int customer_in_list_index=0;
    iter_list = customer_list_head;
    int score_temp=0;
    int max_score = -1;
    int second_max_score = -1;
    while (iter_list != NULL){

        score_temp = calculate_similarity_score(
             one_customer->data.restaurant_visit,
             iter_list->data.restaurant_visit,number_of_restaurants);   
        if((score_temp > max_score) && 
           (customer_index != customer_in_list_index)){      
            second_customer_index = first_customer_index;
            second_max_score =  max_score;
            first_customer_index = customer_in_list_index;
            max_score = score_temp;
         }
        else if((score_temp > second_max_score) && 
                (customer_index != customer_in_list_index)){
            second_customer_index = customer_in_list_index;
            second_max_score = score_temp;
        }

        customer_in_list_index++;
        iter_list = iter_list->next;
    }

    if(max_score > 0){
    update_restaurant_visit_list_with_for_similar_customers(
        one_customer,customer_list_head,first_customer_index,
        number_of_restaurants);
    }
    if(second_max_score > 0){
    update_restaurant_visit_list_with_for_similar_customers(one_customer,
        customer_list_head,second_customer_index,number_of_restaurants);
    }
}

/*For stage 4:calculates similarity score for a customer with another customer*/
int calculate_similarity_score(int target_visit_restaurant[],
        int visit_restaurant[],int number_of_restaurants){
    
    int similarity_score = 0;
    int score=0;
    
    for(int i=0;i<number_of_restaurants;i++){
        if(target_visit_restaurant[i] > 0 && visit_restaurant[i] > 0 ){
        score = (target_visit_restaurant[i] * visit_restaurant[i]);

        similarity_score += score; 
        }
    }
       
    return similarity_score;
}


/*For stage 4:updates restaurant visit for highest similarity scores customers*/
void update_restaurant_visit_list_with_for_similar_customers(
    node_t* target_customer,node_t* customer_list_head,int similar_customer,
    int number_of_restaurants){
    
    node_t* iter_list = customer_list_head;
    int i=0; 
        
    for(i=0;i<similar_customer;i++){
        iter_list = iter_list->next; 
    }
    for(i=0;i<number_of_restaurants;i++){
        
        if(iter_list->data.restaurant_visit[i] > 0){
            if(target_customer->data.restaurant_visit[i] <= 0){
                target_customer->data.restaurant_visit[i] += -1;
            }
        }
    }   
}
/*time complexitiy for stage 4: O(n^3) The It goes through all the customers c. 
To calculate the similarity scores it compares each customer to all other customers
giving us c*c. The check the similarity it has to go through r restaurants. so
c*c*r The similarity scores are checked are changed as altered as we go through 
the customers.*/   
/*Algorithms are fun!!*/