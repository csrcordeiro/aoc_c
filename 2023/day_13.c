#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SET 100
#define MAX_V   20
#define MAX_H   20

typedef struct _pattern_set {
	char line[MAX_V][MAX_H];
	char invert_line[MAX_H][MAX_V];
	int  rows;
	int  columns;
} Pattern;

static void add_line(Pattern *pattern, char *line, int current_line)
{
	size_t line_len = strlen(line) - 1;

	for(int i = 0; i < line_len; i++) {
		pattern->line[current_line][i] = line[i];
	}

	pattern->rows    = current_line + 1;
	pattern->columns = line_len;
}

static void define_invert_line(Pattern *pattern)
{
	for(int i = 0; i < pattern->columns; i++) {
		for(int j = 0; j < pattern->rows; j++) {
			pattern->invert_line[i][j] = pattern->line[j][i];
		}
	}
}

static int is_perfect(Pattern *pattern, int index, char direction)
{
	int backward = index - 2;
	int forward = index + 1;
	int limit = pattern->rows;

	if (direction == 'v')
		limit = pattern->columns;

	char *back_line;
	char *for_line;

	while (backward >= 0 && forward < limit) {
		if (direction == 'h') {
			back_line = pattern->line[backward];
			for_line = pattern->line[forward];

		} else {
			back_line = pattern->invert_line[backward];
			for_line = pattern->invert_line[forward];
		}

		int len = strlen(back_line);

		for(int i = 0; i < len; i++) {
			if (back_line[i] != for_line[i])
				return 0;
		}

		backward--;
		forward++;
	}

	return 1;
}

static int is_equal(char *line_1, char *line_2)
{
	if (strcmp(line_1, line_2) != 0)
		return 0;

	return 1;
}

static int count_perfect_reflection_row(Pattern *pattern)
{
	int mirror_row = -1;
	for(int i = 0; i < pattern->rows - 1; i++) {
		int next_index = i + 1;

		if (is_equal(pattern->line[i], pattern->line[next_index]) == 1) {
			if (is_perfect(pattern, next_index, 'h') == 1)
				mirror_row = next_index;
		}
	}

	return (mirror_row > -1) ? mirror_row : 0;
}

static int count_perfect_reflection_column(Pattern *pattern)
{
	int mirror_column = -1;
	for(int i = 0; i < pattern->columns; i++) {
		int next_index = i + 1;

		if (is_equal(pattern->invert_line[i], pattern->invert_line[next_index]) == 1) {
			if (is_perfect(pattern, next_index, 'v') == 1)
				mirror_column = next_index;
		}
	}

	return (mirror_column > -1) ? mirror_column : 0;
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	Pattern *set[MAX_SET] = { NULL };

	Pattern *current = NULL;
	int pattern_set_count = 0;
	int current_line = 0;
	while(getline(&line, &len, stdin) != -1) {
		if (current == NULL) {
			current = malloc(sizeof(Pattern));
			set[pattern_set_count] = current;
			pattern_set_count++;
		}

		if (*line == '\n') {
			current_line = 0;
			current = NULL;

			continue;
		}

		add_line(current, line, current_line);

		current_line++;
	}

	int summarize = 0;
	for(int i = 0; i < MAX_SET; i++) {
		if (set[i] == NULL)
			break;

		define_invert_line(set[i]);

		int row_reflection = count_perfect_reflection_row(set[i]);

		if (row_reflection > 0) {
			summarize += row_reflection * 100;
		} else {
			summarize += count_perfect_reflection_column(set[i]);
		}
	}

	printf("%d\n", summarize);

	for(int i = 0; i < MAX_SET; i++) {
		if (set[i] == NULL)
			break;

		free(set[i]);
	}

	free(line);

	return 0;
}
