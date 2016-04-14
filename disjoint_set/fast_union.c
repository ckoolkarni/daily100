#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct set_node {
	int data;
	int rank;
	struct set_node *parent;
};

void *
alloc_mem(size_t size)
{
    void *p;

	if ((p = (void *) malloc(size)) == NULL) {
		perror("malloc : ");
		exit(EXIT_FAILURE);
	}
	return p;
}

struct set_node *
alloc_set_node(int data)
{
	struct set_node *n;

	n = (struct set_node *)alloc_mem(sizeof(struct set_node));
	n->data = data;
	n->rank = 0;
	n->parent = NULL;
	return n;
}

void 
free_mem(void *p)
{
	if (p != NULL)
		free(p);
}

void
print_set(struct set_node **set, int n)
{
	int i;

	printf("\n%16s %10s %10s %10s\n",
			"ADDRESS", "DATA", "rank", "PARENT");
	for (i = 0; i < n; i++) {
		printf("%16p %10d %10d %10d\n", 
				set[i], set[i]->data, set[i]->rank, 
				set[i]->parent == NULL ? -1 : set[i]->parent->data);
	}
}

struct set_node **
init_set(int n)
{
	int i;
	struct set_node **set;

	set = alloc_mem(n * sizeof(struct set_node **));

	for (i = 0; i < n; i++) {
	    set[i] = alloc_set_node(i);
	}

	print_set(set, n);
	return set;
}

struct set_node * 
find_set(struct set_node **set, struct set_node *n)
{
	struct set_node *root;

	assert(set != NULL);

	if (n->parent == NULL) {
		return n;
	}
	root = find_set(set, n->parent);
	if (root->parent == NULL && n->parent != root) {
		n->parent = root;
	}
	return root;
}

void
make_union(struct set_node **set, int a, int b)
{
	struct set_node *aroot;
	struct set_node *broot;

	assert(set != NULL);

    aroot = find_set(set, set[a]);
    broot = find_set(set, set[b]);

	if (aroot->rank < broot->rank) {
		aroot->parent = broot;	
	} else if (aroot->rank > broot->rank) {
		broot->parent = aroot;	
	} else {
		broot->parent = aroot;
		aroot->rank++;
	}
}

void
free_set(struct set_node **set, int n)
{
	int i;

	assert(set != NULL);

	for (i = 0; i < n; i++) {
		free_mem(set[i]);
	}
	free_mem(set);
}

int main(int argc, char *argv[])
{
	int i;
    int n;
	struct set_node *t;
	struct set_node **set;

	n = 10;
	set = init_set(n);

	print_set(set, n);
    make_union(set, 0, 1);
    make_union(set, 2, 3);
    make_union(set, 3, 4);
    make_union(set, 8, 9);
    make_union(set, 4, 7);
	for (i = 0; i < n; i++) {
		t = find_set(set, set[i]);
		printf("set %d element %d\n", t->data, i);
	}

	free_set(set, n);
	return 0;
}
