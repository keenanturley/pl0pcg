#include "symbol.h"

#include <string.h>

void init_symbol_table(symbol_table_t *table) {
    table->num_symbols = 0;
    table->var_address_index = 4;
}

symbol create_symbol(kind_type kind, char name[12], int value, int level,
    int address, mark_type mark) {
    symbol s = {
        kind,
        name,
        value,
        level,
        address,
        mark
    };
    return s;
}

symbol create_const_symbol(char name[12], int value) {
    symbol s = {
        KIND_CONST,     // Kind
        name,           // Name
        value,          // Value
        0,              // Level 
        0,              // Address
        0               // Mark
    };
    return s;
}

symbol creat_var_symbol(char name[12]) {
    symbol s = {
        KIND_VAR,       // Kind
        name,           // Name
        0,              // Value
        0,              // Level 
        0,              // Address
        0               // Mark
    };
    return s;
}

void insert_symbol(symbol_table_t *table, symbol *sym) {
    // Grab the address of the destination symbol from the table
    symbol *s = &(table->symbols[table->num_symbols]);

    if (s->kind == KIND_VAR)
        sym->address = (table->var_address_index)++;

    // Copy the contents of the passed in symbol to the destination
    memcpy(s, sym, sizeof(symbol));

    (table->num_symbols)++;
}
