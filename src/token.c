#include <string.h>
#include "token.h"

// Index into this array matches token_type enum values (map)
char *token_type_strings[] = {
    "", "nulsym", "identsym", "numbersym", "plussym", "minussym",
    "multsym", "slashsym", "oddsym", "eqsym", "neqsym", "lessym", 
    "leqsym", "gtrsym", "geqsym", "lparentsym", "rparentsym", "commasym", 
    "semicolonsym", "periodsym", "becomessym", "beginsym", "endsym", 
    "ifsym", "thensym", "whilesym", "dosym", "", "constsym", "varsym", "",
    "writesym", "readsym"
};

token_type evaluate_token_type(char *str) {
    if (strcmp(str, "null") == 0) {
        return nulsym;
    }
    if (strcmp(str, "const") == 0) {
        return constsym;
    }
    if (strcmp(str, "var") == 0) {
        return varsym;
    }
    if (strcmp(str, "begin") == 0) {
        return beginsym;
    }
    if (strcmp(str, "end") == 0) {
        return endsym;
    }
    if (strcmp(str, "if") == 0) {
        return ifsym;
    }
    if (strcmp(str, "then") == 0) {
        return thensym;
    }
    if (strcmp(str, "while") == 0) {
        return whilesym;
    }
    if (strcmp(str, "do") == 0) {
        return dosym;
    }
    if (strcmp(str, "read") == 0) {
        return readsym;
    }
    if (strcmp(str, "write") == 0) {
        return writesym;
    }
    if (strcmp(str, "odd") == 0) {
        return oddsym;
    }
    return identsym;
}

char *token_to_string(token_type token) {
    return token_type_strings[token];
}
