#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "avl.h"

#define MAX(a,b) ((a>b)?(a):(b))
#define HEIGHT(a) ((a) ? (a->height) : 0)

typedef enum { HEAVY_LEFT, LEFT, SAME, RIGHT, HEAVY_RIGHT, ERROR } balance; // LEFT = left is heavier

typedef struct tree {
	struct tree *left, *right, *parent;
	int value;
	int height;
} tree;

static inline balance bal(tree* A) {
	int diff = HEIGHT(A->left) - HEIGHT(A->right);
	switch (diff) {
		case -2: return HEAVY_RIGHT;
		case -1: return RIGHT;
		case 0: return SAME;
		case 1: return LEFT;
		case 2: return HEAVY_LEFT;
		default: printf("unhandled balance: %d\n", diff); return ERROR;
	}
}

static inline char balance_character(balance bal) {
	switch (bal) {
		case HEAVY_LEFT: return '>';
		case LEFT: return '+';
		case SAME: return '=';
		case RIGHT: return '-';
		case HEAVY_RIGHT: return '<';
		case ERROR: return '!';
	}
}

static int create_node(tree** A, int value) {
	tree* _a;
	int f = 0;
	*A = malloc(sizeof(tree));
	if (!*A) {
		f = 1;
	} else {
		bzero(*A, sizeof(tree));
		_a = *A;	
		_a->height = 1;
		_a->value = value;
	}
	return f;
}

static void destroy_node(tree** A) {
	tree* _a = *A;
	if (_a) {
		free(_a);
		*A = NULL;
	}
}

static bool avl_property_ok(tree *a) {
	return !a || (bal(a) != HEAVY_LEFT && bal(a) != HEAVY_RIGHT) && avl_property_ok(a->left) && avl_property_ok(a->right) && a->height == MAX(HEIGHT(a->left), HEIGHT(a->right))+1;
}

static void dump(tree* A) {
	if (!A) printf("*");
	else {
		printf("(%d%c H %d", A->value, balance_character(bal(A)), A->height);
		if (A->left || A->right) {
			printf(" L");
			dump(A->left);
			printf(" R");
			dump(A->right);
		}
		printf(")");
	}
}

static void recalculate_height(tree *A) {
	int height = MAX(HEIGHT(A->left), HEIGHT(A->right))+1;
	if (A->height != height) {
		printf("[%d] se presouva do %d\n", A->value, height);
	}
	A->height = height;
}

static void recalculate_heights(tree* A) {
	while (A != NULL) {
		recalculate_height(A);
		A = A->parent;
	}
}

static tree* rotate_right(tree* center) {
	dump(center); printf(" =(R)=> ");
	tree *A, *B, *E;
	A = center;
	B = E = NULL;
	if (A) {
		B = A->left;
	}
	if (B) {
		E = B->right;
		B->right = A; B->parent = A->parent;
		if (B->parent) if (A == B->parent->left) B->parent->left = B; else B->parent->right = B;
	}
	A->parent = B;
	if (E) E->parent = A;
	A->left = E;
	dump(B); printf("\n");
	recalculate_heights(center);
	return B;
}

static tree* rotate_left(tree* center) {
	dump(center); printf(" =(L)=> ");
	tree *A, *C, *D;
	A = center;
	C = D =  NULL;
	if (A) {
		C = A->right;
	}
	if (C) {
		D = C->left;
		C->left = A; C->parent = A->parent;
		if (C->parent) if (A == C->parent->left) C->parent->left = C; else C->parent->right = C;
	}
	A->parent = C;
	if (D) D->parent = A;
	A->right = D;
	dump(C); printf("\n");
	recalculate_heights(center);
	return C;
}

static void dump_tree(tree* A) {
	printf("%s:\t", avl_property_ok(A) ? "OK" : "!!!");
	dump(A);
	printf("\n");
}

static void lookup_node(tree* A, int value, tree** result) {
	tree* i = A;
	*result = NULL;
	while (i) {
		if (i->value > value) {
			i = i->left;
		} else if (i->value == value) {
			*result = i;
			break;
		} else {
			i = i->right;
		}
	}
}

static void lookup(tree* A, int value, bool *result) {
	tree* tmp;
	lookup_node(A, value, &tmp);
	*result = tmp != NULL;
}


static tree* rebalance(tree *a) {
	printf("==>");
	tree* child, *next;
	do {
		printf("@%d ==> ",a->value);
		dump_tree(a);
		if (bal(a) == HEAVY_LEFT) {
			// the tree must be redone.
			child = a->left;
			if (bal(child) == RIGHT) {
				printf("LEFT-RIGHT\n");
				rotate_left(child);
				rotate_right(a);
			} else {
				printf("LEFT-LEFT\n");
				rotate_right(a);
			}
		} else if (bal(a) == HEAVY_RIGHT) {
			// the tree must be redone.
			child = a->right;
			if (bal(child) == RIGHT) {
				printf("RIGHT-RIGHT\n");
				rotate_left(a);
			} else {
				printf("RIGHT-LEFT\n");
				rotate_right(child);
				rotate_left(a);
			}
		}
		recalculate_heights(a);
		if (!a->parent) break;
		a = a->parent;
	} while (1);
	printf("Novy strom: ");
	dump_tree(a);
	return a;
}

static int delete(tree** A) {
	tree* _a = *A, *i, *tmpT, *root;
	int f = 0, tmp;
	root = *A;
	printf("mazu: "); dump_tree(_a);
	if (_a) {
		if (_a->left && !_a->right) {
			_a->value = _a->left->value;
			delete(&_a->left);
			recalculate_heights(_a);
		} else if (!_a->left && _a->right) {
			_a->value = _a->right->value;
			delete(&_a->right);
			recalculate_heights(_a);
		} else if (_a->left && _a->right) {
			i = _a->right;
			while (i->left && i->right) {
				i = i->left;
			}
			_a->value = i->value;
			printf("nahrazuji %d\n", i->value);
			tmpT = i->parent;
			delete(&i);
			printf("po smazani: ");
			dump_tree(_a);
			recalculate_heights(tmpT);
			rebalance(tmpT);
		} else { // just...
			if (_a->parent) {
				if (_a->parent->left == _a) {
					_a->parent->left = NULL;
				} else {
					_a->parent->right = NULL;
				}
				recalculate_height(_a->parent);
				rebalance(_a->parent);
			}
			destroy_node(A);
		}
	}
	return f;
}

static int insert(tree** A, int value) {
	printf("TREE += %d\n", value);
	tree* _a;
	int f = 0;
	if (!*A) {
		f = create_node(A, value);
	} else {
		_a = *A;
		while (_a) {
			if (_a->value > value && _a->left) {
				_a = _a->left;
			} else if (_a->value == value) {
				return; // no work to do here.
			} else if (_a->value < value && _a->right) {
				_a = _a->right;
			} else {
				if (_a->value > value) {
					create_node(&_a->left, value);
					_a->left->parent = _a;
					recalculate_heights(_a);
					_a = *A = rebalance(_a);
				} else {
					create_node(&_a->right, value);
					_a->right->parent = _a;
					recalculate_heights(_a);
					_a = *A = rebalance(_a);
				}
				return f;
			}
		}
	}
	return f;
}

// AVL tree public interface.
int avl_init(AVL** avl) {
	AVL* _avl;
	int f = 0;
	_avl = malloc(sizeof(AVL));
	bzero(_avl, sizeof(AVL));
	if (!_avl) {
		f = 1;
	} else {
		*avl = _avl;
	}
	return f;
}

int avl_dump(AVL* avl) {
	int f = 0;
	if (avl) {
		dump_tree(avl->root);
	} else {
		f = 1;
	}
	return f;
}

int avl_lookup(AVL* avl, int value, bool *result) {
	int f = 0;
	if (avl && result) {
		lookup(avl->root, value, result);
	} else {
		f = 1;
	}
	return f;
}

int avl_insert(AVL* avl, int value) {
	int f = 0;
	if (avl) {
		f = insert(&avl->root, value);	
	} else {
		f = 1;
	}
	return f;
}

int avl_delete(AVL* avl, int value) {
	int f = 0;
	if (avl) {
		tree* tmp, *tmp2;
		lookup_node(avl->root, value, &tmp);
		if (tmp == avl->root) {
			f = delete(&avl->root);
		} else {
			f = delete(&tmp);
		}
		if (avl->root) while (avl->root->parent) {
			avl->root = avl->root->parent;
		}
	} else {
		f = 1;
	}
	return f;
}
