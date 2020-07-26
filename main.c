/* Fraudulent transations detector: 
 * 
 * Student Name: DANG CHI TRUNG
 * Student ID: 1069862
 * algorithms are fun
 * 
 * 
 *
 * Original problem by Jianzhong Qi, May 2020
 * There are partial code segment in this program are written by Jianzhong Qi 
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ops.h"

int
main(int argc, char *argv[]) {
    /* to hold all input credit cards */
    card_t card_lst[MAX_NUM_CARD];
    /* to hold the number of input cards */
    int num_card = 0;
    /* to held all transactions */
    list_t *list = create_empty_list();

    /* stage 1: read the first credit card record */
    stage_one(&card_lst[num_card]); 
	num_card++;

    /* stage 2: read all credit cards record */
    stage_two(card_lst, &num_card);

    /* stage 3: reading transactions */
    stage_three(list);

    /* stage 4: check for fraudulent transactions */
    stage_four( list, card_lst, num_card);

    /* free memory after using */
	free_list(list);
	list = NULL;
    return 0;
}

/* stage 1: read the first credit card record */
void stage_one(card_t *one_card){
    /* code for stage 1 */
    read_one_card(one_card);
    set_to_default(one_card);

    /*print stage header */
    print_stage_header(STAGE_NUM_ONE);

    /* print dersired output */
    printf("Card ID: %s\n", one_card->id);
    printf("Daily limit: %d\n", one_card->daily_limit);
    printf("Transaction limit: %d\n", one_card->transation_limit);

    printf("\n");
}

/* stage 2: read all credit card records */
void stage_two(card_t card_lst[], int *num_card){

    /* code for stage 2 */
    double average;
    int pos;

    read_all_card(card_lst, num_card);
    average = aver_daily_limit(card_lst, *num_card);
    pos = find_largest(card_lst, *num_card);

    /* set balanked value in each card to 0 */
    for(int i = 1; i<*num_card; i++){
        set_to_default(&card_lst[i]);
    }

    /* print stage header */
    print_stage_header(STAGE_NUM_TWO);

    /* print desired output */
    printf("Number of credit cards: %d\n", *num_card);
    printf("Average daily limit: %.2f\n", average);
    printf("Card with the largest transaction limit: %s\n", card_lst[pos].id);

    printf("\n");
}

/* stage 3: reading transactions */
void stage_three(list_t *list){
    /* code for stage 3 */
    int stop = FALSE;
    while (!stop) {
		temp_trans_t one_trans;
        /* read the rest of transactions until reach EOF */
		read_one_trans(&one_trans, &stop);
		if (stop){
			break;
		}
		insert_at_foot(list, &one_trans);
	}
	
    /* print stage header */
    print_stage_header(STAGE_NUM_THREE);

    /* print desired output */
    print_trans_id(list);

    printf("\n");
}

/* check for fraudulent transactions */
void stage_four(list_t *list, card_t card_lst[], int num_card){
    /* print stage header */
    print_stage_header(STAGE_NUM_FOUR);

    /* code for stage 4 */
    process_trans(list, card_lst, num_card);
    
}

/* Explanation for O(nlogm) average time complexity given n transactions and m credit card records.
 * The code segment contains this algorithm is in process_trans function from line 444
 * 
 * - Firstly, I have looped through the transactions stored in linked list one by one for one time only 
 * in this stage
 * This will cost a O(n) time complexity for input size of n transactions
 * 
 * - Secondly, for each transaction, I have conducted a binary search though the card list, 
 * an update_card function, an check_date function and a print_card status function.
 * 
 *  + Average time complexity of binary search algorithms is O(logm) given m is number of credit cards
 * 
 *  + All of these functions above except for binary seach consist mostly of comparison operation
 * These three function will cost a constant time complexity which is neligible compared
 * to O(logm) of binary search algorithms and therefore will be emitted
 * 
 * =>> Overall, time complexity of algorithms for this stage is O(nlogm)
*/