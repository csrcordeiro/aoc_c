#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#define MAX_IN        1000000
#define DEFAULT_SPACE 100

typedef struct _lens {
	char *line;
	char *label;
	char operation;
	int  power;
} Lens;

typedef struct _box {
	int  id;
	Lens *lenses[DEFAULT_SPACE];
	struct _box *next;
} Box;

static void print_boxes(Box *head)
{
	Box *current = head;
	while (current != NULL) {
		printf("Box %d ", current->id);

		for(int i = 0; current->lenses[i] != NULL; i++) {
			printf("[%s %d] ", current->lenses[i]->label, current->lenses[i]->power);
		}

		printf("\n\n");

		current = current->next;
	}
}

static char extract_operation(char *line)
{
	while(line != NULL) {
		if (*line == '\0')
			break;

		if (*line == '=')
			return '=';

		line++;
	}

	return '-';
}

static char *extract_label(char *line)
{
	size_t len = strlen(line) + 1;
	char *label = (char*) malloc(sizeof(char) * len);

	for(int i = 0; i < len; i++) {
		if (line[i] == '=' || line[i] == '-')
			break;

		label[i] = line[i];
	}

	return label;
}

static int extract_power(char *line)
{
	size_t len = strlen(line) + 1;

	for(int i = 0; i < len; i++) {
		if (line[i] == '=')
			return convert_integer(&line[i+1]);
	}

	return 0;
}

static int extract_lens(Lens *lens[MAX_IN], char *line)
{
	int number_of_inputs = 0;
	Lens *current_lens = NULL;
	for(char *seq = strtok(line, ","); seq != NULL; seq = strtok(NULL, ",")) {
		current_lens = malloc(sizeof(Lens));

		current_lens->line = seq;
		current_lens->label = extract_label(seq);
		current_lens->power = extract_power(seq);
		current_lens->operation = extract_operation(seq);

		lens[number_of_inputs] = current_lens;
		number_of_inputs++;
	}

	return number_of_inputs;
}

static int calculate_hash(char *line)
{
	int hash = 0;
	char *line_copy = line;

	while(line_copy != NULL) {
		if (*line_copy == '\n' || *line_copy == '\0')
			break;

		char character = *line_copy;

		hash += (int) character;
		hash *= 17;

		hash %= 256;

		line_copy++;
	}

	return hash;
}

static void insert_lens_in_correct_order(Box *box, Lens *lens)
{
	if (lens->operation == '=') {
		for(int i = 0; i < DEFAULT_SPACE; i++) {
			if (box->lenses[i] == NULL || strcmp(box->lenses[i]->label, lens->label) == 0) {
				box->lenses[i] = lens;
				break;
			}
		}

	} else {
		int remove_index = -1;

		for(int i = 0; box->lenses[i] != NULL; i++) {
			if (strcmp(box->lenses[i]->label, lens->label) == 0) {
				remove_index = i;
				break;
			}
		}

		if (remove_index > -1) {

			for(int i = remove_index; box->lenses[i] != NULL; i++) {
				box->lenses[i] = box->lenses[i+1];
			}

		}
	}
}

static Box *create_new_box(Lens *lens, int hash)
{
	Box *box = NULL;

	box = malloc(sizeof(Box));
	box->id = hash;
	box->next = NULL;

	for(int i = 0; i < DEFAULT_SPACE; i++)
		box->lenses[i] = NULL;

	if (lens->operation == '=')
		box->lenses[0] = lens;

	return box;
}

static void insert_lens_in_box(Box **box, Lens *lens)
{
	int hash = calculate_hash(lens->label);

	if (*box == NULL) {
		*box = create_new_box(lens, hash);

	} else {
		Box *current = *box;
		while (current != NULL && current->id != hash)
			current = current->next;

		if (current == NULL) {
			current = *box;

			while (current->next != NULL)
				current = current->next;

			current->next = create_new_box(lens, hash);

		} else {
			insert_lens_in_correct_order(current, lens);
		}
	}
}

static int calculate_focus_power(Box *box)
{
	int result = 0;

	int power = 0;
	int slot_number = 0;
	int lens_total_power = 0;

	for(int i = 0; box->lenses[i] != NULL; i++) {
		power = box->lenses[i]->power;
		slot_number = i + 1;

		lens_total_power = (box->id + 1) * slot_number * power;

		result += lens_total_power;
	}

	return result;
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	Lens *lens[MAX_IN];
	int number_of_inputs = 0;

	getline(&line, &len, stdin);

	number_of_inputs = extract_lens(lens, line);

	int part_1 = 0;
	int part_2 = 0;

	Box *box = NULL;
	for(int i = 0; i < number_of_inputs; i++) {
		part_1 += calculate_hash(lens[i]->line);

		insert_lens_in_box(&box, lens[i]);
	}

	Box *current_box = box;
	while (current_box != NULL) {
		part_2 += calculate_focus_power(current_box);

		current_box = current_box->next;
	}

	printf("%d %d\n", part_1, part_2);

	current_box = box;
	Box *box_to_free = current_box;
	while (current_box != NULL) {
		box_to_free = current_box;

		current_box = current_box->next;

		free(box_to_free);
	}

	for(int i = 0; i < number_of_inputs; i++) {
		free(lens[i]);
	}

	return 0;
}
