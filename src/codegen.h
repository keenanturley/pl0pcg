#ifndef CODEGEN_H
#define CODEGEN_H

#define MAX_CODE_LENGTH 200

typedef enum opcode {
    LIT = 1, RTN, LOD, STO, CAL, INC, JMP, JPC, SIO_WRITE,
    SIO_READ, SIO_END, NEG, ADD, SUB, MUL, DIV, ODD,
    MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
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
 * @brief Initialize the given code generator object
 * 
 * @param generator The generator to initialize
 */
void init_code_generator(code_generator_t *generator);

/**
 * @brief Create a instruction object
 * 
 * @param op Opcode of instruction
 * @param r Register number
 * @param l Lexicographical level
 * @param m Modifier
 * @return cg_instruction The resulting instruction
 */
cg_instruction create_instruction(opcode op, int r, int l, int m);

/**
 * @brief Insert the given instruction into the code_generator_t
 * 
 * @param generator Pinter to the generator to instert into
 * @param op Opcode of instruction
 * @param r Register number
 * @param l Lexicographical level
 * @param m Modifier
 */
void emit_instruction(code_generator_t *generator, opcode op, int r, int l, 
    int m);

/**
 * @brief Insert the given instruction into the code generator
 * 
 * Uses emit_instruction with the given instructions values as arguments
 * 
 * @param generator Generator to insert instruction into
 * @param i Instruction to insert
 */
void emit_prepared_instruction(code_generator_t *generator, cg_instruction *i);

#endif /* CODEGEN_H */
