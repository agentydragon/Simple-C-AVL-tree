#ifndef AVL_H_INCLUDED
#define AVL_H_INCLUDED

#include <stdbool.h>

typedef struct AVL {
	struct tree* root;
} AVL;

int avl_init(AVL** avl);
int avl_dump(AVL* avl);
int avl_insert(AVL* avl, int value);
int avl_lookup(AVL* avl, int value, bool *result);
int avl_delete(AVL* avl, int value);
int avl_destroy(AVL** avl);

#endif
