#include <stdio.h>
#include <stdlib.h>

#define MAX_GRID 140

typedef struct _point {
	int id;
	int x;
	int y;
	int distance_between_pairs;
	struct _point *next;
} Point;

static void print_grid(char **grid, int y, int x)
{
	for(int i = 0; i < y; i++) {
		for(int j = 0; j < x; j++) {
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}

static void plot_grid(char grid[MAX_GRID][MAX_GRID], char *line, int current_size)
{
	for(int i = 0; i < MAX_GRID; i++) {
		grid[current_size][i] = line[i];
	}
}

static void find_expanded_dimentions(char grid[MAX_GRID][MAX_GRID], int universe_size, int *col_empty, int *row_empty)
{
	for(int i = 0; i < universe_size; i++) {
		col_empty[i] = 1;
		row_empty[i] = 1;
	}

	for(int i = 0; i < universe_size; i++) {
		for(int j = 0; j < universe_size; j++) {
			if (grid[i][j] == '#') {
				col_empty[j] = 0;
				row_empty[i] = 0;
			}
		}
	}
}

static char** expand_grid(
		char grid[MAX_GRID][MAX_GRID],
		int universe_size,
		int *col_empty_galaxies,
		int *row_empty_galaxies,
		int dimention_x,
		int dimention_y
)
{
	char **new_grid = (char**) malloc(dimention_y * sizeof(char*));
	for(int i = 0; i < dimention_x; i++) {
		new_grid[i] = (char*) malloc(dimention_x * sizeof(char));
	}


	int new_grid_x = 0;
	int new_grid_y = 0;
	for(int i = 0; i < universe_size; i++) {
		new_grid_x = 0;

		if (row_empty_galaxies[i] == 1) {
			for(int k = 0; k < dimention_x; k++) {
				new_grid[new_grid_y][k] = '.';
				new_grid[new_grid_y + 1][k] = '.';
			}
			new_grid_y += 2;

			continue;
		}

		for(int j = 0; j < universe_size; j++) {
			if (col_empty_galaxies[j] == 1) {
				new_grid[new_grid_y][new_grid_x] = '.';

				new_grid_x++;

				new_grid[new_grid_y][new_grid_x] = '.';
			} else {
				new_grid[new_grid_y][new_grid_x] = grid[i][j];
			}

			new_grid_x++;
		}

		new_grid_y++;
	}

	return new_grid;
}

static int count_new_dimention(int *empty_dimentions, int universe_size)
{
	int new_dimention = universe_size;
	for(int i = 0; i < universe_size; i++) {
		if (empty_dimentions[i] == 1)
			new_dimention++;
	}

	return new_dimention;
}

static void insert_in_list(Point **head, Point *element)
{
	if (*head == NULL) {
		*head = element;
	} else {
		Point *current = *head;
		while (current->next != NULL)
			current = current->next;

		current->next = element;
	}
}

static void print_points(Point *head)
{
	Point *current = head;
	while (current != NULL) {
		printf("id: %d (%d, %d) dist: %d ", current->id, current->x, current->y, current->distance_between_pairs);
		current = current->next;
	}
	printf("\n");
}

static Point* find_points(char **grid, int dimention_x, int dimention_y)
{
	Point *head = NULL;
	Point *new_point = NULL;
	int counter = 1;
	for(int i = 0; i < dimention_y; i++) {
		for(int j = 0; j < dimention_x; j++) {
			if (grid[i][j] == '#') {
				new_point = malloc(sizeof(Point));

				new_point->id = counter;
				new_point->x = j;
				new_point->y = i;

				insert_in_list(&head, new_point);

				counter++;
			}
		}
	}

	return head;
}

static int compute_distance(Point *current, Point *other)
{
	int a = abs(current->x - other->x);
	int b = abs(current->y - other->y);

	return a + b;
}

static void calculate_distances(Point *head)
{
	Point *current = head;
	while(current->next != NULL) {
		Point *other = current->next;

		while(other != NULL) {
			int distance = compute_distance(current, other);

			current->distance_between_pairs += distance;

			other = other->next;
		}

		current = current->next;
	}
}

static int part_1_result(Point *head)
{
	Point *c = head;
	int sum_of_distance = 0;
	while (c != NULL) {
		sum_of_distance += c->distance_between_pairs;

		c = c->next;
	}

	return sum_of_distance;
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	char grid[MAX_GRID][MAX_GRID];

	int universe_size = 0;
	while(getline(&line, &len, stdin) != -1) {
		plot_grid(grid, line, universe_size);

		universe_size++;
	}


	int col_empty_galaxies[universe_size];
	int row_empty_galaxies[universe_size];

	find_expanded_dimentions(grid, universe_size, col_empty_galaxies, row_empty_galaxies);

	int dimention_x = count_new_dimention(col_empty_galaxies, universe_size);
	int dimention_y = count_new_dimention(row_empty_galaxies, universe_size);

	char **new_grid = expand_grid(
			grid,
			universe_size,
			col_empty_galaxies,
			row_empty_galaxies,
			dimention_x,
			dimention_y
	);

	Point *head = find_points(new_grid, dimention_x, dimention_y);

	calculate_distances(head);

	printf("%d\n", part_1_result(head));

	free(line);

	for(int i = 0; i < dimention_y; i++) {
		free(new_grid[i]);
	}

	Point *current = head;
	Point *next = NULL;
	while(current != NULL) {
		next = current->next;

		free(current);

		current = next;
	}

	free(new_grid);

	return 0;
}
