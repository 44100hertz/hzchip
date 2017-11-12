#include <stdio.h>
#include <stdlib.h>

#include "file.h"

const char *file_to_string(const char *filename)
{
	// count lines
	FILE *f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	rewind(f);

	char *out = malloc(size+1);
	out[size] = '\0';
	fread(out, 1, size, f);
	fclose(f);

	return out;
}
