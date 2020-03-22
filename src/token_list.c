#include <stdlib.h>
#include <stdio.h>
#include "token_list.h"
#include "token.h"

// Initial capacity of list if none was specified
const int DEFAULT_INITIAL_CAPACITY = 8;
// Multiplier to use when resizing list
const int CAPACITY_MULTIPLIER = 2;

token_list_t *create_token_list() {
    token_list_t * l = (token_list_t *)malloc(sizeof(token_list_t));
    l->capacity = DEFAULT_INITIAL_CAPACITY;
    l->size = 0;
    l->tokens = (token *)malloc(sizeof(token) * l->capacity);

    return l;
}

void ensure_capacity(token_list_t *l) {
    if (l->size < l->capacity) return;
    if (l->size > l->capacity) {
        fprintf(stderr, "ERROR: List capacity overflow\n");
        fprintf(stderr, "   size: %d, capacity: %d\n", l->size, l->capacity);
        exit(EXIT_FAILURE);
    }

    l->capacity *= CAPACITY_MULTIPLIER;
    l->tokens = (token *)realloc(l->tokens, sizeof(token) * l->capacity);

    if (l->tokens == NULL) {
        fprintf(stderr, "ERROR: List reallocation failed\n");
        exit(EXIT_FAILURE);
    }
}

void add(token_list_t *l, token t) {
    ensure_capacity(l);

    l->tokens[l->size] = t;
    l->size++;
}

token *get(token_list_t *l, int i) {
    if (i < 0 || i >= l->size) return NULL; // Invalid index
    return &(l->tokens[i]);
}

token_list_t *free_token_list(token_list_t *l) {
    // Free the string inside each token
    for (int i = 0; i < l->size; i++) {
        token *t = get(l, i);
        free(t->name);
    }
    // Free the tokens array
    free(l->tokens);
    // Free the list itself
    free(l);
    return NULL;
}
