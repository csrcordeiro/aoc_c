/*
 * For part 2, this solution uses two algorithms to calculate the Area and
 * find out the number of inner points in the Area.
 *
 * To calculate Area.
 * https://en.wikipedia.org/wiki/Shoelace_formula
 *
 * Adapted to calculate the inner points in area.
 * https://en.wikipedia.org/wiki/Pick%27s_theorem
 *
 * Got the idea from this redditor:
 * https://www.reddit.com/r/adventofcode/comments/18evyu9/comment/kcqmhwk/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GRID 140
#define OUT_GRID ('X')

typedef struct _point {
	int x;
	int y;
} Point;

typedef struct _result {
	int steps;
	int tiles;
} Result;

static void print_grid(char grid[MAX_GRID][MAX_GRID])
{
	for(int i = 0; i < MAX_GRID; i++) {
		for(int j = 0; j < MAX_GRID; j++) {
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}

static void init_grid(char grid[MAX_GRID][MAX_GRID])
{
	for(int i = 0; i < MAX_GRID; i++) {
		for(int j = 0; j < MAX_GRID; j++) {
			grid[i][j] = OUT_GRID;
		}
	}
}

static void mark_grid(char grid[MAX_GRID][MAX_GRID], char *line, int current_y, Point *start)
{
	for(int i = 0; i < MAX_GRID; i++) {
		char symbol = line[i];

		if (symbol == 'S') {
			start->x = i;
			start->y = current_y;
		}

		grid[current_y][i] = symbol;
	}
}

/*
 * Mark the walk so we know wich direction to go.
 */
static void leave_breadcrumb(Point *prev, Point *current)
{
	prev->x = current->x;
	prev->y = current->y;
}

/*
 * Rules for walking the maze:
 *
 * | is a vertical pipe connecting north and south.
 * - is a horizontal pipe connecting east and west.
 * L is a 90-degree bend connecting north and east.
 * J is a 90-degree bend connecting north and west.
 * 7 is a 90-degree bend connecting south and west.
 * F is a 90-degree bend connecting south and east.
 * . is ground; there is no pipe in this tile.
 * S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.
 */
static void walk(char grid[MAX_GRID][MAX_GRID], Point *current_point, Point *previous_point)
{
	int p_x = previous_point->x;
	int p_y = previous_point->y;

	int c_x = current_point->x;
	int c_y = current_point->y;

	char direction = grid[c_y][c_x];

	int new_x = c_x;
	int new_y = c_y;

	switch(direction) {
		case 'J':
			if (p_x < c_x) {
				new_y = current_point->y-1;
			} else if (p_y < c_y) {
				new_x = current_point->x-1;
			}
			break;

		case 'L':
			if (p_y < c_y) {
				new_x = current_point->x+1;
			} else if (p_x > c_x) {
				new_y = current_point->y-1;
			}
			break;

		case '7':
			if (p_x < c_x) {
				new_y = current_point->y+1;
			} else if (p_y > c_y) {
				new_x = current_point->x-1;
			}
			break;

		case 'F':
			if (p_y > c_y) {
				new_x = current_point->x+1;
			} else if (p_x > c_x) {
				new_y = current_point->y+1;
			}
			break;
		case '|':
			if (p_y > c_y) {
				new_y = current_point->y-1;
			} else {
				new_y = current_point->y+1;
			}
			break;
		case '-':
			if (p_x < c_x) {
				new_x = current_point->x+1;
			} else {
				new_x = current_point->x-1;
			}
			break;
	}

	leave_breadcrumb(previous_point, current_point);

	current_point->x = new_x;
	current_point->y = new_y;
}

static int is_same_point(Point *a, Point *b)
{
	return a->x == b->x && a->y == b->y;
}

static void start_walking_and_counting_results(char grid[MAX_GRID][MAX_GRID], Point *start, Point *next, Result *result)
{
	Point *previous = malloc(sizeof(Point));
	previous->x = start->x;
	previous->y = start->y;

	Point *current = malloc(sizeof(Point));
	current->x = next->x;
	current->y = next->y;

	Point visited_points[MAX_GRID * MAX_GRID];

	int steps = 0;
	int two_times_the_area_of_polygon = 0;
	do {
		two_times_the_area_of_polygon += (previous->y + current->y) * (previous->x - current->x);

		walk(grid, current, previous);

		steps++;
	} while(is_same_point(start, current) == 0);

	result->steps = (steps + 1) / 2;
	result->tiles = (abs(two_times_the_area_of_polygon / 2)) - result->steps + 1;

	free(previous);
	free(current);
}

static int can_go(char grid[MAX_GRID][MAX_GRID], Point *start, char point)
{
	char direction;

	switch(point) {
		case 'N':
			direction = grid[start->y-1][start->x];
			if (direction == '|' || direction == 'F' || direction == '7')
				return 1;
			break;
		case 'S':
			direction = grid[start->y+1][start->x];
			if (direction == '|' || direction == 'J' || direction == 'L')
				return 1;
			break;
		case 'E':
			direction = grid[start->y][start->x+1];
			if (direction == '-' || direction == 'J' || direction == '7')
				return 1;
			break;

		case 'W':
			direction = grid[start->y][start->x-1];
			if (direction == '-' || direction == 'F' || direction == 'L')
				return 1;
			break;
	}

	return 0;
}


static void move(char grid[MAX_GRID][MAX_GRID], Point *start, char point, Result *result) {

	Point *next = malloc(sizeof(Point));
	next->x = start->x;
	next->y = start->y;

	switch(point) {
		case 'N':
			next->y--;
			break;
		case 'S':
			next->y++;
			break;
		case 'E':
			next->x++;
			break;
		case 'W':
			next->x--;
			break;

	}

	start_walking_and_counting_results(grid, start, next, result);

	free(next);
}

static Result *get_max(Result *results)
{
	Result *greatest = (&results[0]);
	Result *current = greatest;

	for(int i = 0; i < 4; i++) {
		current = &results[i];
		if (current->steps > greatest->steps)
			greatest = current;
	}

	return greatest;
}

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;

	char grid[MAX_GRID][MAX_GRID];

	init_grid(grid);

	int current_y = 0;
	Point *start = malloc(sizeof(Point));
	while(getline(&line, &len, stdin) != -1) {
		mark_grid(grid, line, current_y, start);

		current_y++;
	}

	Result paths[4];

	if (can_go(grid, start, 'E') == 1) {
		move(grid, start, 'E', &paths[0]);
	}

	if (can_go(grid, start, 'W') == 1) {
		move(grid, start, 'W', &paths[1]);
	}

	if (can_go(grid, start, 'N') == 1) {
		move(grid, start, 'N', &paths[2]);
	}

	if (can_go(grid, start, 'S') == 1) {
		move(grid, start, 'S', &paths[3]);
	}

	Result *result = get_max(paths);

	printf("%d %d\n", result->steps, result->tiles);

	free(line);
	free(start);

	return 0;
}
