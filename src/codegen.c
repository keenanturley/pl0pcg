#include "codegen.h"
#include "error.h"

void init_code_generator(code_generator_t *generator) {
    generator->code_size = 0;
}

void emit_instruction(code_generator_t *generator, opcode op, int r, int l, 
    int m) {
    // Throw an error and exit if we've went over our maximum code length
    if (generator->code_size == MAX_CODE_LENGTH) error(25);

    // Otherwise, put this instruction in the code generator
    cg_instruction *i = &(generator->code[generator->code_size]);
    i->op = op;
    i->regiser_num = r;
    i->lex_level = l;
    i->modifier = m;

    (generator->code_size)++;
}
