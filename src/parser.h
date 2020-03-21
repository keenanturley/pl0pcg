#ifndef PARSER_H
#define PARSER_H

#include "symbol.h"
#include "codegen.h"

typedef struct parser {
    symbol_table_t symbol_table;
    code_generator_t code_generator;
} parser;

#endif /* PARSER_H */
