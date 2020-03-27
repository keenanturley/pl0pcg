#include "parser.h"
#include "symbol.h"
#include "codegen.h"
#include "error.h"

void init_parser(parser_t *parser, token_list_t *token_list) {
    parser->token_list = token_list;
    parser->token_cursor = 0;
    init_symbol_table(&(parser->symbol_table));
    parser->register_cursor = 0;
    init_code_generator(&(parser->code_generator));
}

void add_code(parser_t *parser, cg_instruction *i) {
    emit_prepared_instruction(&(parser->code_generator), i);
}

token *current_token(parser_t *parser) {
    return get_token(parser->token_list, parser->token_cursor);
}

token *next_token(parser_t *parser) {
    return get_token(parser->token_list, ++(parser->token_cursor));
}

void parse_program(parser_t *parser) {
    parse_block(parser);
    if (current_token(parser)->name != periodsym) {
        error(PERIOD_EXPECTED);
    }
}

void parse_block(parser_t *parser) {
    parse_const_declaration(parser);
    parse_var_declaration(parser);
    parse_statement(parser);
    // TODO: CODEGEN: end of program
}

void parse_const_declaration(parser_t *parser) {
    if (current_token(parser) == constsym) {
        do {
            // Check for identifier
            if (next_token(parser)->type != identsym) {
                error(IDENTIFIER_EXPECTED_CONST_DECLARATION);
            }

            // Check for equals sign
            if (next_token(parser)->type != eqsym) {
                error(EQUALS_EXPECTED_CONST_DECLARATION);
            }

            // Check for number
            if (next_token(parser)->type != numbersym) {
                error(NUMBER_EXPECTED_CONST_DECLARATION);
            }
        } while (next_token(parser)->type == commasym);

        // Check for declaration ending semicolon
        // Current token wasn't a comma, so it should be a semicolo
        if (current_token(parser)->type != semicolonsym) {
            error(SEMICOLON_EXPECTED_CONST_DECLARATION);
        }

        // Consume semicolon
        next_token(parser);
    }
}

void parse_var_declaration(parser_t *parser) {
    if (current_token(parser)->type == varsym) {
        do {
            // Check for identifier
            if (next_token(parser)->type != identsym) {
                error(IDENTIFIER_EXPECTED_VAR_DECLARATION);
            }
        } while (next_token(parser)->type == commasym);

        if (current_token(parser)->type != semicolonsym) {
            error(SEMICOLON_EXPECTED_VAR_DECLARATION);
        }

        // Consume semicolon
        next_token(parser);
    }
}

void parse_statement(parser_t *parser) {
    if (current_token(parser)->type == identsym) {
        if (next_token(parser)->type != becomessym) {
            error(BECOMES_EXPECTED_ASSIGNMENT_STATEMENT);
        }

        // Consume becomes
        next_token(parser);

        parse_expression(parser);
    } 
    else if (current_token(parser)->type == beginsym) {
        // Consume begin
        next_token(parser);

        do {
            parse_statement(parser);
        } while (next_token(parser)->type == semicolonsym);

        if (current_token(parser)->type != endsym) {
            error(END_EXPECTED_BEGIN_STATEMENT);
        }

        // Consume end 
        next_token(parser);
    }
    else if (current_token(parser)->type == ifsym) {
        // Consume if symbol
        next_token(parser);

        parse_condition(parser);

        if (current_token(parser)->type != thensym) {
            error(THEN_EXPECTED_IF_STATEMENT);
        }

        // Consume then symbol
        next_token(parser);

        parse_statement(parser);
    }
    else if (current_token(parser)->type == whilesym) {
        // Consume while symbol
        next_token(parser);

        parse_condition(parser);

        if (current_token(parser)->type != dosym) {
            error(DO_EXPECTED_WHILE_STATEMENT);
        }

        // Consume do symbol
        next_token(parser);

        parse_statement(parser);
    }
    else if (current_token(parser)->type == readsym) {
        if (next_token(parser)->type != identsym) {
            error(IDENTIFIER_EXPECTED_READ_STATEMENT);
        }
        // TODO: CODEGEN: Read identifier

        // Consume identifier
        next_token(parser);
    }
    else if (current_token(parser)->type == writesym) {
        if (next_token(parser)->type != identsym) {
            error(IDENTIFIER_EXPECTED_WRITE_STATEMENT);
        }
        // TODO: CODEGEN: Write identifier

        // Consume identifier
        next_token(parser);
    }
}

void parse_condition(parser_t *parser) {
    if (current_token(parser)->type == oddsym) {
        // Consume odd symbol
        next_token(parser);

        parse_expression(parser);
    } else {
        parse_expression(parser);
        parse_rel_op(parser);
        parse_expression(parser);
    }
}

void parse_rel_op(parser_t *parser) {
    switch (current_token(parser)->type) {
        // TODO: CODEGEN: Correct operation
        case eqsym:
        case neqsym:
        case lessym:
        case leqsym:
        case gtrsym:
        case geqsym:
        default:
            error(REL_OP_EXPECTED);
    }
    
    // Consume rel-op symbol
    next_token(parser);
}

void parse_expression(parser_t *parser) {
    if (current_token(parser)->type == plussym) {
        // TODO: CODEGEN: ?

        // Consume plus
        next_token(parser);
    }
    else if (current_token(parser)->type == minussym) {
        // TODO: CODEGEN: ?

        // Consume minus
        next_token(parser);
    }

    parse_term(parser);

    while (current_token(parser)->type == plussym || 
        current_token(parser)->type == minussym) {
        // Consume plus or minus
        next_token(parser);

        parse_term(parser);
    }
}

void parse_term(parser_t *parser) {
    parse_factor(parser);

    while (current_token(parser)->type == multsym ||
        current_token(parser)->type == slashsym) {
        // Consume multiply or divide
        next_token(parser);

        parse_factor(parser);
    }
}

void parse_factor(parser_t *parser) {
    if (current_token(parser)->type == identsym) {
        // TODO: CODEGEN: ?
    } 
    else if (current_token(parser)->type == numbersym) {
        // TODO: CODEGEN: ?
    }
    else if (current_token(parser)->type == lparentsym) {
        // Consume left parenthesis
        next_token(parser);

        parse_expression(parser);

        if (current_token(parser)->type != rparentsym) {
            error(RIGHT_PARENTHESIS_EXPECTED_FACTOR);
        }

        // Consume right parenthesis
        next_token(parser);
    }
    else {
        error(INVALID_EXPRESSION);
    }
}
