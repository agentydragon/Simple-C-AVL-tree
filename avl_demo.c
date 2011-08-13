#include <stdio.h>
#include "avl.h"

int main(void) {
	AVL* avl;
	int i, y;
	int test[] = {50,30,10,11,12,13,40,90,70,80,20,60,0}; // zero-terminated
	bool flag;
	if (avl_init(&avl) == 0) {
		for (i = 0; test[i]; i++) {
			avl_insert(avl, test[i]);
			printf("+%d\n", test[i]);
			y = i;
		}
		for (i = y; i >= 0; i--) {
			avl_delete(avl, test[i]);
			printf("-%d\n", test[i]);
			for (y = 0; test[y]; y++) {
				avl_lookup(avl, test[y], &flag);
				printf("%d: %c\n", test[y], flag ? '+' : '-');
			}
			avl_dump(avl);
		}
	} else {
		printf("avl_init failed\n");
	}
}
