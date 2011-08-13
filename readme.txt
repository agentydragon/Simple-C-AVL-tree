README:

This package contains a simple C implementation of an AVL tree. Its public
interface contains the following methods (all of them give a non-zero return
value on error):
	- int avl_init(AVL**); - initializes an empty AVL tree.
	- int avl_dump(AVL*); - dumps the contents of the AVL tree in stdout.
	- int avl_insert(AVL*, int value); - inserts a new value into the given AVL tree.
	- int avl_lookup(AVL*, int value, bool *result); - tries to find the given
		value in the tree and returns true or false in its result argument based
		on its success.
	- int avl_destroy(AVL**); - destroys an AVL tree.

BUGS:

- avl_destroy doesn't delete and destroy the tree contents, just the AVL
	structure (too lazy to fix right now...)

LICENCE:

GPL >=3. And please let me know if you like this piece of code.

AUTHOR:

Michal "prvak" Pokorny, mp@decin.cz
