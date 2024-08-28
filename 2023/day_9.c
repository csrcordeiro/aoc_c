#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#define MAX_ENTRIES 200

typedef struct _entry {
	long *numbers;
	int  total_numbers;
	long prediction;
} Entry;

static void read_entry(char *line, Entry *entry)
{
	long values[100];
	char *tok = NULL;
	int i = 0;
	int number_count = 0;

	for (tok = strtok(line, " "), i = 0; tok != NULL; i++, tok = strtok(NULL, " "), number_count++) {
		values[i] = convert_integer(tok);
	}

	size_t total_numbers_size = number_count * sizeof(long);
	entry->numbers = (long*) malloc(total_numbers_size);
	memcpy(entry->numbers, values, total_numbers_size);

	entry->total_numbers = number_count;
}

static void make_prediction(Entry *entry)
{
	long layers[entry->total_numbers];
	long last_prediction_processed[entry->total_numbers];
	int prediction_index = 0;
	int all_zeros_prediction = 1;
	int total_numbers = entry->total_numbers;

	last_prediction_processed[0] = entry->numbers[total_numbers - 1];

	for(int i = 0; i < total_numbers; i++) {
		layers[i] = entry->numbers[i];
	}

	do {
		all_zeros_prediction = 1;
		total_numbers -= 1;

		long diff = 0;
		for(int i = 0; i < total_numbers; i++) {
			diff = (layers[i + 1] - layers[i]);

			layers[i] = diff;

			if (diff != 0)
				all_zeros_prediction = 0;
		}

		last_prediction_processed[prediction_index] = layers[total_numbers];
		prediction_index++;

	} while (all_zeros_prediction != 1);

	entry->prediction = 0;
	for(int i = 0; i < prediction_index; i++) {
		entry->prediction += last_prediction_processed[i];
	}
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	Entry *entries[MAX_ENTRIES];
	for(int i = 0; i < MAX_ENTRIES; i++) {
		entries[i] = NULL;
	}

	int entry_index = 0;
	Entry *entry = NULL;
	while(getline(&line, &len, stdin) != -1) {
		entry = malloc(sizeof(Entry));

		read_entry(line, entry);

		entries[entry_index] = entry;

		entry_index++;
	}

	long sum_of_predictions = 0;
	for(int i = 0; i < MAX_ENTRIES; i++) {
		if (entries[i] == NULL)
			break;

		make_prediction(entries[i]);

		sum_of_predictions += entries[i]->prediction;
	}


	printf("%ld\n", sum_of_predictions);

	free(line);
	for(int i = 0; i < MAX_ENTRIES; i++) {
		free(entries[i]);
	}

	return 0;
}

// Part 1: 1974913025.
