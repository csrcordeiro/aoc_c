#ifndef UTILS_H
#define UTILS_H

#include<string.h>

long convert_integer(char* number_as_string);

long convert_integer(char* number_as_string)
{
	char *ptr = number_as_string;

	// If it's a single digit.
	if (strlen(ptr) == 2)
		return *ptr - 48;

	long number = 0;

	while(*ptr != '\0' && *ptr != '\n') {
		if (*ptr == ' ') {
			ptr++;
			continue;
		}
		// Using ASCII arithmetics.
		number = number * 10 + (*ptr - 48);
		ptr++;
	}

	return number;
}

#endif /* UTILS_H */
