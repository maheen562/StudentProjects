/* DNA sequence mapper:
 *
 * Skeleton code written by Jianzhong Qi, April 2022
 * Edited by: Maheen Abdul Khaliq Khan 1193813
 *
 */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <math.h>

#define STAGE_NUM_ONE 1 /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

#define MAX_READ_ID_LENGTH 100 /* maximum read ID length */
#define MAX_READ_LENGTH 100 /* maximum read length */
#define MAX_NUM_READS 100 /* maximum number of reads */
#define MAX_REF_LENGTH 1000 /* maximum reference DNA length */
#define LOWER_BOUND_ASCII 33 /*lower bound for ASCII*/
#define ERROR_PROBABILITY_FOR_BASE_REPLACEMENT 0.15 /*minimum error 
probability for base replacement*/
#define MAX_VALUE_OF_SCORE 73 /*Is the largest possible index of score*/
#define MAX_AVERAGE_OF_SCORE 100 /*maximum possible average of score*/
#define MAX_INDEX 1000 /*largest possible index of read*/

#define BASE_A 'A' /*base A character*/
#define BASE_C 'C' /*base C character*/
#define BASE_G 'G' /*base G character*/
#define BASE_T 'T' /*base T character*/
#define CHAR_FOR_BASE_REPLACEMENT '*' /*chaaracter used for base replacement*/
#define NULL_BYTE '\0' /*Null byte to indicate end of string*/
#define START_OF_ID '@' /*character used to start ID*/

typedef char read_id_t[MAX_READ_ID_LENGTH + 1]; /* a read ID */
typedef char read_t[MAX_READ_LENGTH + 1]; /* a read */
typedef char score_t[MAX_READ_LENGTH + 1]; /* quality scores of a read */
typedef char ref_t[MAX_REF_LENGTH + 1]; /* a reference DNA sequence */

/****************************************************************/

/* function prototypes */
void take_one_read(read_t one_read, score_t scores_of_one_read, read_id_t id);
void print_stage_header(int stage_num);
void stage_one(read_t one_read, score_t scores_of_one_read);
void stage_two(read_t reads[], score_t scores[], int * num_reads);
void stage_three(read_t reads[], score_t scores[], int num_reads);
void stage_four(ref_t ref_sequence);
void stage_five(read_t reads[], score_t scores[], int num_reads,
  ref_t ref_sequence);
int index_of_base_with_smallest_quality_score(read_t one_read,
    score_t scores_of_one_read, char * min_value_read);
float average_quality_score(score_t scores[], read_t reads[], int num_reads, 
    read_t score_avg_min);
void error_probability_base_replacement(read_t one_read,
    score_t scores_of_one_read);
float match_score(read_t one_read, score_t score_of_one_read,
    int length_of_read, ref_t sub_string, int substring_position);
float error_probability(char scores_of_one_read);
int length_of_read(read_t one_read);
void read_reference_sequence(ref_t ref_sequence);

/* add your own function prototypes here */
/****************************************************************/

/* main function controls all the action, do NOT modify this function */
int
main(int argc, char * argv[]) {
    /* to hold all input reads and quality scores */
    read_t reads[MAX_NUM_READS];
    score_t scores[MAX_NUM_READS];
    /* to hold the number of input reads */
    int num_reads = 0;
    /* to hold the input reference sequence */
    ref_t ref_sequence;

    /* stage 1: process one read */
    stage_one(reads[0], scores[0]);
    num_reads++;

    /* stage 2: process all reads */
    stage_two(reads, scores, & num_reads);

    /* stage 3: mask bases with high error probability */
    stage_three(reads, scores, num_reads);

    /* stage 4: process reference sequence */
    stage_four(ref_sequence);

    /* stage 5: map reads to the reference sequence */
    stage_five(reads, scores, num_reads, ref_sequence);

    /* all done; take some rest */
    return 0;
}

/* print stage header given stage number */
void
print_stage_header(int stage_num) {
    printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* add your code below */

/* stage 1: process one read */
void
stage_one(read_t one_read, score_t scores_of_one_read) {

    print_stage_header(STAGE_NUM_ONE);

    int smallest_index;
    read_id_t id;
    read_t min_value_read;
    
    take_one_read(one_read, scores_of_one_read, id);    
    smallest_index = index_of_base_with_smallest_quality_score(one_read,
        scores_of_one_read, min_value_read);
    
    printf("Base with the smallest quality score: %s\n", min_value_read);
    printf("Index: %d\n\n", smallest_index);

}

/* stage 2: process all reads */
void
stage_two(read_t reads[], score_t scores[], int * num_reads) {

    print_stage_header(STAGE_NUM_TWO);

    read_id_t id;
    float average_smallest_quality_scores = 0.0;
    read_t scores_avg_min_read;

    id[0] = START_OF_ID;
    while (id[0] == START_OF_ID) {

        take_one_read(reads[ * num_reads], scores[ * num_reads], id);
        * num_reads = * num_reads + 1;
    }

    * num_reads = * num_reads - 1;

    average_smallest_quality_scores = average_quality_score(scores, reads, 
      (*num_reads), scores_avg_min_read);

    printf("Total number of reads: %d\n", * num_reads);
    printf("Smallest average quality score: %.2f\n",
      average_smallest_quality_scores);
    printf("Read with the smallest average quality score:\n%s\n\n", 
      scores_avg_min_read);

}

/* stage 3: mask bases with high error probability */
void
stage_three(read_t reads[], score_t scores[], int num_reads) {

    print_stage_header(STAGE_NUM_THREE);

    for (int i = 0; i < num_reads; i++) {
        error_probability_base_replacement(reads[i], scores[i]);
        printf("%s\n", reads[i]);

    }
    printf("\n");

}

/* stage 4: process reference sequence */
void stage_four(ref_t ref_sequence) {

    print_stage_header(STAGE_NUM_FOUR);

    int total_number_of_bases = 0, number_of_bases_A = 0, number_of_bases_C = 0,
    number_of_bases_G = 0, number_of_bases_T = 0;
    int index = 0;

    read_reference_sequence(ref_sequence);

    while (ref_sequence[index] != NULL_BYTE) {
        if (ref_sequence[index] == BASE_A) {
            number_of_bases_A++;
        } else if (ref_sequence[index] == BASE_C) {
            number_of_bases_C++;
        } else if (ref_sequence[index] == BASE_G) {
            number_of_bases_G++;
        } else if (ref_sequence[index] == BASE_T) {
            number_of_bases_T++;
        }
        total_number_of_bases++;
        index++;
    }

  printf("Length of the reference sequence: %d\n", total_number_of_bases);
  printf("Number of A bases: %d\n", number_of_bases_A);
  printf("Number of C bases: %d\n", number_of_bases_C);
  printf("Number of G bases: %d\n", number_of_bases_G);
  printf("Number of T bases: %d\n\n", number_of_bases_T);

}

/* stage 5: map reads to the reference sequence */
void
stage_five(read_t reads[], score_t scores[], int num_reads,
  ref_t ref_sequence) {

  print_stage_header(STAGE_NUM_FIVE);

  float nearest_match = 0.0;
  float match_score_of_position;
  int nearest_match_position;
  int length_of_ref_seq = 0, length_of_one_read = 0, substring_position = 0;
  read_t matches[num_reads];

  length_of_ref_seq = length_of_read(ref_sequence);

  for (int i = 0; i < num_reads; i++) {

    length_of_one_read = length_of_read(reads[i]);

    /*checking which substring is nearest match in sequence for each read, and 
    printing it out*/  
    while (((length_of_ref_seq - substring_position) >= length_of_one_read)) {

      match_score_of_position = match_score(reads[i], scores[i],
          length_of_one_read, ref_sequence,substring_position);

      if (match_score_of_position > nearest_match) {

        nearest_match = match_score_of_position;
        nearest_match_position = substring_position;

      }
      substring_position++;

    }

    strncpy(matches[i], & ref_sequence[nearest_match_position],
      length_of_one_read);
    matches[i][length_of_one_read] = NULL_BYTE ;
    substring_position = 0;
    nearest_match = 0.0;
  }

  for (int i = 0; i < num_reads; i++) {

    printf("Read:  %s\n", reads[i]);
    printf("Match: %s\n", matches[i]);
  }

}


/* process a read record */
void
take_one_read(read_t one_read, score_t scores_of_one_read, read_id_t id) {

  scanf("%s", id);
  if (id[0] == START_OF_ID) {
    scanf("%s", one_read);
    getchar();
    getchar(); // skip '+' and '\n'
    scanf("%s", scores_of_one_read);
  }
}

/*reads the reference sequence from the file*/
void 
read_reference_sequence(ref_t ref_sequence) {

  scanf("%s", ref_sequence);
}

/*Calculates the length of a read, score, or reference sequence*/
int 
length_of_read(read_t one_read) {

  int index = 0;
  while (one_read[index] != NULL_BYTE ) {

    index++;
  }

  return index;
}

/* returns the index of the base with the smallest quality score */
int 
index_of_base_with_smallest_quality_score(read_t one_read,
  score_t scores_of_one_read,
  read_t min_value_read) {
  int i, min_index = MAX_INDEX, min_value_score = MAX_VALUE_OF_SCORE;

  for (i = 0; scores_of_one_read[i] != NULL_BYTE ; i++) {
    if (scores_of_one_read[i] < min_value_score) {
      * min_value_read = one_read[i];
      min_value_score = scores_of_one_read[i];
      min_index = i;
    }
  }

  return min_index;
}

/*Calculates average quality score of all scores in file*/
float 
average_quality_score(score_t scores[], read_t reads[], int num_reads,
    read_t scores_avg_min_read) {
    float scores_avg = 0.0, scores_avg_min = MAX_AVERAGE_OF_SCORE;
    int scores_index = 0, scores_sum = 0;
    int length_of_each_read = 0;
    
    
    for (int i = 0; i < num_reads; i++) {
        
        length_of_each_read = length_of_read(reads[i]);
        
        while (scores[i][scores_index] != NULL_BYTE ) { 
            scores_sum += scores[i][scores_index];
            scores_index++;
        }
        scores_avg = (scores_sum * 1.0) / (length_of_each_read*1.0);
        if (scores_avg_min > scores_avg) {
            scores_avg_min = scores_avg;
            strcpy(scores_avg_min_read, reads[i]);
        }
        scores_index = 0;
        scores_sum = 0;

     }
        

    return scores_avg_min;
}

/*Calculates the error probability of a score of one read*/
float 
error_probability(char score_of_one_read) {

    float x = (score_of_one_read - LOWER_BOUND_ASCII) * 1.0 / 10.0;
    float p = 1 / (pow(10, x));

    return p;
}

/*replaced bases that have an error probability of less then 0.15*/
void 
error_probability_base_replacement(read_t one_read,
    score_t scores_of_one_read) {

    int score_read_index = 0;
    float p = 0.0;

    while (scores_of_one_read[score_read_index] != NULL_BYTE ) {
       p = error_probability(scores_of_one_read[score_read_index]);

      if (p > ERROR_PROBABILITY_FOR_BASE_REPLACEMENT) {
          one_read[score_read_index] = CHAR_FOR_BASE_REPLACEMENT;
    }

    score_read_index++;
    }

}

/*calculates the match score of each read*/
float 
match_score(read_t one_read, score_t score_of_one_read,
    int length_of_read, ref_t sub_string,
    int substring_position) {

    float score = 0.0;

    for (int i = 0; i < length_of_read; i++) {
        if (one_read[i] == sub_string[substring_position + i]) {
            score += log2(error_probability(score_of_one_read[i]));

        } else if (one_read[i] == CHAR_FOR_BASE_REPLACEMENT) {
              score += log2(0.25);
        } else {
          score += log2(1);
        }
    }

    return -1 * score;

}

/*All done!! Algorithms are fun!*/