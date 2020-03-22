#ifndef TOKEN_H
#define TOKEN_H

/* Token Types */
typedef enum token_type { nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, constsym = 28, varsym, writesym = 31,
    readsym
} token_type;

/* Token structure */
typedef struct token {
    char *name;
    token_type type;
} token;

/**
 * @brief Returns token_type of string
 * 
 * If the given string is not a reserved word token, it will be considered
 * an identifier
 * 
 * @param str The string representation of the token
 * @return token_type The token type of the string
 */
token_type evaluate_token_type(char *str);

/**
 * @brief Returns string representation of given token
 * 
 * @param token A token type to stringify
 * @return char* String representing the given token
 */
char *token_to_string(token_type token);

#endif /* TOKEN_H */
