#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

struct node {
	int id;
	int dest;
	int weight;
	bool visited;
	struct node *next;
	struct node *prev;
};

struct graph {
	int nvertices;
	struct node **adj_list_array;   
};

struct node * 
alloc_node(int src, int dest, int weight)
{
	struct node *n;

	if ((n = (struct node *) malloc(sizeof(*n))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->id = src;
	n->dest = dest;
	n->weight = weight;
	n->visited = false;
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
		g->adj_list_array[i] = alloc_node(i, INT_MAX, 0);
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
	t = g->adj_list_array[src];

	while (t->next != NULL) 
		t = t->next;
	t->next = n;
	n->prev = t;
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
			printf("[%p id %2d dest = %2d]<--", temp, temp->id, temp->dest);
		}
		printf("%s\n", flag == 1 ? "TAIL" : "NULL" );
	}
}

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
print_queue(struct queue *q)
{
	struct queue_node *t;

	printf("HEAD ");
	for (t = q->head; t != NULL; t = t->prev)
		printf("<-%p ", t->data);
	printf("<-TAIL\n");
}

struct queue_node * 
alloc_queue_node(void *data)
{
	struct queue_node *n;

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

	if (q->head == NULL || q->tail == NULL) {
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
	return q->head == NULL && q->tail == NULL;	
}

unsigned long long 
get_current_length(struct queue *q)
{
	return q->current_length;
}


void
bfs(struct graph *g, int src)
{
	struct queue *q;
	struct node *curr;

	assert(g != NULL);

	q = create_queue();

	curr = g->adj_list_array[src];
	enqueue(q, curr);

	while (! is_empty(q)) {
		curr = dequeue(q);
		printf("id %d dest %d\n", curr->id, curr->dest);
		curr = curr->next;
		while (curr != NULL) {
			if (g->adj_list_array[curr->dest]->visited == false) {
				g->adj_list_array[curr->dest]->visited = true;
				enqueue(q, g->adj_list_array[curr->dest]);
			}
			curr = curr->next;
		}
	}
}

int main(int argc, char *argv[])
{
	struct graph *g;

	g = creat_graph(14);

#if 0
	/* Graph 1 */
	add_edge(g, 0, 1, 10);
	add_edge(g, 0, 2, 5);
	add_edge(g, 1, 3, 7);
	add_edge(g, 2, 3, 15);
#endif
	/* Graph 2 */
	add_edge(g, 0, 1, 4);
    add_edge(g, 0, 7, 8);
    add_edge(g, 1, 2, 8);
    add_edge(g, 1, 7, 11);
    add_edge(g, 2, 3, 7);
    add_edge(g, 2, 8, 2);
    add_edge(g, 2, 5, 4);
    add_edge(g, 3, 4, 9);
    add_edge(g, 3, 5, 14);
    add_edge(g, 4, 5, 10);
    add_edge(g, 5, 6, 2);
    add_edge(g, 6, 7, 1);
    add_edge(g, 6, 8, 6);
    add_edge(g, 7, 8, 7);

	print_graph(g);
    bfs(g, 0);
	return 0;
}

































