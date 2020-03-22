#include "parser.h"
#include "symbol.h"
#include "codegen.h"

void init_parser(parser_t *parser, token_list_t *token_list) {
    parser->token_list = token_list;
    parser->token_cursor = 0;
    init_symbol_table(&(parser->symbol_table));
    init_code_generator(&(parser->code_generator));
}

token *next_token(parser_t *parser) {
    return get_token(parser->token_list, parser->token_cursor);
}

void parse_program(parser_t *parser) {
    
}

void parse_block(parser_t *parser) {

}

void parser_const_declaration(parser_t *parser) {

}

void parse_var_declaration(parser_t *parser) {

}

void parse_statement(parser_t *parser) {

}

void parse_condition(parser_t *parser) {

}

void parse_rel_op(parser_t *parser) {

}

void parse_expression(parser_t *parser) {

}

void parse_term(parser_t *parser) {

}

void parse_factor(parser_t *parser) {
    
}
