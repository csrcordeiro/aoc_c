// First answers 57075758.
// Second answers 31161857.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utils/utils.h"

# define MAX_SEED 20
# define INFINITY 4294967295

typedef struct _map {
	unsigned int source;
	unsigned int destination;
	unsigned int length;
	struct _map *next;
} Map;

static Map *seed_to_soil;
static Map *soil_to_fertilizer;
static Map *fertilizer_to_water;
static Map *water_to_light;
static Map *light_to_temperature;
static Map *temperature_to_humidity;
static Map *humidity_to_location;

static void initialize_numbers(int* numbers, int size)
{
	for(int i = 0; i < size; i++) {
		numbers[i] = 0;
	}
}

static void collect_seeds(char* line, int* seeds)
{
	char *ptr = (char*)malloc(strlen(line)+1);
	strcpy(ptr, line);

	int saved;
	sscanf(ptr, "seeds:%*[ ]%n", &saved);
	ptr+=saved;

	initialize_numbers(seeds, MAX_SEED);

	char* saved_ptr;
	for (char *numbs = strtok_r(ptr, " ", &saved_ptr); numbs != NULL; numbs = strtok_r(NULL, " ", &saved_ptr)) {
		if (*numbs != '\n' && *numbs != '\0' && *numbs != ' ') {
			*seeds = convert_integer(numbs);
			seeds++;
		}
	}

}

static void insert_entry(Map** head, Map* entry)
{
	if (*head == NULL) {
		*head = entry;
	} else {
		Map *current = *head;
		while(current->next != NULL) {
			current = current->next;
		}

		current->next = entry;
	}
}

static void process_map(char** line, size_t* len, FILE* file, Map** map_head)
{
	if (**line == '\n')
		getline(line, len, file);

	while (getline(line, len, file) != -1 && **line != '\n') {
		// Skip map header
		if (isalnum(**line) == 0) {
			continue;
		}

		Map *new_map = malloc(sizeof(Map));

		sscanf(*line, "%u%*[ ]%u%*[ ]%u", &(new_map->destination), &(new_map->source), &(new_map->length));

		insert_entry(map_head, new_map);

	}
}

static void print_map(Map* first_map)
{
	Map *ptr = first_map;
	while(ptr != NULL) {
		printf("Source: %u Destination: %u Length: %u\n", ptr->source, ptr->destination, ptr->length);
		ptr = ptr->next;
	}
}

static unsigned int find_item(unsigned int item, Map* map)
{
	unsigned int result = item;
	unsigned int diff, upper_bound = 0;

	Map *current = map;
	while(current != NULL) {
		upper_bound = current->source + current->length;
		if (item >= current->source &&  item <= upper_bound) {
			diff = item - current->source;
			result = diff + current->destination;
			break;
		}
		current = current->next;
	}

	return result;
}

static unsigned int shortest_location(unsigned int seed)
{
	unsigned int soil, fertilizer, water, light, temperature, humidity;

	soil = find_item(seed, seed_to_soil);
	fertilizer = find_item(soil, soil_to_fertilizer);
	water = find_item(fertilizer, fertilizer_to_water);
	light = find_item(water, water_to_light);
	temperature = find_item(light, light_to_temperature);
	humidity = find_item(temperature, temperature_to_humidity);

	return find_item(humidity, humidity_to_location);
}

static int in_between(unsigned int number, unsigned int start, unsigned int end)
{
	if (number > start && number < end) {
		return 1;
	}

	return 0;
}

/*
 * This function will discard numbers from the input
 * that were already searched for in previous ranges.
 */
static void discover_range_to_search(
		unsigned int seed_range_start,
		unsigned int seed_range_end,
		unsigned int* new_start,
		unsigned int* new_end,
		unsigned int* searched_ranges
) {

	*new_start = seed_range_start;
	*new_end = seed_range_end;

	unsigned int searched_start, searched_end;
	for(int i = 0; i < MAX_SEED; i++) {
		if (searched_ranges[i] == 0) {
			searched_ranges[i] = *new_start;
			searched_ranges[i+1] = *new_end;
			break;
		}

		searched_start = searched_ranges[i];
		searched_end = searched_ranges[i+1];

		if (in_between(seed_range_start, searched_start, searched_end) == 1 && \
				in_between(seed_range_end, searched_start, searched_end) == 1) {
			*new_start = 0;
			*new_end = 0;
			break;
		} else if (in_between(seed_range_start, searched_start, searched_end) == 1) {
			*new_start = searched_end + 1;
			break;
		} else if (in_between(seed_range_end, searched_start, searched_end) == 1) {
			*new_end = seed_range_start - 1;
			break;
		}
	}
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	FILE *file = stdin;

	unsigned int seeds[MAX_SEED];

	getline(&line, &len, file);
	collect_seeds(line, seeds);
	getline(&line, &len, file);

	process_map(&line, &len, file, &seed_to_soil);
	process_map(&line, &len, file, &soil_to_fertilizer);
	process_map(&line, &len, file, &fertilizer_to_water);
	process_map(&line, &len, file, &water_to_light);
	process_map(&line, &len, file, &light_to_temperature);
	process_map(&line, &len, file, &temperature_to_humidity);
	process_map(&line, &len, file, &humidity_to_location);

	unsigned int location, min_location, seed_range_end, seed_range_start;
	unsigned int searched_ranges[MAX_SEED];

	min_location = INFINITY;

	initialize_numbers(searched_ranges, MAX_SEED);

	for(int i = 0; i < MAX_SEED; i+=2) {
		if (seeds[i] == 0)
			break;

		seed_range_start = seeds[i];
		seed_range_end = seed_range_start + (seeds[i+1] - 1);

		unsigned int search_start, search_end;

		discover_range_to_search(
				seed_range_start,
				seed_range_end,
				&search_start,
				&search_end,
				searched_ranges
		);

		if (search_start == 0 && search_end == 0) {
			continue;
		}

		for (int j = 0; (search_start + j) < search_end; j++) {
			location = shortest_location(search_start + j);

			if (location < min_location || min_location == 0)
				min_location = location;
		}

	}

	printf("%u\n", min_location);

	free(line);

	return 0;
}
