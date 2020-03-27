#include "parser.h"
#include "symbol.h"
#include "codegen.h"
#include "error.h"

#include <stdlib.h>
#include <stdbool.h>

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
    // Allocate space on the stack for FV, SL, DL, and RA
    emit_instruction(
        &(parser->code_generator),
        INC,
        0,
        0,
        4
    );

    parse_block(parser);
    if (current_token(parser)->name != periodsym) {
        error(PERIOD_EXPECTED);
    }

    // End of program instruction
    emit_instruction(
        &(parser->code_generator),
        SIO_END,
        0,
        0,
        3
    );
}

void parse_block(parser_t *parser) {
    parse_const_declaration(parser);
    parse_var_declaration(parser);
    parse_statement(parser);
}

void parse_const_declaration(parser_t *parser) {
    if (current_token(parser) == constsym) {
        do {
            // Check for identifier
            if (next_token(parser)->type != identsym) {
                error(IDENTIFIER_EXPECTED_CONST_DECLARATION);
            }
            token *identifier = current_token(parser); 

            // Check for equals sign
            if (next_token(parser)->type != eqsym) {
                error(EQUALS_EXPECTED_CONST_DECLARATION);
            }

            // Check for number
            if (next_token(parser)->type != numbersym) {
                error(NUMBER_EXPECTED_CONST_DECLARATION);
            }
            token *number = current_token(parser);

            // Identifier must not be already declared on the same level
            symbol *present = search_symbol(
                &(parser->symbol_table),
                identifier->name
            );

            if (present != NULL) {
                error(IDENTIFIER_ALREADY_DECLARED);
            }

            // Add const to symbol table
            symbol s = create_const_symbol(
                identifier->name,
                atoi(number->name)
            );
            insert_symbol(&(parser->symbol_table), &s);
        } while (next_token(parser)->type == commasym);

        // Check for declaration ending semicolon
        // Current token wasn't a comma, so it should be a semicolon
        if (current_token(parser)->type != semicolonsym) {
            error(SEMICOLON_EXPECTED_CONST_DECLARATION);
        }

        // Consume semicolon
        next_token(parser);
    }
}

void parse_var_declaration(parser_t *parser) {
    if (current_token(parser)->type == varsym) {
        int num_vars = 0;
        do {
            // Check for identifier
            if (next_token(parser)->type != identsym) {
                error(IDENTIFIER_EXPECTED_VAR_DECLARATION);
            }

            // Identifier must not be already declared on the same level
            symbol *present = search_symbol(
                &(parser->symbol_table),
                current_token(parser)->name
            );

            if (present != NULL) {
                error(IDENTIFIER_ALREADY_DECLARED);
            }

            // Create and insert var symbol
            symbol s = create_var_symbol(current_token(parser)->name);
            insert_symbol(&(parser->symbol_table), &s);

            num_vars++;
        } while (next_token(parser)->type == commasym);

        if (current_token(parser)->type != semicolonsym) {
            error(SEMICOLON_EXPECTED_VAR_DECLARATION);
        }

        if (num_vars >= 1) {
            // Allocate space on the stack for the variables
            emit_instruction(
                &(parser->code_generator),
                INC,
                0,
                0,
                num_vars
            );
        }

        // Consume semicolon
        next_token(parser);
    }
}

void parse_statement(parser_t *parser) {
    if (current_token(parser)->type == identsym) {
        // Find this variable
        symbol *s = search_symbol(
            &(parser->symbol_table), 
            current_token(parser)->name
        );

        // Symbol not in symbol table
        if (s == NULL) {
            error(UNDECLARED_IDENTIFIER);
        }

        if (s->kind != KIND_VAR) {
            error(ASSIGNMENT_TO_NON_VARIABLE);
        }

        if (next_token(parser)->type != becomessym) {
            error(BECOMES_EXPECTED_ASSIGNMENT_STATEMENT);
        }

        // Consume becomes
        next_token(parser);

        parse_expression(parser);

        // Store the result of the expression in variable's address
        emit_instruction(
            &(parser->code_generator),
            STO,
            parser->register_cursor - 1,
            s->level,
            s->address
        );

        // Decrement register cursor as we're done using that value
        (parser->register_cursor)--;
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

        // Save current code spot
        int start = parser->code_generator.code_size;
        // Emit the conditional jump instruction
        emit_instruction(
            &(parser->code_generator),
            JPC,
            0,
            0,
            0
        );

        parse_statement(parser);

        // Modify the conditional jump to jump after statement
        code_generator_t *cg = &(parser->code_generator);
        cg->code[start].modifier = cg->code_size;
    }
    else if (current_token(parser)->type == whilesym) {
        // Consume while symbol
        next_token(parser);

        // Get condition evaluation code line
        int condition = parser->code_generator.code_size;

        parse_condition(parser);

        // Get loop start line
        int loop = parser->code_generator.code_size;
        // Generate conditional jump out of loop
        emit_instruction(
            &(parser->code_generator),
            JPC,
            0,
            0,
            0   // Temporary until end of loop instruction is found
        );

        if (current_token(parser)->type != dosym) {
            error(DO_EXPECTED_WHILE_STATEMENT);
        }

        // Consume do symbol
        next_token(parser);

        parse_statement(parser);

        // Generate unconditional jump to loop condition evaluation
        emit_instruction(
            &(parser->code_generator),
            JPC,
            0,
            0,
            condition
        );

        // Modify jump line of conditional jump
        code_generator_t *cg = &(parser->code_generator);
        cg->code[loop].modifier = cg->code_size;
    }
    else if (current_token(parser)->type == readsym) {
        if (next_token(parser)->type != identsym) {
            error(IDENTIFIER_EXPECTED_READ_STATEMENT);
        }
        // Retrieve this identifier's symbol from the table
        symbol *s = search_symbol(
            &(parser->symbol_table), 
            current_token(parser)->name
        );

        if (s == NULL) {
            error(READ_INTO_INVALID_IDENTIFIER);
        }

        if (s->kind != KIND_VAR) {
            error(READ_INTO_NON_VARIABLE);
        }
        
        // Read value into next available register
        emit_instruction(
            &(parser->code_generator),
            SIO_READ,
            parser->register_cursor,
            0,
            0
        );

        // Immediately store the value into the variable's address
        emit_instruction(
            &(parser->code_generator),
            STO,
            parser->register_cursor,
            s->level,
            s->address
        );

        // Consume identifier
        next_token(parser);
    }
    else if (current_token(parser)->type == writesym) {
        if (next_token(parser)->type != identsym) {
            error(IDENTIFIER_EXPECTED_WRITE_STATEMENT);
        }
        // Retrieve this identifier's symbol from the table
        symbol *s = search_symbol(
            &(parser->symbol_table), 
            current_token(parser)->name
        );

        if (s == NULL) {
            error(WRITE_FROM_INVALID_IDENTIFIER);
        }

        if (s->kind != KIND_VAR && s->kind != KIND_CONST) {
            error(WRITE_FROM_NON_VAR_CONST_IDENTIFIER);
        }

        if (s->kind == KIND_VAR) {
            // Load the variable from its address
            emit_instruction(
                &(parser->code_generator),
                LOD,
                parser->register_cursor,
                s->level,
                s->address
            );
        }
        else { // KIND_CONST
            // Load the const from its value
            emit_instruction(
                &(parser->code_generator),
                LIT,
                parser->register_cursor,
                0,
                s->value
            );
        }

        // Emit write command from the temporary register
        emit_instruction(
            &(parser->code_generator),
            SIO_WRITE,
            parser->register_cursor,
            0,
            0
        );

        // Consume identifier
        next_token(parser);
    }
}

void parse_condition(parser_t *parser) {
    // EBNF: "odd" expression
    if (current_token(parser)->type == oddsym) {
        // Consume odd symbol
        next_token(parser);

        parse_expression(parser);

        emit_instruction(
            &(parser->code_generator),
            ODD,
            parser->register_cursor - 1,
            0,
            0
        );
    } else { // EBNF: expression rel-op expression
        parse_expression(parser);

        token_type rel_op = current_token(parser);
        parse_rel_op(parser);

        parse_expression(parser);

        // Evaluate the condition based on expressions and operator
        opcode op;
        switch(rel_op) {
            case eqsym:
                op = EQL;
                break;
            case neqsym:
                op = NEQ;
                break;
            case lessym:
                op = LSS;
                break;
            case leqsym:
                op = LEQ;
                break;
            case gtrsym:
                op = GTR;
                break;
            case geqsym:
                op = GEQ;
                break;
        }

        emit_instruction(
            &(parser->code_generator),
            op,
            parser->register_cursor - 2,
            parser->register_cursor - 2,
            parser->register_cursor - 1
        );
        // Decrement register cursor, operation squashes 2 values into 1
        parser->register_cursor--;
    }
}

void parse_rel_op(parser_t *parser) {
    switch (current_token(parser)->type) {
        case eqsym:
        case neqsym:
        case lessym:
        case leqsym:
        case gtrsym:
        case geqsym:
            break;
        default:
            error(REL_OP_EXPECTED);
    }
    
    // Consume rel-op symbol
    next_token(parser);
}

void parse_expression(parser_t *parser) {
    bool will_negate = false;
    if (current_token(parser)->type == plussym) {
        // Consume plus
        next_token(parser);
    }
    else if (current_token(parser)->type == minussym) {
        will_negate = true;
        // Consume minus
        next_token(parser);
    }

    parse_term(parser);

    // Negate term
    if (will_negate) {
        emit_instruction(
            &(parser->code_generator),
            NEG,
            parser->register_cursor - 1,
            0,
            0
        );
    }

    while (current_token(parser)->type == plussym || 
        current_token(parser)->type == minussym) {
        token_type operator = current_token(parser)->type;

        // Consume plus or minus
        next_token(parser);

        parse_term(parser);

        // Evaluate previous and current factor using current operator
        if (operator == plussym) {
            emit_instruction(
                &(parser->code_generator),
                ADD,
                parser->register_cursor - 2,
                parser->register_cursor - 2,
                parser->register_cursor - 1
            );
        } else { // minussym
            emit_instruction(
                &(parser->code_generator),
                SUB,
                parser->register_cursor - 2,
                parser->register_cursor - 2,
                parser->register_cursor - 1
            );
        }
        // Decrement register cursor, operation squashes 2 values into 1
        parser->register_cursor--;
    }
}

void parse_term(parser_t *parser) {
    parse_factor(parser);

    while (current_token(parser)->type == multsym ||
        current_token(parser)->type == slashsym) {
        token_type operator = current_token(parser)->type;

        // Consume multiply or divide
        next_token(parser);

        parse_factor(parser);

        // Evaluate previous and current factor using current operator
        if (operator == multsym) {
            emit_instruction(
                &(parser->code_generator),
                MUL,
                parser->register_cursor - 2,
                parser->register_cursor - 2,
                parser->register_cursor - 1
            );
        } else { // slashsym
            emit_instruction(
                &(parser->code_generator),
                DIV,
                parser->register_cursor - 2,
                parser->register_cursor - 2,
                parser->register_cursor - 1
            );
        }
        // Decrement register cursor, operation squashes 2 values into 1
        parser->register_cursor--;
    }
}

void parse_factor(parser_t *parser) {
    // EBNF: ident
    if (current_token(parser)->type == identsym) {
        symbol *s = search_symbol(
            &(parser->symbol_table), 
            current_token(parser)->name
        );

        if (s == NULL) {
            error(UNDECLARED_IDENTIFIER);
        }

        // Load variable
        if (s->kind == KIND_VAR) {
            emit_instruction(
                &(parser->code_generator),
                LOD,
                (parser->register_cursor)++,
                s->level,
                s->address
            );
        } 
        // Load literal constant
        else if (s->kind == KIND_CONST) {
            emit_instruction(
                &(parser->code_generator),
                LIT,
                (parser->register_cursor)++,
                0,
                s->value
            );
        }
        else {
            error(NON_VAR_CONST_IDENTIFIER_FACTOR);
        }
    } 
    // EBNF: number
    else if (current_token(parser)->type == numbersym) {
        emit_instruction(
            &(parser->code_generator),
            LIT,
            (parser->register_cursor)++,
            0,
            atoi(current_token(parser)->name)
        );
    }
    // EBNF: "(" expression ")"
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
