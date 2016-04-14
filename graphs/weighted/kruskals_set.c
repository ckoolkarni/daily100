#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
 
struct set_node {
	int id;
	int rank;
	struct set_node *parent;
};

struct edge_node {
	int src;
	int dest;
	int weight;
};

struct graph {
	int curr;
	int nedges;
	int nvertices;
	struct edge_node **array;
};

void
print_mst(struct graph *g)
{
	int i;

	printf("\n%10s %10s %10s %10s\n", 
			"ADDR", "SRC", "DEST", "WEIGHT");
	for (i = 0 ; i < g->nedges; i++) {
		printf("%10p %10d %10d %10d\n", g->array[i], g->array[i]->src, 
				g->array[i]->dest, g->array[i]->weight);
	}
}

void *
alloc_mem(size_t size)
{
	void *n;
	if ((n = malloc(size)) == NULL) {
		perror("malloc : ");
		exit(EXIT_FAILURE);
	}
	return n;
}

struct edge_node *
alloc_edge_node(int src, int dest, int weight)
{
	struct edge_node *n;

	assert(src >= 0 && dest >= 0 && src != dest);

	n = (struct edge_node *) alloc_mem(sizeof(*n));

	n->src = src;
	n->dest = dest;
	n->weight = weight;

	return n;
}

void
free_mem(void *p)
{
	if (p != NULL)
		free(p);
}

struct graph *
create_graph(int nvertices, int nedges)
{
	int i;
	struct graph *g;

	assert(nedges >= 0);
    g = (struct graph *) alloc_mem(sizeof (struct graph));

	g->array = (struct edge_node **) 
				alloc_mem(nedges * sizeof (struct edge_node *));
    printf("array %10p\n", g->array);
	g->curr = 0;
    g->nedges = nedges;
    g->nvertices = nvertices;
	for (i = 0; i < nedges; i++) {
		g->array[i] = alloc_edge_node(i, INT_MAX, INT_MAX);
		printf("array[%d] %10p\n", i, g->array[i]);
	}

	return g;
}

void
add_edge(struct graph *g, int src, int dest, int weight)
{
	assert(g != NULL && src != INT_MAX && dest != INT_MAX);

	g->array[g->curr]->src = src;
	g->array[g->curr]->dest = dest;
	g->array[g->curr]->weight = weight;
	g->curr++;
}

void
destroy_graph(struct graph *g)
{
	int i;

	assert(g != NULL);
	for (i = 0; i < g->nedges; i++) {
		free_mem(g->array[i]);
	}
	free_mem(g->array);
	free_mem(g);
}

struct set_node *
alloc_set_node(int id)
{
	struct set_node *n;

	n = (struct set_node *) alloc_mem(sizeof(struct set_node));
	n->id = id;
	n->rank = 0;
	n->parent = NULL;
	return n;
}

void
print_set(struct set_node **set, int n)
{
	int i;

	printf("\n%16s %10s %10s %10s\n",
			"ADDRESS", "DATA", "rank", "PARENT");
	for (i = 0; i < n; i++) {
		printf("%16p %10d %10d %10d\n", 
				set[i], set[i]->id, set[i]->rank, 
				set[i]->parent == NULL ? -1 : set[i]->parent->id);
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

int 
find_set(struct set_node **set, int n)
{
	int root;

	assert(set != NULL);

	if (set[n]->parent == NULL) {
		return set[n]->id;
	}
	root = find_set(set, set[n]->parent->id);
/*	Path Compression optimization
	if (set[root]->parent == NULL && set[n]->parent != root) {
		set[n]->parent = root;
	}*/
	return root;
}

void
make_union(struct set_node **set, int a, int b)
{
	struct set_node *aroot;
	struct set_node *broot;

	assert(set != NULL);

    aroot = set[find_set(set, a)];
    broot = set[find_set(set, b)];

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

int
weight_compare(const void *a, const void *b)
{
	struct edge_node **aedge;
	struct edge_node **bedge;

	assert(a != NULL && b!= NULL);

	aedge = (struct edge_node **) a;
	bedge = (struct edge_node **) b;

	if ((*aedge)->weight < (*bedge)->weight)
		return -1;
	else if ((*aedge)->weight > (*bedge)->weight)
		return 1;
	else 
		return 0;
}

void
kruskal(struct graph *g)
{
	int i;
	int src_root;
	int dest_root;
	int result_count;
	struct set_node **set;
	struct edge_node *curr_edge;
	struct edge_node **result;
	
	set = init_set(g->nvertices);

	result = (struct edge_node **) malloc(g->curr * sizeof (struct edge_node*));
    result_count = 0;
	for (i = 0; i < g->nedges; i++) {
        curr_edge = g->array[i];
        assert(curr_edge != NULL);
		printf("Now Processing EDGE SRC %d DEST %d\n", curr_edge->src, curr_edge->dest);
		src_root = find_set(set, curr_edge->src);
		dest_root = find_set(set, curr_edge->dest);

		if (src_root != dest_root) {
			result[result_count++] = curr_edge;	
			make_union(set, curr_edge->src, curr_edge->dest);
		}
	}

    printf("%10s %10s\n", "SRC", "DEST");
	for (i = 0 ; i < result_count; i++) {
		printf("%10d %10d\n", result[i]->src, result[i]->dest);
	}
	free(result);
}

int main(int argc, char *argv[])
{
	int edges;
	int vertices;
	struct graph *g;

#if 1
	edges = 14;
	vertices = 9;
	g = create_graph(vertices, edges);

	add_edge(g, 0, 1, 4);
	add_edge(g, 0, 7, 8);
	add_edge(g, 1, 2, 8);
	add_edge(g, 1, 7, 11);
	add_edge(g, 2, 3, 7);
	add_edge(g, 2, 8, 2);
	add_edge(g, 3, 4, 9);
	add_edge(g, 3, 5, 14);
	add_edge(g, 4, 5, 10);
	add_edge(g, 5, 2, 4);
	add_edge(g, 5, 6, 2);
	add_edge(g, 6, 8, 6);
	add_edge(g, 6, 7, 1);
	add_edge(g, 7, 8, 7);
#endif

	print_mst(g);
	qsort(g->array, g->curr, sizeof (g->array[0]), weight_compare);
	print_mst(g);
	kruskal(g);
    destroy_graph(g);

	return 0;
}
