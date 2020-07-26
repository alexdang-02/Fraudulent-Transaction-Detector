#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ops.h"


/* print stage header given stage number */
void print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/* set all blanked element in each card to DEFAULT_VALUE */
void set_to_default(card_t *one_card){

    one_card->in_dai_limit = TRUE;
    one_card->in_trans_limit = TRUE;
    one_card->temp_balance = DEFAULT_VALUE;
    one_card->temp_time.day = DEFAULT_VALUE;
    one_card->temp_time.month = DEFAULT_VALUE;
    one_card->temp_time.year = DEFAULT_VALUE;

}

/* read the first line from input */
void read_one_card(card_t *one_card){

    /* read id part of the card */
    if (scanf("%s", one_card->id) != 1){
        printf("Invalid Input");
        exit(EXIT_FAILURE);
    }

    /* read daily limit of the card */
    if (scanf("%d", &one_card->daily_limit) != 1){
        printf("Invalid Input");
        exit(EXIT_FAILURE);
    }

    /* read transaction limit of the card */
    if (scanf("%d", &one_card->transation_limit) != 1){
        printf("Invalid Input");
        exit(EXIT_FAILURE);
    }
}

/* read the rest of card records */
void read_all_card(card_t card_lst[], int *num_card){
    char temp[MAX_CHAR_CARD_ID+1];

    while (TRUE){

        /* read id of the card */
        if (scanf("%s", temp) == 1){
            if (!strcmp(temp, END_CARD_RECORD)) {
                break;
            }
            for (int i = 0; i < MAX_CHAR_CARD_ID+1; i++){
                card_lst[*num_card].id[i] = temp[i]; 
            }
        }else {
            printf("Invalid Input");
            exit(EXIT_FAILURE);
        }
        
        /* read daily limit of the card */
        if (scanf("%d", &card_lst[*num_card].daily_limit) != 1){
            printf("Invalid Input");
            exit(EXIT_FAILURE);
        }

        /* read transaction limit of the card */
        if (scanf("%d", &card_lst[*num_card].transation_limit) != 1){
            printf("Invalid Input");
            exit(EXIT_FAILURE);
        }
        (*num_card)++;
    }
}

/* find average of daily limit */
double aver_daily_limit(card_t *card_lst, int num_card){
    double sum = 0;

    for (int i =0; i < num_card; i++){
        sum += card_lst->daily_limit;
        card_lst++;
    }

    return sum/num_card;
}

/* find position of card with largest transaction limit */
int find_largest(card_t *card_lst, int num_card){
    int pos;
    int cur_max = 0;
    int c;

    for (int i = 0; i < num_card; i++){
        if ( ((c = card_lst[i].transation_limit) > cur_max) || (c == cur_max && i < pos)){
            cur_max = c;
            pos = i;
        }
    }

    return pos;
}

/* insert value into linked-list */
void insert_at_foot(list_t *list, temp_trans_t *one_trans) {
	node_t *new;
	new = (node_t *)malloc(1*sizeof(node_t));

    /* copy data into each element in list */
    /* for time in one_trans, only save year, month and date */
	strcpy(new->data.trans_id, one_trans->trans_id);
	strcpy(new->data.card_id, one_trans->card_id);
	new->data.time.year = one_trans->time.year;
	new->data.time.month = one_trans->time.month;
	new->data.time.day = one_trans->time.day;
    new->data.amount = one_trans->amount;

	new->next = NULL;

	if(new == NULL) {
		printf("Memory allocation failed, exiting\n");
		exit(EXIT_FAILURE);
	}
	if (list->head == NULL) {
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}

}

/* read each line of transactions */
int read_one_trans(temp_trans_t *one_trans, int *stop){
    int c;

    /* read transaction id of the transaction */
    /* with assumption that the input is valid, the only case when following if 
    is TRUE is when c != 1 and that's when scanf reaches EOF */
	if ((c = scanf("%s", one_trans->trans_id)) != 1){
            *stop = TRUE;
            return 0;
    }

    /* read card id of the transaction */
    if (scanf("%s", one_trans->card_id) != 1){
        printf("Invalid Input");
        exit(EXIT_FAILURE);
    }

    /* read time of the transaction */
    if (scanf("%4d:%2d:%2d:%2d:%2d:%2d", &one_trans->time.year, &one_trans->time.month, 
	&one_trans->time.day, &one_trans->time.hour, &one_trans->time.minute, &one_trans->time.second ) != 6){
        printf("Invalid Input");
        exit(EXIT_FAILURE);
    }

    /* read amount of transaction */
    if (scanf("%d", &one_trans->amount) != 1){
        printf("Invalid Input");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/* free memory after using */
void free_list(list_t *list) {
	node_t *new;
	
	while(list->head != NULL) {
		new = list->head;
		list->head = list->head->next;
		free(new);
		new = NULL;	
	}
	
	free(list);
}

/* print out transactions */
void print_trans_id(list_t *list) {
	node_t *new;
	
	new = list->head;
	while(new != NULL) {
		printf("%s\n", new->data.trans_id);
		new = new->next;
	}
}

/* create memory for linked list */
list_t *create_empty_list() {

	list_t *list = (list_t *)malloc(1*sizeof(list_t));

	if(list == NULL) {
		printf("Memory allocation failed, exiting\n");
		exit(EXIT_FAILURE);
	}

	list->head = list->foot = NULL;
	
	return list;
}

/* process each transactions one by one and update information in each card 
and print out status according to transactions */
void process_trans(list_t *list, card_t card_lst[], int num_card){
    node_t *new;
	new = list->head;

	while(new != NULL) {

        /* find corresponding card_id of transactions in card_lst using binary search */
        int loc;
        loc = binary_search(card_lst, 0, num_card, new->data.card_id);
        if (loc == NOT_FOUND){
            printf("Can not found\n");
        }

        update_card(&(new->data), &(card_lst[loc]));

        /* print out desired output */
        printf("%s       ",new->data.trans_id);
        print_card_status(&card_lst[loc]);

		new = new->next;
	}

}

/* search for location of card in card_lst using binary search algorithms */
int binary_search(card_t card_lst[], int lo, int hi, char* key){
    /* if not found return -1 */
    if (lo>=hi){
        return NOT_FOUND;
    }
    int mid = (lo+hi)/2;
    if (strcmp(key, card_lst[mid].id)<0){
        return binary_search(card_lst, lo, mid, key);
    } else if (strcmp(key, card_lst[mid].id)>0){
        return binary_search(card_lst, mid+1, hi, key);
    } else {
        return mid;
    }
}

/* print status of card based on updated information */
void print_card_status(card_t *one_card){

    int dai_lim = one_card->in_dai_limit;
    int trans_lim = one_card->in_trans_limit;

    if (dai_lim == TRUE && trans_lim == TRUE){
        printf("IN_BOTH_LIMITS\n");
    }else if (dai_lim == FALSE && trans_lim == FALSE) {
        printf("OVER_BOTH_LIMITS\n");
    }else if (dai_lim == FALSE) {
        printf("OVER_DAILY_LIMIT\n");
    }else {
        printf("OVER_TRANS_LIMIT\n");
    }

}

/* updated status in each card */
void update_card(trans_t *one_trans, card_t *one_card){

    /* divide into two cases: same day and new day */
    if (check_date(&one_trans->time, &one_card->temp_time)){

        /* update transaction limit status */
        if(one_trans->amount > one_card->transation_limit){
            one_card->in_trans_limit = FALSE;
        }

        /* add transaction amount in case of same day and update daily limit status */
        one_card->temp_balance += one_trans->amount;
        if(one_card->temp_balance > one_card->daily_limit){
            one_card->in_dai_limit = FALSE;
        }

    }else{

        /* reset both status to default in case of new day before updated them*/
        one_card->in_trans_limit = TRUE;
        if(one_trans->amount > one_card->transation_limit){
            one_card->in_trans_limit = FALSE;
        }

        /* reset temp_balance to 0 and add new amount to it  */
        one_card->in_dai_limit = TRUE;
        one_card->temp_balance = one_trans->amount;
        if(one_card->temp_balance > one_card->daily_limit){
            one_card->in_dai_limit = FALSE;
        }
        
        /* update date in card in case of new day */
        one_card->temp_time.day = one_trans->time.day;
        one_card->temp_time.month = one_trans->time.month;
        one_card->temp_time.year = one_trans->time.year;
    }

}

/* check whether two input date are the same */
int check_date(trans_time_t *time1, trans_time_t *time2){

    if ((time1->year == time2->year) && (time1->month == time2->month) && 
    (time1->day == time2->day)){
        return TRUE;
    }

    return FALSE;
}