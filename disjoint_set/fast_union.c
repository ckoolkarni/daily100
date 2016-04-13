#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct set_node {
	int data;
	int child_count;
	struct set_node *parent;
};

void *
alloc_mem(size_t size)
{
    void *p;

	if ((p = malloc(size)) == NULL) {
		perror("malloc : ");
		exit(EXIT_FAILURE);
	}
	return p;
}

struct set_node *
alloc_set_node(int data)
{
	struct set_node *n;

	n = alloc_mem(sizeof(struct set_node));
	n->data = data;
	n->child_count = 0;
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
			"ADDRESS", "DATA", "CHILD_COUNT", "PARENT");
	for (i = 0; i < n; i++) {
		printf("%16p %10d %10d %10d\n", 
				set[i], set[i]->data, set[i]->child_count, 
				set[i]->parent == NULL ? -1 : set[i]->parent->data);
	}
}

struct set_node **
init_set(int n)
{
	int i;
	struct set_node **set;

	set = alloc_mem(sizeof(struct set_node**));

	printf("\n%16s %10s %10s %10s\n",
			"ADDRESS", "DATA", "CHILD_COUNT", "PARENT");
	for (i = 0; i < n; i++) {
		set[i] = alloc_mem(sizeof(struct set_node *));
		set[i]->data = i;
		set[i]->child_count = 0;
		set[i]->parent = NULL;

		printf("%16p %10d %10d %10d\n", 
				set[i], set[i]->data, set[i]->child_count, 
				set[i]->parent == NULL ? -1 : set[i]->parent->data);
	}
	return set;
}

void
make_union(struct set_node **set, int a, int b)
{
	assert(set != NULL);
	
	/* in case of same number of children use data to choose the parent */
	printf("%s \n", set[a] == NULL ? "NULL" : "NOT NULL");
	printf("%d \n", set[b]->child_count);
	if (set[a]->child_count == set[b]->child_count) {
		if (set[a]->data > set[b]->data) {
			set[b]->parent = set[a];
			set[a]->child_count++;
		} else {
			set[a]->parent = set[b];
			set[b]->child_count++;
		}
	} else {
		if (set[a]->child_count > set[b]->child_count) {
			set[b]->parent = set[a];	
			set[a]->child_count++;
		} else {
			set[a]->parent = set[b];	
			set[b]->child_count++;
		}
	}
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
#if 0
    make_union(set, 0, 1);
	print_set(set, n);
    make_union(set, 2, 3);
	print_set(set, n);
    make_union(set, 3, 4);
	print_set(set, n);
    make_union(set, 8, 9);
	print_set(set, n);
    make_union(set, 4, 7);
#endif
	for (i = 0; i < n; i++) {
		t = find_set(set, set[i]);
		printf("set %d element %d\n", t->data, i);
	}
	free_set(set, n);
	return 0;
}
