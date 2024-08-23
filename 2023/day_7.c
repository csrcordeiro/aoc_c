#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#define MAX_HANDS 1000

int JOKER_RULE = 1;

enum Type {
	HIGH,
	ONE,
	TWO,
	THREE,
	FULL,
	FOUR,
	FIVE
};

typedef struct _hand {
	char *cards;
	int bid;
	enum Type type;
} Hand;

static char* type_to_s(enum Type t)
{
	switch (t) {
		case FIVE:
			return "FIVE CARDS";
		case FOUR:
			return "FOUR CARDS";
		case FULL:
			return "FULL HOUSE";
		case THREE:
			return "THREE CARDS";
		case TWO:
			return "TWO PAIR";
		case ONE:
			return "ONE PAIR";
	}

	return "HIGHER CARD";
}


static Hand* new_hand(char *line)
{
	Hand *hand = malloc(sizeof(Hand));

	char *cards = strtok(line, " ");
	hand->cards = malloc(strlen(cards)+1);

	strcpy(hand->cards, cards);

	hand->bid = convert_integer(strtok(NULL, " "));

	return hand;
}

/*
 * Using ASCII numbers to count frequency.
 * Got from https://www.geeksforgeeks.org/return-maximum-occurring-character-in-the-input-string/
 */
static char find_most_frequent_card(char *str)
{
    int count[256] = { 0 };

    for (int i = 0; i < 5; i++)
        count[(int)str[i]]++;

    char max_char = 'J';
    int max_count = 0;
    for (int i = 0; i < 5; i++) {
		if (str[i] == 'J')
			continue;

		char current_char = count[(int)str[i]];
        if (current_char > max_count) {
            max_count = current_char;
            max_char = str[i];
        }
    }

    return max_char;
}

static char find_repeated_char(Hand *hand, int target_amount, char ignore)
{
	char *card = malloc(strlen(hand->cards) + 1);
	strcpy(card, hand->cards);

	if (JOKER_RULE == 1) {
		char most_frequent_card = find_most_frequent_card(card);
		for(int i = 0; i < 5; i++) {
			if (card[i] == 'J')
				card[i] = most_frequent_card;
		}
	}

	int find_counter = 0;
	char result;
	for(int i = 0; i < 5; i++) {
		find_counter = 1;

		if (card[i] == ignore) {
			continue;
		}

		for (int j = i+1; j < 5; j++) {
			if (card[i] == card[j]) {
				find_counter++;
			}

			if (find_counter == target_amount) {

				result = card[i];
				free(card);

				return result;
			}
		}
	}

	free(card);

	return ' ';
}


static int is_one(Hand *hand)
{
	return find_repeated_char(hand, 2, ' ') != ' ';
}

static int is_two(Hand *hand)
{
	char found_char = find_repeated_char(hand, 2, ' ');
	if (found_char == ' ') {
		return 0;
	}

	return find_repeated_char(hand, 2, found_char) != ' ';
}


static int is_full(Hand *hand)
{
	char found_char = find_repeated_char(hand, 3, ' ');
	if (found_char == ' ') {
		return 0;
	}

	return find_repeated_char(hand, 2, found_char) != ' ';
}

static int is_three(Hand *hand)
{
	return find_repeated_char(hand, 3, ' ') != ' ';
}

static int is_four(Hand *hand)
{
	return find_repeated_char(hand, 4, ' ') != ' ';
}

static int is_five(Hand *hand)
{
	return find_repeated_char(hand, 5, ' ') != ' ';
}

static void insert_hand(Hand *hands, Hand *hand)
{
		Hand *current = hands;

		while(current->cards != NULL) {
			current++;
		}

		current->cards = hand->cards;
		current->bid = hand->bid;
		current->type = hand->type;
}

static int card_value(char card)
{
	if (JOKER_RULE == 1 && card == 'J')
		return 1;

	switch(card) {
		case 'A':
			return 14;
		case 'K':
			return 13;
		case 'Q':
			return 12;
		case 'J':
			return 11;
		case 'T':
			return 10;
		default:
			return convert_integer(&card);
	}
}

static int compare_cards(const void *p1, const void *p2)
{
	Hand *first = (Hand*) p1;
	Hand *second = (Hand*) p2;

	if (first->cards == NULL)
		return -1;

	if (second->cards == NULL)
		return -1;

	int is_equal_type = first->type == second->type;

	if (is_equal_type == 1) {
		char *first_cards = first->cards;
		char *second_cards = second->cards;

		int first_card_value;
		int second_card_value;

		for(int i = 0; i < 5; i++) {
			first_card_value = card_value(first_cards[i]);
			second_card_value = card_value(second_cards[i]);

			if (first_card_value < second_card_value) {
				return -1;
			} else if (first_card_value > second_card_value) {
				return 1;
			}
		}

	} else if (first->type < second->type) {
		return -1;
	}

	return 1;
}

static void categorize(Hand *hand)
{
	if (is_five(hand) == 1) {
		hand->type = FIVE;
	} else if (is_four(hand) == 1) {
		hand->type = FOUR;
	} else if (is_full(hand) == 1) {
		hand->type = FULL;
	} else if (is_three(hand) == 1) {
		hand->type = THREE;
	} else if (is_two(hand) == 1) {
		hand->type = TWO;
	} else if (is_one(hand) == 1) {
		hand->type = ONE;
	} else {
		hand->type = HIGH;
	}
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	int input_number = 0;

	Hand hands[MAX_HANDS];

	for(int i = 0; i < MAX_HANDS; i++) {
		hands[i].cards = NULL;
		hands[i].bid = 0;
	}

	Hand *hand = NULL;
	while(getline(&line, &len, stdin) != -1) {
		hand = new_hand(line);

		categorize(hand);

		insert_hand(hands, hand);

		free(hand);

		input_number++;
	}

	free(line);

	qsort(hands, input_number, sizeof(Hand), compare_cards);

	int total_winning = 0;
	for(int i = 0; i < input_number; i++) {
		total_winning += (hands[i].bid * (i+1));
		/* printf("Card: %s Bid: %d Type: %s\n", hands[i].cards, hands[i].bid, type_to_s(hands[i].type)); */
	}

	printf("%d\n", total_winning);

	return 0;
}

// First answer 247815719.
// Second answer 248747492.
