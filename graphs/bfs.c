#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node {
	int dest;
	int weight;
	struct node *next;
	struct node *prev;
};

struct graph {
	int nvertices;
	struct node **adj_list_array;   
};

struct node * 
alloc_node(int dest, int weight)
{
	struct node *n;

	if ((n = (struct node *) malloc(sizeof(*n))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->dest = dest;
	n->weight = weight;
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
creat_graph(int vertices)
{
	int i;
	struct graph *g;
	
	if ((g = (struct graph *) malloc(sizeof (struct graph))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	
	if ((g->adj_list_array = (struct node **) malloc(
					vertices * sizeof (struct node *))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	g->nvertices = vertices;
	for (i = 0; i < vertices; i++) {
		g->adj_list_array[i] = NULL;
	}

	return g;
}

void
add_edge(struct graph *g, int src, int dest, int weight)
{
	struct node *n;
	struct node *t;
	struct node **head;

	assert(g != NULL && src != INT_MAX && dest != INT_MAX, weight != INT_MAX);
	head = &g->adj_list_array[src];
	n = alloc_node(dest, weight);

	if (*head == NULL) {
		*head = n;	
	} else {
		t = g->adj_list_array[src];
		while (t->next != NULL) 
			t = t->next;
		t->next = n;
		n->prev = t;
	}
}

void
print_graph(struct graph *g)
{
	int i;
	int flag;
	struct node *temp;

	for (i = 0; i < g->nvertices; i++) {
		printf("HEAD-->");
		flag = 0;
		for (temp = g->adj_list_array[i]; temp != NULL; temp = temp->next) { 
			flag = 1;
			printf("[dest = %2d, wt %2d]<--", temp->dest, temp->weight);
		}
		printf("%s\n", flag == 1 ? "TAIL" : "NULL" );
	}
}

void
bfs(struct graph *g, int src)
{
	assert(g != NULL);
	struct queue *q;
	struct node *curr;

	q = create_queue();

	enqueue(q, g-adj_list_array[src]);
	while(! is_empty(q)) {
		for (curr = dqueue(q); curr != NULL; curr = curr->next) {
			enqueue(q, (void *) curr->dest);
		}
	}
}

int main(int argc, char *argv[])
{
	struct graph *g;

	g = creat_graph(4);

	add_edge(g, 0, 1, 10);
	add_edge(g, 0, 2, 5);
	add_edge(g, 1, 3, 7);
	add_edge(g, 2, 3, 15);

	print_graph(g);
    bsf(g);
	return 0;
}

































