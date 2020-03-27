#ifndef PARSER_H
#define PARSER_H

#include "symbol.h"
#include "codegen.h"
#include "token_list.h"

typedef struct parser_t {
    token_list_t *token_list;
    int token_cursor;
    symbol_table_t symbol_table;
    int register_cursor;
    code_generator_t code_generator;
} parser_t;

/**
 * @brief Initialize a parser with the given token list
 * 
 * @param token_list The list of tokens to use in the parser
 */
void init_parser(parser_t *parser, token_list_t *token_list);

/**
 * @brief Adds an instruction to the code generator
 * 
 * Effectively an alias to
 * emit_prepared_instruction(&(parser->code_generator), i);
 * 
 * @param parser Parser whose code generator to use
 * @param i Pointer to instruction to add
 */
void add_code(parser_t *parser, cg_instruction *i);

/**
 * @brief Returns the token at the token_cursor index
 * 
 * Equivalent pseudocode:
 *     return token_list[token_cursor];
 * 
 * @param parser The parser to read the token from
 * @return token* Pointer to the current token
 */
token *current_token(parser_t *parser);

/**
 * @brief Advances the cursor to and returns the next token in the token list
 * 
 * This function returns the equivalent of token_list[++token_cursor].
 * Therefore, after using this function, token_cursor will be the index 
 * of the returned token.
 * 
 * @param parser The parser to read the token from
 * @return token* Pointer to the next token
 */
token *next_token(parser_t *parser);

/**
 * @brief Parse a program
 * 
 * EBNF:
 * program ::= block ".".
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_program(parser_t *parser);

/**
 * @brief Parse a block
 * 
 * EBNF:
 * block ::= const-declaration var-declaration statement.
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_block(parser_t *parser);

/**
 * @brief Parse a const-declaration
 * 
 * EBNF:
 * const-declaration ::= ["const" ident "=" number {"," ident "=" number} ";"].
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_const_declaration(parser_t *parser);

/**
 * @brief Parse a var-declaration
 * 
 * EBNF:
 * var-declaration ::= ["var" ident {"," ident} ";"].
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_var_declaration(parser_t *parser);

/**
 * @brief Parse a statement
 * 
 * EBNF:
 * statement ::= [ident ":=" expression
 *                | "begin" statement {";" statement} "end"
 *                | "if" condition "then" statement
 *                | "while" condition "do" statement 
 *                | "read" ident 
 *                | "write" ident 
 *                | e].
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_statement(parser_t *parser);

/**
 * @brief Parse a condition
 * 
 * EBNF:
 * condition ::= "odd" expression
 *               | expression rel-op expression.
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_condition(parser_t *parser);

/**
 * @brief Parse a rel-op
 * 
 * EBNF:
 * rel-op ::= "=" | "<>" | "<" | "<=" | ">" | ">=".
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_rel_op(parser_t *parser);

/**
 * @brief Parse an expression
 * 
 * EBNF:
 * expression ::= ["+" | "-"] term {("+" | "-") term}.
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_expression(parser_t *parser);

/**
 * @brief Parse a term
 * 
 * EBNF:
 * term ::= factor {("*" | "/") factor}.
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_term(parser_t *parser);

/**
 * @brief Parse a factor
 * 
 * EBNF:
 * factor ::= ident | number | "(" expression ")".
 * 
 * @param parser Pointer to the parser containing the tokens to parse
 */
void parse_factor(parser_t *parser);

#endif /* PARSER_H */
