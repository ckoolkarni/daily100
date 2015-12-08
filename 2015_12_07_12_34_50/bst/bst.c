#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"

struct bst_table*
bst_create(bst_comparison_func *bst_compare, void *bst_param,
		struct libavl_allocator *a)
{
	struct bst_table *tree;
	
	assert(bst_compare != NULL);

	if (a == NULL)
		a = &bst_allocator_default;
	
	tree = a->libavl_alloc(a, sizeof *tree);
	if (tree == NULL)
		return NULL;
	tree->bst_root = NULL;
	tree->bst_compare = bst_compare;
	tree->bst_param = bst_param;
	tree->bst_count = 0;
	tree->bst_generation = 0; 
	
	return tree;;
}

int main(void)
{
	return 0;
}
