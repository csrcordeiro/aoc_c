// First answer is 3035.
// Second answer is 66027.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

static int possible_game_amount = 0;

# define MAX_RED   12
# define MAX_GREEN 13
# define MAX_BLUE  14

# define ARRAY_CAP 100


int extract_game_id(char* line)
{
    int game_number;

    sscanf(line, "Game %d:", &game_number);

    return game_number;
}

// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
void extract_colors(char* line, int* reds, int* greens, int* blues)
{
    char* str = line;

    int start_pos;
    sscanf(str, "Game %*d:%n", &start_pos);
    str += start_pos;

    char *saved;
    for (char *p = strtok_r(str, ";", &saved); p != NULL; p = strtok_r(NULL, ";", &saved)) {
	char *saved2;
	for (char *s = strtok_r(p, ",", &saved2); s != NULL; s = strtok_r(NULL, ",", &saved2)) {
	    int amount = 0;
	    char color[6];

	    sscanf(s, " %d %s", &amount, color);

	    if(strcmp(color, "blue") == 0){
		*blues = amount;
		blues++;
	    } else if(strcmp(color, "green") == 0) {
		*greens = amount;
		greens++;
	    } else if(strcmp(color, "red") == 0) {
		*reds = amount;
		reds++;
	    }
	}
    }

}

bool check_possible_color(int* color, int max)
{
    int* ptr = color;
    while(*ptr > -1) {
	if (*ptr > max)
	    return false;
	ptr++;
    }

    return true;
}

bool check_possible(int* reds, int* greens, int* blues)
{
    return check_possible_color(reds, MAX_RED) && \
	check_possible_color(greens, MAX_GREEN) && \
	check_possible_color(blues, MAX_BLUE);
}

int* create_array_for_color(int size)
{
    int colors[size];
    int* ptr = colors;

    for (int i = 0; i < size; i++) {
	colors[i] = -1;
    }

    return ptr;
}

void init_array(int* array)
{
    for (int i = 0; i < ARRAY_CAP; i++) {
	array[i] = -1;
    }
}

void process_game(char* line)
{
    int game_id = extract_game_id(line);

    int reds[ARRAY_CAP];
    init_array(reds);
    int greens[ARRAY_CAP];
    init_array(greens);
    int blues[ARRAY_CAP];
    init_array(blues);

    extract_colors(line, reds, greens, blues);

    bool possible = check_possible(reds, greens, blues);

    if(possible){
	possible_game_amount += game_id;
    }
}

int main(int argv, char **argc)
{
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, stdin) != -1) {
      process_game(line);
    }

    printf("%d\n", possible_game_amount);

    free(line);

    return 0;
}
