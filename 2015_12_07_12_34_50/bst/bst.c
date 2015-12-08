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

void*
bst_find(const struct bst_table *t, void *item)
{
	struct bst_node *p;
	int dir;
	int cmp;

	assert(t != NULL && item != NULL);
	for (p = t->bst_root; p != NULL; p = p->bst_link[dir]) {
		cmp = t->bst_compare(item, p->bst_data, t->bst_param);	
		if (cmp == 0)
			return p->bst_data;
		dir = cmp > 0;
	}
	return NULL;	
}

int main(void)
{
	return 0;
}
