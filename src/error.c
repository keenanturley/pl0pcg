#include "error.h"

#include <stdio.h>
#include <stdlib.h>

char *error_type_strings[] = { 
    "",
    "Use = instead of :=.",
    "= must be followed by a number.",
    "Identifier must be followed by =.",
    "const or var must be followed by an identifier.",
    "Semicolon or comma missing.",
    "Statement expected"
};

void error(error_type e) {
    fprintf(stderr, error_type_strings[e]);
    exit(e);
}
