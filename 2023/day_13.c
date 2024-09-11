#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SET 100
#define MAX_V   20
#define MAX_H   20

typedef struct _pattern_set {
	char line[MAX_V][MAX_H];
	int  rows;
	int  columns;
} Pattern;

static void add_line(Pattern *pattern, char *line, int current_line)
{
	size_t line_len = strlen(line);
	for(int i = 0; i < line_len; i++) {
		if (line[i] == '\n') {
			pattern->line[current_line][i] = '\0';
			break;
		}
		pattern->line[current_line][i] = line[i];
	}

	pattern->rows    = current_line + 1;
	pattern->columns = line_len;
}

static int is_perfect(Pattern *pattern, int axis, char direction)
{
	int backward = axis - 2;
	int forward = axis + 1;

	if (direction == 'v') {
		while (backward >= 0 && forward < pattern->rows) {
			for(int i = 0; i < pattern->columns - 1; i++) {
				if (pattern->line[backward][i] != pattern->line[forward][i])
					return 0;
			}

			backward--;
			forward++;
		}

	} else {
		while (backward >= 0 && forward < pattern->columns - 1) {
			for(int i = 0; i < pattern->rows; i++) {
				if (pattern->line[i][backward] != pattern->line[i][forward])
					return 0;
			}

			backward--;
			forward++;
		}

	}

	return 1;
}

static int count_perfect_reflection_row(Pattern *pattern)
{
	int mirror_row = -1;
	for(int i = 0; i < pattern->rows - 1; i++) {
		for(int j = 0; j < pattern->columns; j++) {
			if (pattern->line[i][j] != pattern->line[i+1][j]) {
				mirror_row = -1;
				break;
			} else {
				mirror_row = i + 1;
			}
		}

		if (mirror_row > -1 && is_perfect(pattern, mirror_row, 'v') == 1)
			break;
	}

	if (mirror_row < -1)
		return 0;

	return (mirror_row > 0) ? mirror_row : 0;
}

static int count_perfect_reflection_column(Pattern *pattern)
{
	int mirror_column = -1;
	for(int i = 0; i < pattern->columns - 1; i++) {
		for(int j = 0; j < pattern->rows; j++) {
			if (pattern->line[j][i] != pattern->line[j][i+1]) {
				mirror_column = -1;
				break;
			} else {
				mirror_column = i + 1;
			}
		}

		if (mirror_column > -1 && is_perfect(pattern, mirror_column, 'h') == 1)
			break;
	}

	return (mirror_column > 0) ? mirror_column : 0;
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
