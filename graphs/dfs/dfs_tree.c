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
	bool visited;
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
			printf("[id %2d dest = %2d visited %s]<--", temp->id,
					temp->dest, temp->visited == true ? "TRUE": "FALSE");
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
dfs_tree(struct graph *g, int src)
{
	struct stack *s;
	struct node *curr;
  
	assert(g != NULL);

	s = creat_stack();
	g->adj_list_array[src]->dist = 0;
	push(s, g->adj_list_array[src]);

	while (! is_empty(s)) {
		curr = pop(s);
		curr = curr->next;
		while (curr != NULL) {
			if (g->adj_list_array[curr->dest]->dist == INT_MAX) {
			     g->adj_list_array[curr->dest]->dist =
					 g->adj_list_array[curr->id]->dist + 1;
				push(s, g->adj_list_array[curr->dest]);
				printf("%d %d\n", curr->id, curr->dest);
			}
			curr = curr -> next;
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
	print_graph(g);
#endif

#if 1
	g = create_graph(7);
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
	print_graph(g);
#endif
	i = 0;
	dfs_tree(g, i);
	for (i = 0; i < g->vertices ; i++) {
		printf("dist [%d] %d\n", i, g->adj_list_array[i]->dist);
	}
	return 0;
}
