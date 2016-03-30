#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

struct stack_node {
	void *data;
	bool visited;
	struct stack_node *next;
	struct stack_node *prev;

};

struct stack {
	int current_count;
	struct stack_node *top;
};

struct node {
	int id;
	int dist;
	int dest;
	int parent;
	bool discovered;
	bool processed;
	unsigned long entry_time;
	unsigned long exit_time;
	struct node *next;
	struct node *prev;
};

struct graph {
	int vertices;
	unsigned long time;
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
			printf("[id %2d dest = %2d discovered %s]<--", temp->id,
					temp->dest, temp->discovered == true ? "TRUE": "FALSE");
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
	n->dist = INT_MAX;
	n->dest = dest;
	n->parent = INT_MAX;
	n->discovered = false;
	n->discovered = true;
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
	g->time = 0;
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

void
print_stack(struct stack *s)
{
	struct stack_node *temp;

	assert(s != NULL);
    printf("TOP <-");
	for (temp = s->top; temp != NULL; temp = temp->prev) {
		printf("<-%2d", ((struct node *)temp->data)->id);
	}
    printf("<-BOTTOM\n");
}

struct stack_node *
alloc_stack_node(void *data)
{
	struct stack_node *n;

	if ((n = (struct stack_node *) malloc(sizeof(struct stack_node))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->data = data;
	n->visited = false;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

void
free_stack_node(struct stack_node *n)
{
	if (n != NULL)
		free(n);
}

struct stack *
creat_stack(void)
{
	struct stack *s;

	if ((s = (struct stack *) malloc(sizeof (struct stack))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	s->top = NULL;
	s->current_count = 0;

	return s;
}

void
push (struct stack *s, void *data)
{
    struct stack_node *n;

	assert(s != NULL);
	n = alloc_stack_node(data);

	if (s->top == NULL) {
		s->top = n;
	} else {
		s->top->next = n;
		n->prev = s->top;
		s->top = n;
	}
	s->current_count++;
}

void *
pop(struct stack *s)
{
	void *data;
	struct stack_node *t;

	assert(s != NULL);
	if (s->top == NULL) {
		fprintf(stderr, "stack underflow\n");
		exit(EXIT_FAILURE);
	}

	t = s->top;
	s->top = s->top->prev;
	/* Last element in the stack */
	if (s->top != NULL)
		s->top->next = NULL;
    data = t->data;
	free_stack_node(t);
	s->current_count--;

	return data;
}

int
is_empty(struct stack *s)
{
	return s->top == NULL;
}

void
process_vertex_early(struct graph *g, int v)
{
	printf(" %s : %d\n ", __func__, v);
}

void 
process_edge(struct graph *g, int x, int y)
{
	printf(" %s : (%d, %d)\n", __func__, x, y);
}

void 
process_vertex_later(struct graph *g, int v)
{
	printf(" %s : %d\n ", __func__, v);
}


void
dfs(struct graph *g, int src)
{
	struct stack *s;
	struct node *curr;
  
	assert(g != NULL);

	s = creat_stack();

	if (g->adj_list_array[src]->discovered == false) {
		printf("push %d ", g->adj_list_array[src]->id);
		push(s, g->adj_list_array[src]);
	}

	while (! is_empty(s)) {
		curr = (struct node*) s->top->data;
		process_vertex_early(g, curr->id);
		if (curr->discovered == false) {
			curr->discovered = true;
			curr = curr->next;
			/* find undicovered vertext which is in to adj(s->top) */
			while (curr != NULL && curr->discovered == true) {
				curr = curr->next;
			}

			if (curr != NULL) {
				curr->discovered = true;
				if (g->adj_list_array[curr->dest]->discovered == false) {
					printf("push %d ", g->adj_list_array[curr->dest]->id);
					process_edge(g, curr->id, curr->dest);
					g->adj_list_array[curr->dest]->parent = curr->id;
					push(s, g->adj_list_array[curr->dest]);
				}
			} else {
				curr = pop(s);
				process_vertex_later(g, curr->id);
				curr->processed = true;
				printf("pop %d ", curr->id);
			}
		}
	}
	printf("\n");
}


int main(void)
{
	int i;
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
	for (i = 0; i < g->vertices; i++) {
		printf("--------------------%d--------------------\n", i);
		dfs(g, i);
		print_graph(g);
	}
	return 0;
}
