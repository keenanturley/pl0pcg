#ifndef SYMBOL_H
#define SYMBOL_H

#define MAX_SYMBOL_TABLE_SIZE 200

typedef enum kind_type {
    KIND_CONST = 1,
    KIND_VAR = 2
} kind_type;

typedef enum mark_type {
    MARK_IN_USE = 0,
    MARK_UNAVAILABLE = 1
} mark_type;

typedef struct symbol {
    kind_type kind;
    char name[12];
    int value;
    int level;
    int address;
    mark_type mark;
} symbol;

typedef struct symbol_table_t {
    symbol symbols[MAX_SYMBOL_TABLE_SIZE];
    int num_symbols;
    int var_address_index;
} symbol_table_t;

void init_symbol_table(symbol_table_t *table);

/**
 * @brief Create a symbol and return a copy
 * 
 * @param kind Kind of symbol
 * @param name Name of symbol
 * @param value Value of symbol
 * @param level Lexicographical level of symbol
 * @param address Address of symbol
 * @param mark Mark representing availability of symbol
 * @return symbol A copy of the created symbol
 */
symbol create_symbol(kind_type kind, char name[12], int value, int level,
    int address, mark_type mark);

/**
 * @brief Create a const symbol
 * 
 * @param name Name of the identifier
 * @param value Value of the constant
 * @return symbol The created symbol
 */
symbol create_const_symbol(char name[12], int value);

/**
 * @brief Create a var symbol
 * 
 * The level of all symbols in this assignment should be 0, because we are not 
 * implementing procedures.
 * 
 * The address is not needed when creating the symbol because it will be set 
 * on insertion into the table. The address cannot be known without access to 
 * the symbol table we want to insert into.
 * 
 * @param name Name of the identifier
 * @return symbol The created symbol
 */
symbol creat_var_symbol(char name[12]);

/**
 * @brief Copies the values from sym into the next available symbol
 * 
 * @param table Pointer to the table to insert a symbol into
 * @param sym Pointer to the symbol whose values to copy
 */
void insert_symbol(symbol_table_t *table, symbol *sym);

symbol *search_symbol(symbol_table_t *table, char name[12]);

#endif /* SYMBOL_H */
