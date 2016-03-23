#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

struct node {
	int id;
	int dest;
	struct node *next;
	struct node *prev;
};

struct graph {
	int vertices;
	struct node **adj_list_array;
};

void
print_graph(struct graph *g)
{
	int i;
	int flag;
	struct node *temp;

	assert(g != NULL);
	for (i = 0; i < g->vertices; i++) {
		printf("HEAD-->");
		flag = 0;
		for (temp = g->adj_list_array[i]; temp != NULL; temp = temp->next) {
			flag = 1;
			printf("[%p id %2d dest = %2d]<--", temp, temp->id, temp->dest);
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
create_graph(int vertices)
{
	int i;
	struct graph *g;

	assert(vertices >= 0);
	if ((g = (struct graph *) malloc(sizeof (struct graph))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	if ((g->adj_list_array = (struct node **) malloc (vertices * 
								sizeof (struct node *))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
    g->vertices = vertices;
	for (i = 0; i < vertices; i++) {
		g->adj_list_array[i] = alloc_node(i, INT_MAX);
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
	t = g->adj_list_array[src];

	while (t->next != NULL) {
		t = t->next;
	}
	
	n->prev = t;
	t->next = n;
}

int main(void)
{
	struct graph *g;

	g = create_graph(4);

	/* Graph 1 */
	add_edge(g, 0, 1);
	add_edge(g, 0, 2);
	add_edge(g, 1, 3);
	add_edge(g, 2, 3);
	print_graph(g);
	return 0;
}
