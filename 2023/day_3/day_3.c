// First  answer 540025
// Second answer 84584891
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct number_info {
    int number;
    int line;
    int range_begin;
    int range_end;
    struct number_info *next;
} NumberInfo;

typedef struct token_info {
    char token;
    int line;
    int position;
    struct token_info *next;
} TokenInfo;

static NumberInfo *first_number;
static TokenInfo  *first_token;

static int all_parts;
static int gear_ratio;

void print_number_info()
{
    NumberInfo *current = first_number;
    while(current != NULL) {
	printf("%d %d:[%d, %d] -> ", current->number, current->line, current->range_begin, current->range_end);
	current = current->next;
    }
    printf("\n");
}

void print_token_info()
{
    TokenInfo *current = first_token;
    while(current != NULL) {
	printf("%c %d:[%d] -> ", current->token, current->line, current->position);
	current = current->next;
    }
    printf("\n");
}

void insert_new_number(int number, int start, int end, int line_number)
{
    NumberInfo *number_info = malloc(sizeof(NumberInfo));
    number_info->number = number;
    number_info->line = line_number;
    number_info->range_begin = start;
    number_info->range_end = (start + end) - 1;

    if (first_number == NULL) {
	first_number = number_info;
    } else {
	NumberInfo *current_number_info = first_number;
	while(current_number_info->next != NULL)
	    current_number_info = current_number_info->next;

	current_number_info->next = number_info;
    }
}

void insert_new_token(char token, int index, int line_number)
{
    TokenInfo *token_info = malloc(sizeof(TokenInfo));
    token_info->token = token;
    token_info->line = line_number;
    token_info->position = index;

    if (first_token == NULL) {
	first_token = token_info;
    } else {
	TokenInfo *current_token_info = first_token;
	while(current_token_info->next != NULL)
	    current_token_info = current_token_info->next;

	current_token_info->next = token_info;
    }
}

int process_number(char *line, int index, size_t max_char, int line_number)
{
    int n = 0;
    int number = 0;
    for (int i = index; i < max_char; i++) {
	if (isdigit(line[i])) {
	    // Using ASCII arithmetics.
	    number = number * 10 + (line[i] - 48);
	} else {
	    break;
	}
	n++;
    }

    insert_new_number(number, index, n, line_number);

    return n;
}

void extract_info(char *line, int line_number)
{
    size_t size = strlen(line);
    int index = 0;

    while (index < size) {
	if (isdigit(line[index])) {
	    int processed = process_number(line, index, size, line_number);
	    index+=processed;
	    continue;
	} else if(line[index] != '.' && line[index] != '\0' && line[index] != '\n') {
	    insert_new_token(line[index], index, line_number);
	}

	index++;
    }
}

NumberInfo* search_first_number_in_line(int line_number)
{
    NumberInfo *current = first_number;
    while(current != NULL) {
	if (current->line == line_number) {
	    return current;
	}
	current = current->next;
    }

    return current;
}

bool in_range(int position, int start, int end)
{
    if (position >= start && position <= end)
	return true;

    if (position - 1 >= start && position - 1 <= end)
	return true;

    if (position + 1 >= start && position + 1 <= end)
	return true;

    return false;
}

void process_line(TokenInfo *current_token, int line_number)
{
    NumberInfo *number = search_first_number_in_line(line_number);

    while (number != NULL && number->line == line_number) {
	if(in_range(current_token->position, number->range_begin, number->range_end)) {
	    all_parts += number->number;
	}

	number = number->next;
    }

}

int number_of_adjacent_gears(TokenInfo *current_token, int line_number, int* value_gears)
{
    int adjacent_gears = 0;
    NumberInfo *number = search_first_number_in_line(line_number);

    while (number != NULL && number->line == line_number) {
	if(in_range(current_token->position, number->range_begin, number->range_end)) {
	    adjacent_gears++;
	    *value_gears *= number->number;
	}

	number = number->next;
    }

    return adjacent_gears;
}

void process_possible_gear(TokenInfo *current_token)
{
    if(current_token->token != '*') {
	return;
    }

    int value_gears = 1;
    int gears_found = 0;

    gears_found += number_of_adjacent_gears(current_token, current_token->line - 1, &value_gears);
    gears_found += number_of_adjacent_gears(current_token, current_token->line, &value_gears);
    gears_found += number_of_adjacent_gears(current_token, current_token->line + 1, &value_gears);

    if(gears_found == 2) {
	gear_ratio += value_gears;
    }
}

void process_all_token()
{
    TokenInfo *current_token = first_token;
    while(current_token != NULL) {
	process_possible_gear(current_token);

	int position = current_token->position;
	int line = current_token->line;

	process_line(current_token, line - 1);
	process_line(current_token, line);
	process_line(current_token, line + 1);

	current_token = current_token->next;
    }
}


int main(void)
{
    char *line = NULL;
    size_t len = 0;

    int line_number = 0;
    while (getline(&line, &len, stdin) != -1) {
	extract_info(line, line_number);
	line_number++;
    }

    process_all_token();
    printf("%d %d\n", all_parts, gear_ratio);

    free(line);

    return 0;
}
