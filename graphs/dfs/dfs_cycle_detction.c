#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

struct node {
	int id;
	int dest;
	int weight;
	int parent;
	bool processed;
	bool discovered;
    unsigned long entry_time;
	unsigned long exit_time;
	struct node *next;
	struct node *prev;
};

struct graph {
	int time;
	int nvertices;
	bool finished;
	bool directed;
	struct node **array;
};

void
print_graph(struct graph *g)
{
	int i;
	int flag;
	struct node *temp;

	assert(g != NULL);
	for (i = 0; i < g->nvertices; i++) {
		printf("HEAD-->");
		flag = 0;
		for (temp = g->array[i]; temp != NULL; temp = temp->next) {
			flag = 1;
			printf("[id %2d dest = %2d ]<--", temp->id,
					temp->dest);
		}
		printf("%s\n", flag == 1 ? "TAIL" : "NULL" );
	}
}

struct node *
alloc_node(int src, int dest)
{
	struct node *n;

	assert(src >= 0 && dest >= 0 && src != dest);

	if ((n = (struct node *) malloc(sizeof(*n))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->id = src;
	n->dest = dest;
	n->parent = INT_MAX;
	n->entry_time = INT_MAX;
	n->exit_time = INT_MAX;
	n->processed = false;
	n->discovered = false;
	n->next = NULL;
	n->prev = NULL;

	return n;
}

void
free_node(struct node *n)
{
	if (n != NULL)
		free(n);
}

struct graph *
create_graph(int nvertices)
{
	int i;
	struct graph *g;

	assert(nvertices >= 0);
	if ((g = (struct graph *) malloc(sizeof (struct graph))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	if ((g->array = (struct node **) malloc (nvertices *
								sizeof (struct node *))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	g->time = 0;
    g->nvertices = nvertices;
	g->finished = false;
	g->directed = true;
	for (i = 0; i < nvertices; i++) {
		g->array[i] = alloc_node(i, INT_MAX);
	}

	return g;
}

void
add_edge(struct graph *g, int src, int dest)
{
	struct node *n;
	struct node *t;

	assert(g != NULL && src != INT_MAX && dest != INT_MAX);

	n = alloc_node(src, dest);
	t = g->array[src];

	while (t->next != NULL) {
		t = t->next;
	}
	
	n->prev = t;
	t->next = n;
}


void
destroy_graph(struct graph *g)
{
	int i;
	struct node *c;
	struct node *t;

	assert(g != NULL);

	for (i = 0 ; i < g->nvertices; i++) {

		for (c = g->array[i]->next; c != NULL; 
						t = c, c = c->next, free_node(t)); 

		free_node(g->array[i]);
	}

	free(g->array);

	free(g);
}


void 
find_path(struct graph *g, int start, int end)
{
	assert(g != NULL);

	if ((start == end) || end == -1) {
		printf("\n%d", start);
	} else {
		find_path(g, start, g->array[end]->parent); 
		printf("\n%d", end);
	}
}

void
process_vertex_early(struct graph *g, int v)
{
	printf("%d ", v);
}

void 
process_edge(struct graph *g, int x, int y)
{
	if (g->array[y]->discovered == true && g->array[x]->parent != y) {
		g->finished = true;
		printf("cycle from %d %d\n",y, x);
		find_path(g, y, x);
		printf("\n\n");
	}
}

void 
process_vertex_late(struct graph *g, int v)
{

}

void
dfs(struct graph *g, int src)
{
	struct node *t;
	struct node *curr;
  
	assert(g != NULL);

	if (g->finished)
		return;

	g->time++;
	g->array[src]->discovered = true;
	g->array[src]->entry_time = g->time;
    t = curr = g->array[src];
 	process_vertex_early(g, src);
    for (curr = curr->next; curr != NULL; curr = curr->next) {
		if (g->array[curr->dest]->discovered == false) {
			g->array[curr->dest]->parent = src;
			process_edge(g, src, curr->dest);
			dfs(g, curr->dest);
			/* ??? */
		} else if ((! g->array[curr->dest]->processed) || g->directed == true) {
			process_edge(g, src, curr->dest);
		}
		if (g->finished)
			return;
	}
	process_vertex_late(g, src);
	g->time++;
	t->exit_time = g->time;
	t->processed = true;
}

struct graph *
build_graph(void)
{
	struct graph *g;

#if 0
	/* Graph 1 */
	g = create_graph(4);
	add_edge(g, 0, 1);
	add_edge(g, 0, 2);
	add_edge(g, 1, 3);
	add_edge(g, 2, 3);
#endif

#if 1
	g = create_graph(9);
	add_edge(g, 0, 1);
	add_edge(g, 0, 4);
	add_edge(g, 1, 2);
	add_edge(g, 1, 4);
	add_edge(g, 1, 6);
	add_edge(g, 2, 3);
	add_edge(g, 2, 4);
	add_edge(g, 3, 5);
	add_edge(g, 4, 5);
	add_edge(g, 5, 6);
	add_edge(g, 7, 3);
	add_edge(g, 7, 8);
	add_edge(g, 8, 3);
	print_graph(g);
#endif
	return g;
}

int main(void)
{
	int i;
	struct graph *g;

	g = build_graph();
	for (i = 0; i < g->nvertices; i++) {
		printf("--------------------%d--------------------\n", i);
		dfs(g, i);
		print_graph(g);
	}
	destroy_graph(g);
	return 0;
}
