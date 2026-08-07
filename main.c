#include <stdio.h>
#include <string.h>
#include <err.h>

int main(int argc, char **argv) {
    char *fileName = argv[1];
    if (fileName == NULL) {
        errx(1, "Filename must be provided.");
    }
    printf("%s\n", fileName);
    return 0;
}
