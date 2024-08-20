#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/utils.h"

#define MAX_RACES 4

typedef struct _race {
	long time;
	long distance;
	int combinations;
	struct _race *next;
} Race;

static Race *head;

static void insert_race(Race *race)
{
	if (head == NULL) {
		head = race;
	} else {
		Race *current = head;
		while(current->next != NULL) {
			current = current->next;
		}

		current->next = race;
	}
}

static void print_races(void)
{
	Race *current = head;
	while(current != NULL) {
		printf("Time: %ld Distance: %ld Combinations: %d\n", current->time, current->distance, current->combinations);
		current = current->next;
	}
}

static void extract_numbers(long *dest, char *line, char *prefix)
{
	int saved;

	sscanf(line, prefix, &saved);

	line += saved;

	for(char *tok = strtok(line, " "); tok != NULL; tok = strtok(NULL, " ")) {
		*dest = convert_integer(tok);
		dest++;
	}
}

static void initialize_numbers(long *numbers, int size)
{
	for(int i = 0; i < size; numbers[i++] = -1);
}

static int calculate_combinations(long time, long distance)
{
	long distance_record = distance + 1;
	long velocity = 0;
	int combinations = 0;

	for(long i = 1; i < time; i++) {
		velocity = (time - i) * i;
		if (velocity >= distance_record)
			combinations++;
	}

	return combinations;
}

static void process_races(char *time, char *distance)
{
	long races[MAX_RACES], distances[MAX_RACES];

	initialize_numbers(races, MAX_RACES);
	extract_numbers(races, time, "Time:%n");

	initialize_numbers(distances, MAX_RACES);
	extract_numbers(distances, distance, "Distance:%n");

	for(int i = 0; i < MAX_RACES; i++) {
		long int_race = races[i];
		long int_dist = distances[i];

		if (int_race == -1 || int_dist == -1) {
			break;
		}

		Race *race = malloc(sizeof(Race));
		race->time = int_race;
		race->distance = int_dist;
		race->combinations = calculate_combinations(int_race, int_dist);

		insert_race(race);
	}
}

static long calculate_total_combinations(void)
{
	long total_combinations = 1;
	Race *current = head;
	while(current != NULL) {
		total_combinations *= current->combinations;

		current = current->next;
	}

	return total_combinations;
}

int main(int argc, char **argv)
{

	char *line = NULL;
	size_t len = 0;

	char *time, *distance;

	getline(&line, &len, stdin);
	time = malloc(strlen(line)+1);
	time = strcpy(time, line);

	getline(&line, &len, stdin);
	distance = malloc(strlen(line)+1);
	distance = strcpy(distance, line);

	process_races(time, distance);

	printf("%ld\n", calculate_total_combinations());

	free(line);
	free(time);
	free(distance);

	return 0;
}
