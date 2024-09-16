#include <stdio.h>
#include <string.h>

#define MAX 200

static void print_platform(char platform[MAX][MAX], int south_row)
{
	for(int i = 0; i < south_row; i++) {
		printf("%s\n", platform[i]);
	}
}

static void tilt_north(char platform[MAX][MAX], int south_row)
{
	size_t max_column = strlen(platform[0]);

	int moved_rocks = 0;

	do {
		moved_rocks = 0;

		for(int i = south_row; i >= 0; i--) {
			for(int j = 0; j < max_column; j++) {
				char current_char = platform[i][j];
				char upper_char = platform[i-1][j];


				if (upper_char != '.' || current_char != 'O')
					continue;

				moved_rocks = 1;

				platform[i][j]   = '.';
				platform[i-1][j] = 'O';
			}

		}

	} while (moved_rocks == 1);
}

static void process_platform(char platform[MAX][MAX], char *line, int current_line)
{
	for(int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
		platform[current_line][i] = line[i];
	}
}

static int compute_weight(char platform[MAX][MAX], int south_row)
{
	size_t max_column = strlen(platform[0]);
	int result = 0;

	for(int j = 0; j < max_column; j++) {
		for(int i = 0; i < south_row; i++) {
			if (platform[i][j] != 'O')
				continue;

			int weight_factor = south_row - i;

			result += weight_factor;
		}
	}

	return result;
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	char platform[MAX][MAX];

	int row_index = 0;
	while (getline(&line, &len, stdin) != -1) {
		process_platform(platform, line, row_index);
		row_index++;
	}

	tilt_north(platform, row_index);

	int weight = compute_weight(platform, row_index);

	printf("%d\n", weight);

	return 0;
}
