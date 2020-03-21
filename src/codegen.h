#ifndef CODEGEN_H
#define CODEGEN_H

#define MAX_CODE_LENGTH 200

typedef enum opcode {
    LIT = 1, RTN, LOD, STO, CAL, INC, JMP, JPC, SIO_WRITE,
    SIO_READ, SIO_END, NEG, ADD, SUB, MUL, DIV, ODD,
    MOD, EQL, NEQ, LSS
} opcode;

typedef struct cg_instruction {
    opcode op;
    int regiser_num;
    int lex_level;
    int modifier;
} cg_instruction;

typedef struct code_generator_t {
    cg_instruction code[MAX_CODE_LENGTH];
    int code_size;
} code_generator_t;

/**
 * @brief Insert the given instruction into the code_generator_t
 * 
 * @param generator A pointer to the generator to instert into
 * @param op The opcode of the instruction
 * @param r The register number to operate with
 * @param l The lexicographical level
 * @param m The modifier
 */
void emit_instruction(code_generator_t *generator, opcode op, int r, int l, 
    int m);

#endif /* CODEGEN_H */
