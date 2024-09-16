#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 101
#define LOOPS (1000000000)

static void print_platform(char platform[MAX][MAX], int south_row)
{
	for(int i = 0; i < south_row; i++) {
		printf("%s\n", platform[i]);
	}
}

static void tilt(char platform[MAX][MAX], int south_row, char direction)
{
	size_t max_column = strlen(platform[0]);

	int moved_rocks = 0;

	do {
		moved_rocks = 0;

		int start_line = 0;
		int start_column = 0;

		switch(direction){
			case 'N':
				start_line = south_row;
				start_column = 0;
				break;

			case 'S':
				start_line = 0;
				start_column = 0;
				break;

			case 'E':
				start_line = 0;
				start_column = 0;
				break;

			case 'W':
				start_line = 0;
				start_column = max_column;
				break;
		}

		for(int i = 0; i < south_row; i++) {
			int current_line = abs(start_line - i);
			int next_line = current_line;

			if (direction == 'N')
				next_line = current_line - 1;

			if (direction == 'S')
				next_line = current_line + 1;

			for(int j = 0; j < max_column; j++) {
				int current_column = abs(start_column - j);
				int next_column = current_column;

				if (direction == 'W')
					next_column = current_column - 1;

				if (direction == 'E')
					next_column = current_column + 1;

				char current_char = platform[current_line][current_column];
				char direction_char = platform[next_line][next_column];

				if (direction_char != '.' || current_char != 'O')
					continue;

				moved_rocks = 1;

				platform[current_line][current_column] = '.';
				platform[next_line][next_column] = 'O';
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

static void do_cycle(char platform[MAX][MAX], int south_row)
{
	tilt(platform, south_row, 'N');
	tilt(platform, south_row, 'W');
	tilt(platform, south_row, 'S');
	tilt(platform, south_row, 'E');
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

static int compare_platforms(char platform[MAX][MAX], char picture[MAX][MAX], int south_row)
{

	size_t max_column = strlen(platform[0]);
	for(int i = 0; i < south_row; i++) {
		for(int j = 0; j < max_column; j++) {
			if (platform[i][j] == '\0')
				break;

			if (platform[i][j] != picture[i][j])
				return 0;
		}
	}

	return 1;
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	char platform[MAX][MAX];
	char platform_picture[MAX][MAX];

	int row_index = 0;
	while (getline(&line, &len, stdin) != -1) {
		process_platform(platform, line, row_index);
		row_index++;
	}

	tilt(platform, row_index, 'N');
	int part_1 = compute_weight(platform, row_index);

	int start = 0;
	int cycle = 0;
	for(int i = 0; i < LOOPS; i++) {
		// Take a picture of the platform every 100 cycles.
		if (i % 100 == 0) {
			memcpy(platform_picture, platform, sizeof(platform));
			start = i;
		}

		do_cycle(platform, row_index);

		// If it turns out to be the same, it means the platform is just repeating
		// states and we can skip ahead and save computation.
		if (memcmp(platform_picture, platform, sizeof(platform)) == 0) {
			cycle = i - start + 1;

			while (i <= LOOPS)
				i += cycle;

			i -= cycle;
		}
	}

	int part_2 = compute_weight(platform, row_index);

	printf("%d %d\n", part_1, part_2);

	return 0;
}
