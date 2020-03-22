#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

/**
 * @file token_list.h
 * @author Keenan Turley (keenan.turley@knights.ucf.edu)
 * @brief Simple ArrayList implementation for PL/0 tokens
 * @date 2020-02-14
 *
 * Originally created for pl0la (Lexical Analyzier), modified for use in
 * pl0pcg (Parse + Code Generator). 
 * 
 */

#include "token.h"

extern const int DEFAULT_INITIAL_CAPACITY;
extern const int CAPACITY_MULTIPLIER;

typedef struct token_list_t {
    token *tokens; // Dynamic array of tokens
    int size;
    int capacity;
} token_list_t;

/**
 * @brief Create a list
 *
 * Dyanmically allocates and returns a list with the default initial capicity,
 * defined above.
 * The caller should call free_list() when finished with the list.
 *
 * @return token_list_t* The created list
 */
token_list_t *create_token_list();

/**
 * @brief Ensure there's enough space for another token
 *
 * Checks the given list's size against its capacity.
 *
 * If the size is less than the capacity, nothing is done.
 * If the size is greater than the capacity, an error is logged to stderr and
 * the program is exited with EXIT_FAILURE.
 * If the size is equal to the capacity, the list is reallocated with twice
 * capacity as before. (capacity *= 2)
 *
 * If the reallocation fails, an error will be logged to stderr and the
 * program will exit with EXIT_FAILURE.
 *
 * @param l The list whose capacity to ensure
 */
void ensure_capacity(token_list_t *l);

/**
 * @brief Add a token to the end of the list
 *
 * @param l The list to add to
 * @param t The token to add
 */
void add_token(token_list_t *l, token_list_t t);

/**
 * @brief Returns the token at index i in the list
 *
 * @param l The list to look in
 * @param i The index of the desired token
 * @return token* A pointer to the desired token
 */
token *get_token(token_list_t *l, int i);

/**
 * @brief Frees the list and its components
 *
 * @param l The list to free
 * @return token_list_t* Always NULL
 */
token_list_t *free_token_list(token_list_t *l);

#endif /* TOKEN_LIST_H */
