#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

enum color {
	UNCOLORED,
	WHITE,
	BLACK
};

struct node {
	int id;
	int dest;
	int weight;
	int parent;
	bool processed;
	bool discovered;
	enum color color;
	struct node *next;
	struct node *prev;
};

struct graph {
	int nvertices;
	bool bipartite;
	struct node **array;
};
struct queue_node {
    void *data;
	struct queue_node *next;
	struct queue_node *prev;
};

struct queue {
	struct queue_node *head;
	struct queue_node *tail;
	unsigned long long current_length;
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
			printf("[%p id %2d dest = %2d]<--", temp, temp->id, temp->dest);
		}
		printf("%s\n", flag == 1 ? "TAIL" : "NULL" );
	}
}

struct node *
alloc_node(int src, int dest, int weight)
{
	struct node *n;

	assert(src >= 0 && dest >= 0 && src != dest);
	if ((n = (struct node *) malloc(sizeof(*n))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->id = src;
	n->dest = dest;
	n->weight = weight;
	n->parent = INT_MAX;
	n->processed = false;
	n->discovered = false;
	n->color = UNCOLORED;
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
	
	assert(vertices > 0);
	if ((g = (struct graph *) malloc(sizeof (struct graph))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	
	if ((g->array = (struct node **) malloc(
					vertices * sizeof (struct node *))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	g->nvertices = vertices;
	g->bipartite = false;
	for (i = 0; i < vertices; i++) {
		g->array[i] = alloc_node(i, INT_MAX, 0);
	}

	return g;
}

void
add_edge(struct graph *g, int src, int dest, int weight)
{
	struct node *n;
	struct node *t;

	assert(g != NULL && src != INT_MAX && dest != INT_MAX && weight != INT_MAX);

	n = alloc_node(src, dest, weight);
	t = g->array[src];

	while (t->next != NULL)
		t = t->next;
	t->next = n;
	n->prev = t;
}

void
print_queue(struct queue *q)
{
	struct queue_node *t;

	assert(q != NULL);
	printf("HEAD ");
	for (t = q->head; t != NULL; t = t->prev)
		printf("<-%p ", t->data);
	printf("<-TAIL\n");
}

struct queue_node *
alloc_queue_node(void *data)
{
	struct queue_node *n;

	assert(data != NULL);
	if ((n = (struct queue_node *) malloc(sizeof(struct queue_node))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	n->data = data;
	n->next = NULL;
	n->prev = NULL;

	return n;
}

void
free_queue_node(struct queue_node *n)
{
	if (n != NULL)
		free(n);
}

struct queue *
create_queue(void)
{
	struct queue *q;

	if ((q = (struct queue *) malloc(sizeof (struct queue))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	q->head = NULL;
	q->tail = NULL;
    q->current_length = 0;
	return q;	
}

void
enqueue(struct queue *q, void *data)
{
	struct queue_node *n;

	assert(q != NULL && data != NULL);
	n = alloc_queue_node(data);

	if (q->head == NULL && q->tail == NULL) {
		q->head = q->tail = n;		
	} else {
		n->prev = q->head;
		q->head->next = n;
		q->head = n;
	}
    q->current_length++;
}

void *
dequeue(struct queue *q)
{
	void *data;
	struct queue_node *t;

	assert(q != NULL);
	if (q->head == NULL && q->tail == NULL) {
		fprintf(stderr, "Queue Underflow\n");
		exit(EXIT_FAILURE);
	}

	t = q->tail;
	data = t->data;
	if (q->head == q->tail) {
		q->head = q->tail = NULL;		
	} else {
		q->tail = q->tail->next;
		q->tail->prev = NULL;
	}

	free_queue_node(t);
    q->current_length--;
	return data;	
}

int
is_empty(struct queue *q)
{
	assert(q != NULL);
	return q->head == NULL && q->tail == NULL;	
}

void
destroy_queue(struct queue *q) 
{
	assert(q != NULL && q->head == NULL && q->tail == NULL);
	free(q);
}

unsigned long long
get_current_length(struct queue *q)
{
	assert(q != NULL);
	return q->current_length;
}

void
procress_vertex_early(struct node *v)
{
}

void 
process_edge(struct graph *g, int x, int y)
{
	if (g->array[x]->color == g->array[y]->color) {
		g->bipartite = false;
		printf("not bipartite (%d, %d)\n", x, y);
	} else {

		assert(g->array[y]->color == UNCOLORED);

		switch (g->array[x]->color) {
			case WHITE:
				g->array[y]->color = BLACK;
				break;
			case BLACK:
				g->array[y]->color = WHITE;
				break;
			default:
				fprintf(stderr, "invalid color for %d\n", x);
		}
	}
}

void 
process_vertex_late(struct node *v)
{

}

void
bfs(struct graph *g, int src)
{
	struct queue *q;
	struct node *t;
	struct node *curr;

	assert(g != NULL && src >= 0);

	q = create_queue();

	curr = g->array[src];
	curr->discovered = true;
	enqueue(q, curr);

	while (! is_empty(q)) {
		t = curr = dequeue(q);
		procress_vertex_early(curr);
        curr->processed = true;
		curr = curr->next;
		while (curr != NULL) {

			if (g->array[curr->dest]->processed == false) {
				process_edge(g, curr->id, curr->dest);
			}
			
			if (g->array[curr->dest]->discovered == false) {

				g->array[curr->dest]->parent = curr->id;
				g->array[curr->dest]->discovered = true;

				enqueue(q, g->array[curr->dest]);

			}
			curr = curr->next;
		}
		process_vertex_late(t);
	}
	destroy_queue(q);
}

struct graph *
build_graph(void)
{

	struct graph *g;

	g = creat_graph(12);

	add_edge(g, 0, 6, 0);
	add_edge(g, 0, 7, 0);
	add_edge(g, 1, 8, 0);
	add_edge(g, 2, 9, 0);
	add_edge(g, 2, 10, 0);
	add_edge(g, 2, 11, 0);
	add_edge(g, 3, 7, 0);
	add_edge(g, 4, 6, 0);
	add_edge(g, 4, 7, 0);
	add_edge(g, 5, 11, 0);

	return g;
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
two_colors(struct graph *g)
{
	int i;

	for (i = 0; i < g->nvertices; i++) {
		if (g->array[i]->discovered == false) {
			g->array[i]->color = WHITE;
			bfs(g, i);
		}
	}
}


int main(int argc, char *argv[])
{
	struct graph *g;

	g = build_graph();

	print_graph(g);

	two_colors(g);

    destroy_graph(g);

	return 0;
}
