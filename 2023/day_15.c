#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IN 1000000

typedef struct _sequence {
	char *line;
} Sequence;

static int extract_sequence(Sequence *sequence[MAX_IN], char *line)
{
	int number_of_inputs = 0;
	Sequence *current_sequence = NULL;
	for(char *seq = strtok(line, ","); seq != NULL; seq = strtok(NULL, ",")) {
		current_sequence = malloc(sizeof(Sequence));

		current_sequence->line = seq;

		sequence[number_of_inputs] = current_sequence;
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

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	Sequence *sequence[MAX_IN];
	int number_of_inputs = 0;

	getline(&line, &len, stdin);

	number_of_inputs = extract_sequence(sequence, line);

	int total_hash = 0;
	for(int i = 0; i < number_of_inputs; i++) {
		total_hash += calculate_hash(sequence[i]->line);
	}

	printf("%d\n", total_hash);

	for(int i = 0; i < number_of_inputs; i++) {
		free(sequence[i]);
	}

	return 0;
}
