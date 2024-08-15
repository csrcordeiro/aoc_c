#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _card {
    int card_id;
    int winning_numbers[100];
    int given_numbers[100];
    int matches;
    struct _card *next;
} Card;

static Card *first;

static void print_cards() {
    Card *current = first;
    int *ptr = NULL;
    while(current != NULL) {
	printf("Card [%d]:\n", current->card_id);
	printf("Winning number:\n");
	for(int i = 0; i < 100; i++) {
	    printf("%d ", current->winning_numbers[i]);
	}
	printf("\n");
	printf("Given number:\n");
	for(int i = 0; i < 100; i++) {
	    printf("%d ", current->given_numbers[i]);
	}
	printf("\n");

	current = current->next;
    }
    printf("\n");
}

static void insert_card(int card_id, int* winning_numbers, int* given_numbers)
{
    Card *card = malloc(sizeof(Card));
    card->card_id = card_id;
    card->matches = 0;

    memcpy(card->winning_numbers, winning_numbers, sizeof(card->winning_numbers));
    memcpy(card->given_numbers, given_numbers, sizeof(card->given_numbers));

    if (first == NULL) {
	first = card;
    } else {
	Card *current = first;
	while(current->next != NULL) {
	    current = current->next;
	}

	current->next = card;
    }

}

static int convert_integer(char* string)
{
    int number = 0;
    char *ptr = string;
    while(*ptr != '\0' && *ptr != '\n') {
	if (*ptr == ' ') {
	    ptr++;
	    continue;
	}
	number = number * 10 + (*ptr - 48);
	ptr++;
    }
    return number;
}

static void initialize_numbers(int* number)
{
    for(int i = 0; i < 100; i++) {
	number[i] = -1;
    }
}

static void set_numbers(int* numbers, char* numbers_as_chars)
{
    initialize_numbers(numbers);

    char *saved;
    int i =0;
    for (char *numbs = strtok_r(numbers_as_chars, " ", &saved); numbs != NULL; numbs = strtok_r(NULL, " ", &saved)) {
	numbers[i] = convert_integer(numbs);
	i++;
    }

}

static void extract_card_information(char* line)
{
    char *str = line;
    int winning_numbers[100], given_numbers[100];
    int card_id = 0;
    int start_pos;

    sscanf(str, "Card%*[ ]%d:%n", &card_id, &start_pos);
    str += start_pos;

    char *winning_set = strtok(str, "|");
    char *given_set   = strtok(NULL, "|");

    set_numbers(winning_numbers, winning_set);
    set_numbers(given_numbers, given_set);


    insert_card(card_id, winning_numbers, given_numbers);

}

static void count_matches()
{
    Card *current = first;
    int *winning_numbers = NULL;
    int *given_numbers = NULL;
    int i, j = 0;

    while(current != NULL) {
	winning_numbers = current->winning_numbers;
	given_numbers = current->given_numbers;
	i = 0;
	j = 0;

	while(winning_numbers[i] > -1) {
	    j = 0;
	    while(given_numbers[j] > -1) {
		if (winning_numbers[i] == given_numbers[j]) {
		    current->matches += 1;
		}
		j++;
	    }
	    i++;
	}

	current = current->next;
    }
}

static int calculate_total_score()
{
    int score = 0;
    Card *current = first;
    while(current != NULL) {
	int score_card = 0;
	for(int i = 0; i < current->matches; i++) {
	    if (i == 0) {
		score_card = 1;
	    } else {
		score_card *= 2;
	    }
	}

	score += score_card;

	current = current->next;
    }

    return score;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, stdin) != -1) {
	extract_card_information(line);
    }

    count_matches();

    int score = calculate_total_score();

    printf("%d\n", score);

    return 0;
}
