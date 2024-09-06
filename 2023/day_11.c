#include <stdio.h>
#include <stdlib.h>

#define MAX_GRID 140
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define ll long long

/* #define INCREASE_BY (1) */
#define INCREASE_BY (999999)


typedef struct _point {
	int id;
	int x;
	int y;
	struct _point *next;
} Point;

static void print_grid(char grid[MAX_GRID][MAX_GRID], int y, int x)
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

static void find_expanded_dimentions(char grid[MAX_GRID][MAX_GRID], int *col_empty, int *row_empty,int universe_size)
{
	for(int i = 0; i < universe_size; i++) {
		col_empty[i] = 1;
		row_empty[i] = 1;
	}

	for(int i = 0; i < universe_size; i++) {
		for(int j = 0; j < universe_size; j++) {
			if (grid[i][j] == '#') {
				col_empty[i] = 0;
				row_empty[j] = 0;
			}
		}
	}
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
		printf("id: %d (%d, %d)", current->id, current->x, current->y);
		current = current->next;
	}
	printf("\n");
}

static Point* find_points(char grid[MAX_GRID][MAX_GRID])
{
	Point *head = NULL;
	Point *new_point = NULL;
	int counter = 1;
	for(int i = 0; i < MAX_GRID; i++) {
		for(int j = 0; j < MAX_GRID; j++) {
			if (grid[i][j] == '\0')
				break;

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

static ll compute_distance(Point *current, Point *other)
{
	ll a = abs(current->x - other->x);
	ll b = abs(current->y - other->y);

	return a + b;
}

static int compute_offset(Point *a, Point *b, int *row_empty, int *col_empty)
{
	int offset = 0;
	int min_y = MIN(a->y, b->y);
	int max_y = MAX(a->y, b->y);

	int min_x = MIN(a->x, b->x);
	int max_x = MAX(a->x, b->x);

	for(int i = min_x+1; i < max_x; i++) {
		if (col_empty[i] == 1) {
			offset += INCREASE_BY;
		}
	}

	for(int i = min_y+1; i < max_y; i++) {
		if (row_empty[i] == 1) {
			offset += INCREASE_BY;
		}
	}

	return offset;
}

static ll calculate_distances(Point *head, int *row_empty, int *col_empty)
{
	ll result = 0;
	Point *current = head;
	while(current->next != NULL) {
		Point *other = current->next;

		while(other != NULL) {
			ll distance = compute_distance(current, other);
			ll expansion = compute_offset(current, other, row_empty, col_empty);

			result += distance + expansion;

			other = other->next;
		}

		current = current->next;
	}

	return result;
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


	int row_empty[universe_size];
	int col_empty[universe_size];

	find_expanded_dimentions(grid, row_empty, col_empty,  universe_size);

	Point *head = find_points(grid);

	ll result = calculate_distances(head, row_empty, col_empty);

	printf("%lld\n", result);

	free(line);

	Point *current = head;
	Point *next = NULL;
	while(current != NULL) {
		next = current->next;

		free(current);

		current = next;
	}

	return 0;
}
