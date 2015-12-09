#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"

struct bst_table*
bst_create(bst_comparison_func *bst_compare, void *bst_param,
		struct libavl_allocator *a)
{
	struct bst_table *t;
	
	assert(bst_compare != NULL);

	if (a == NULL)
		a = &bst_allocator_default;
	
	t = a->libavl_alloc(a, sizeof *t);
	if (t == NULL)
		return NULL;
	t->bst_root = NULL;
	t->bst_compare = bst_compare;
	t->bst_param = bst_param;
	t->bst_count = 0;
	t->bst_generation = 0; 
	
	return t;
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

void** 
bst_probe(struct bst_table *t, void *item)
{
	int cmp;
	struct bst_node *p;
	struct bst_node *q;
	struct bst_node *n;
	unsigned char dir;

	assert(t != NULL && item != NULL);
	
	for (q = NULL, p = t->bst_root; p != NULL; p = p->bst_link[dir]) {
        cmp = t->bst_compare(item, p->bst_data, t->bst_param);
        if (cmp == 0)
            return &p->bst_data;
        dir = cmp > 0;
	}
    n = t->a->libavl_alloc(t->a, sizeof (struct bst_node*));
    if (n == NULL)
        return NULL;
    n->bst_link[0] = n->bst_link[1] = NULL;
    n->bst_data = item;
    if (q != NULL) {
        q->bst_link[dir] = n;
    } else {
        t->bst_root = n;    
    }
    t->bst_count++;
    return &n->bst_data;
}

void**
bst_probe_root(struct bst_table *t, void *item)
{
    int k;
    int cmp;
    unsigned char da[BST_MAX_HEIGHT];
    struct bst_node *p;
    struct bst_node *q;
    struct bst_node *n;
    struct bst_node *pa[BST_MAX_HEIGHT];
    
    k = 1;
    da[0] = 0;
    pa[0] = (struct bst_node*) &t->bst_root;

    for (p = t->bst_root; p != NULL; p = p->bst_link[da[k]], k++) {
        cmp = t->bst_compare(item, p->bst_data, t->bst_compare);
        if (cmp == 0)
            return NULL;
        da[k] = cmp > 0;
        pa[k] = p; 
    
    }
    n = t->a->libavl_alloc(t->a, sizeof (struct bst_node*));
    if (n == NULL)
        return NULL;
    n->bst_link[0] = n->bst_link[1] = NULL;
    n->bst_data = item;
    t->bst_count++;
    t->bst_generation++;

    for (; k > 0; k--) {
        q = pa[k - 1];
        switch (da[k - 1]) {
        case 0: /* when moved right from q left rotate at q */
            q->bst_link[0] = n->bst_link[1];
            n->bst_link[1] = q; 
            break;
        case 1: /* when moved left from q right rotate at q */
            q->bst_link[1] = n->bst_link[0];
            n->bst_link[0] = q;
        
        }
        pa[k - 2]->bst_link[da[k - 2]] = n;
    }
    return &n->bst_data;    
}

int main(void)
{
	return 0;
}
