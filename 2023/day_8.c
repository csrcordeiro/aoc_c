#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATHS 760
#define AAA (656565)
#define ZZZ (909090)

typedef struct _path {
	char *original_line;
	int id;
	int left;
	int right;
} Path;

static int convert_path_to_integer(char *position)
{
	int number = 0;
	int factor = 10000;
	int ascii_code = 0;
	while(*position != '\0' && position != NULL) {
		ascii_code = (int) *position;
		number +=  ascii_code * factor;
		factor = factor / 100;
		position++;
	}

	return number;
}

static void process_path(char *line, Path *paths, int index)
{
	Path *current = &paths[index];

	current->original_line = malloc(strlen(line)+1);
	strcpy(current->original_line, line);

	char id[4], left[4], right[4];

	sscanf(line, "%3s = (%3s, %3s)", id, left, right);

	current->id = convert_path_to_integer(id);
	current->left = convert_path_to_integer(left);
	current->right = convert_path_to_integer(right);
}

static Path *find_path(Path *paths, int new_path_id, int total_number_of_paths)
{
	Path *path = NULL;
	for(int i = 0; i < total_number_of_paths; i++) {
		path = &(paths[i]);
		if (path->id == new_path_id) {
			return path;
		}
	}

	return NULL;
}

static int compare_paths(const void *p1, const void *p2)
{
	Path *path_1 = (Path*) p1;
	Path *path_2 = (Path*) p2;

	return (path_1->id - path_2->id);
}

int main(int argc, char **argv)
{
	char *line;
	size_t len = 0;

	getline(&line, &len, stdin);

	int total_direction_size = strlen(line);

	char *directions = malloc(total_direction_size + 1);
	strcpy(directions, line);

	// Skip empty line.
	getline(&line, &len, stdin);

	Path paths[MAX_PATHS];

	int total_input = 0;
	while(getline(&line, &len, stdin) != -1) {
		process_path(line, paths, total_input);

		total_input++;
	}

	qsort(paths, total_input, sizeof(Path), compare_paths);

	Path *current_position = paths;
	int hops = 0;
	int direction_index = 0;
	do {
		if (direction_index >= total_direction_size - 1)
			direction_index = 0;

		char direction = directions[direction_index];

		if (direction == 'L') {
			current_position = find_path(paths, current_position->left, total_input);
		} else {
			current_position = find_path(paths, current_position->right, total_input);
		}

		direction_index++;
		hops++;
	} while(current_position->id != ZZZ);

	printf("%d\n", hops);

	free(line);
	return 0;
}

