#ifndef UTILS_H
#define UTILS_H

long convert_integer(char* number_as_string);

long convert_integer(char* number_as_string)
{
	long number = 0;
	char *ptr = number_as_string;

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
