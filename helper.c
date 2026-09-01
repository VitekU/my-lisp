#include "helper.h"
#include <stdio.h>
#include <err.h>

FILE* create_file_ptr(const char *name) {
    FILE *fp = NULL;
	if (name == NULL) {
		errx(1, "Filename must be provided.");
	}

	if ((fp = fopen(name, "r")) == NULL) {
		err(1, "Error opening file: Failed to open '%s' ", name);
	}
	return fp;
}
