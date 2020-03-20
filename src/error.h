#ifndef ERROR_H
#define ERROR_H

typedef enum error_type {
    INCORRECT_BECOMES = 1, 
    NUMBER_MUST_FOLLOW_EQUALS, 
    EQUALS_MUST_FOLLOW_IDENTIFIER,
    IDENTIFIER_MUST_FOLLOW_CONST_VAR,
    SEMICOLON_COMMA_MISSING,
    STATEMENT_EXPECTED,
} error_type;

void error(error_type e);

#endif /* ERROR_H */
