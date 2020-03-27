#include "error.h"

#include <stdio.h>
#include <stdlib.h>

char *error_type_strings[] = { 
    "",
    "Period expected.",
    "Identifier expected in const declaration.",
    "Equals sign must follow identifier in const declaration.",
    "Number must follow equals sign in const declaration.",
    "Semicolon expected at end of const declaration.",
    "Identifier expected in var declaration.",
    "Semicolon expected at end of var declaration.",
    "Becomes (:=) expected after identifier in statement.",
    "End keyword expected following begin statement.",
    "Then keyword expected following if statement.",
    "Do keyword expected following while statement.",
    "Identifier expected after keyword read in read statement.",
    "Identifier expected after keyword write in write statement.",
    "Relational operator (=, <>, <, <=, >, >=) expected.",
    "Invalid expression. Expected identifier, number, or expression in parentheses.",
    "Right parenthesis expected to close currently opened expression factor."
};

void error(error_type e) {
    fprintf(stderr, "Error: %s\n", error_type_strings[e]);
    exit(e);
}
