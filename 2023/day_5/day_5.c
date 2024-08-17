#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

# define MAX_SEED 15

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

static signed int convert_integer(char* string)
{
	unsigned int number = 0;
	char *ptr = string;
	while(*ptr != '\0' && *ptr != '\n') {
		if (*ptr == ' ') {
			ptr++;
			continue;
		}
		number = number * 10 + (*ptr - 48);
		ptr++;
	}
	return number;
}

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

	Map *current = map;
	while(current != NULL) {
		unsigned int upper_bound = current->source + current->length;
		if (item >= current->source &&  item <= upper_bound) {
			unsigned int diff = item - current->source;
			result = diff + current->destination;
			break;
		}

		current = current->next;
	}

	return result;
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

	unsigned int location[MAX_SEED];
	unsigned int soil, fertilizer, water, light, temperature, humidity;
	int seed_counter = 0;

	for(int i = 0; i < MAX_SEED; i++) {
		if (seeds[i] == 0)
			break;

		soil = find_item(seeds[i], seed_to_soil);
		fertilizer = find_item(soil, soil_to_fertilizer);
		water = find_item(fertilizer, fertilizer_to_water);
		light = find_item(water, water_to_light);
		temperature = find_item(light, light_to_temperature);
		humidity = find_item(temperature, temperature_to_humidity);

		location[i] = find_item(humidity, humidity_to_location);
		seed_counter++;
	}

	unsigned int min_location = location[0];
	for(int i = 1; i < seed_counter; i++) {
		if (location[i] < min_location){
			min_location = location[i];
		}
	}

	printf("%u\n", min_location);

	free(line);

	return 0;
}
