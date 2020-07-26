#define STAGE_NUM_ONE 1							/* stage numbers */ 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "=========================Stage %d=========================\n"	/* stage header format string */

#define MAX_CHAR_CARD_ID 8
#define MAX_NUM_CARD 100
#define MAX_CHAR_TRANS_ID 12
#define TRUE (1==1)
#define FALSE (1==0)
#define END_CARD_RECORD "%%%%%%%%%%"
#define DEFAULT_VALUE 0
#define NOT_FOUND -1


/* store time for reading in input*/
typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}temp_time_t;

/* store time for transactions */
typedef struct{
	int year;
	int month;
	int day;
}trans_time_t;

/* only store hour minute and second while reading in input
but when storing in linked_list, these element are eliminated */

/* store transactions for reading input */
typedef struct{
	char trans_id[MAX_CHAR_TRANS_ID+1];
	char card_id[MAX_CHAR_CARD_ID+1];
	temp_time_t time;
	int amount;
}temp_trans_t;

/* store transactions for processing */
typedef struct{
	char trans_id[MAX_CHAR_TRANS_ID+1];
	char card_id[MAX_CHAR_CARD_ID+1];
	trans_time_t time;
	int amount;
}trans_t;

/* since there are two type of struct of time
there are two for transactions */

typedef struct node node_t;

/* store detail of each credit card */
typedef struct{
    char id[MAX_CHAR_CARD_ID +1];
    int daily_limit;
    int transation_limit;           
    int in_dai_limit;               /* status show whether card has broken daily limit */
    int in_trans_limit;             /* status show whether card has broken transaction limit */
    int temp_balance;               /* to keep track of amount money spent in a day */
    trans_time_t temp_time;
}card_t;

/* the following two structs are for linked list implementation 
and written by Jianzhong Qi */
/* store element in linked list */
struct node {
	trans_t data;
	node_t *next;
};

/* store address for each element in linked list */
typedef struct {
	node_t *head, *foot;
} list_t;

/****************************************************************/
/* function prototypes */
/* stage one */
void stage_one(card_t *one_card);
void read_one_card(card_t *one_card);
void set_to_default(card_t *one_card);

/* stage two */
void stage_two(card_t card_lst[], int *num_card);
void read_all_card(card_t card_lst[], int *num_card);
double aver_daily_limit(card_t *card_lst, int num_card);
int find_largest(card_t *card_lst, int num_card);

/* stage three */
void stage_three(list_t *list);
int read_one_trans(temp_trans_t *one_trans, int *stop);

/* the following four functions are for linked lists implementation
and written by Jianzhong Qi in get_n_prime program    */
void insert_at_foot(list_t *list, temp_trans_t *value);     
list_t *create_empty_list();
void print_trans_id(list_t *list);
void free_list(list_t *list);

/* stage four */
void stage_four(list_t *list, card_t card_lst[], int num_card);
void process_trans(list_t *list, card_t card_lst[], int num_card);
void print_card_status(card_t *one_card);
int check_date(trans_time_t *time1, trans_time_t *time2);
void update_card(trans_t *one_trans, card_t *one_card);
int binary_search(card_t card_lst[], int lo, int hi, char* key);       
 /* binary search function are written by Jianzhong Qi with slight modification */