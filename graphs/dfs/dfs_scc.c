#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

bool g_directed = true;

enum EDGE_TYPE {
	UNCLASSIFIED,
	TREE,
	BACK,
	FORWARD,
	CROSS
};

struct stack_node {
	int data;
	struct stack_node *next;
	struct stack_node *prev;

};

struct stack {
	int current_count;
	struct stack_node *top;
};

struct node {
	int id;
	int dest;
	int scc;                    /* scc number to which this vertex belongs */
	int weight;
	int parent;
	int known_oldest_vertex;	/* oldest known vertex for in same scc */
	bool processed;
	bool discovered;
    unsigned long entry_time;
	unsigned long exit_time;
	enum EDGE_TYPE edge_type;
	struct node *next;
	struct node *prev;
};

struct graph {
	int time;
	int nvertices;
	int component_found;
	bool finished;
	bool directed;
	struct stack *s;
	struct node **array;
};

void
print_stack(struct stack *s)
{
	struct stack_node *temp;

	assert(s != NULL);
    printf("TOP <-");
	for (temp = s->top; temp != NULL; temp = temp->prev) {
		printf("<-%2d", temp->data);
	}
    printf("<-BOTTOM\n");
}

struct stack_node *
alloc_stack_node(int data)
{
	struct stack_node *n;

	if ((n = (struct stack_node *) malloc(sizeof(struct stack_node))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->data = data;
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
create_stack(void)
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
push (struct stack *s, int data)
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

int
pop(struct stack *s)
{
	int data;
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
destroy_stack(struct stack *s)
{
	if (s != NULL)
		free(s);
}

void
print_graph(struct graph *g)
{
	int i;
	int flag;
	struct node *temp;

	assert(g != NULL);
	printf("--------------------%s--------------------\n", 
			g_directed == true? "DIRECTED" : "UNDIRECTED");
	for (i = 0; i < g->nvertices; i++) {
		printf("HEAD-->");
		flag = 0;
		for (temp = g->array[i]; temp != NULL; temp = temp->next) {
			flag = 1;
			printf("[id %2d dest = %2d ]-->", temp->id,
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
	n->known_oldest_vertex = INT_MAX;
	n->scc = INT_MAX;
	n->processed = false;
	n->discovered = false;
	n->edge_type = UNCLASSIFIED;
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
	g->component_found = 0;
	g->directed = true;
    g->s = create_stack();
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

	destroy_stack(g->s);
	for (i = 0 ; i < g->nvertices; i++) {
		for (c = g->array[i]->next; c != NULL; 
						t = c, c = c->next, free_node(t))
			; 
		free_node(g->array[i]);
	}
	free(g->array);
	free(g);
}


enum EDGE_TYPE 
edge_classifiaction(struct graph *g, int x, int y)
{
	if (x == g->array[y]->parent) {
		printf("EDGE (%d %d) is TREE Edge\n", x, y);
		return TREE;
	}
	if (g->array[y]->discovered && (! g->array[y]->processed)) {
		printf("EDGE (%d %d) is BACK Edge\n", x, y);
		return BACK;
	}
	if (g->array[y]->processed && 
			(g->array[y]->entry_time > g->array[x]->entry_time)) {
		printf("EDGE (%d %d) is FORWARD Edge\n", x, y);
   		return FORWARD;
	}
	if (g->array[y]->processed && 
			(g->array[y]->entry_time < g->array[x]->entry_time)) {
		printf("EDGE (%d %d) is CROSS Edge\n", x, y);
		return CROSS;
	}

	printf("ERROR EDGE (%d %d) is UNCLASSIFIED Edge\n", x, y);
	return UNCLASSIFIED;
}

void 
pop_component(struct graph *g, int v)
{
	int t;

	g->component_found++;
	g->array[v]->scc = g->component_found;
	while ((t = pop(g->s)) != v) {
		g->array[t]->scc = g->component_found;
	}
}

void
process_vertex_early(struct graph *g, int v)
{
	push(g->s, v);
}

void 
process_edge(struct graph *g, int x, int y)
{
	enum EDGE_TYPE class;
	struct node *x_known_oldest_vertex;
	
	x_known_oldest_vertex = g->array[g->array[x]->known_oldest_vertex];

	class = edge_classifiaction(g, x, y);

	if (class == BACK) {
		if (g->array[y]->entry_time < x_known_oldest_vertex->entry_time)
			g->array[x]->known_oldest_vertex = y;
	}

	if (class == CROSS) {
		if (g->array[y]->scc == INT_MAX) {
			if (g->array[y]->entry_time	< x_known_oldest_vertex->entry_time) {
				g->array[x]->known_oldest_vertex = y;
			}
		}
	}
}

void 
process_vertex_late(struct graph *g, int v)
{
	struct node *v_known_oldest_vertex;
	struct node *v_parent_known_oldest_vertex;

    v_known_oldest_vertex = g->array[g->array[v]->known_oldest_vertex];
    v_parent_known_oldest_vertex = 
		g->array[g->array[g->array[v]->parent]->known_oldest_vertex];
	if (g->array[v]->known_oldest_vertex = v) {
		pop_component(g, v);
	}

	if (v_known_oldest_vertex->entry_time < 
			v_parent_known_oldest_vertex->entry_time) {
		    g->array[g->array[v]->parent]->known_oldest_vertex = 
				g->array[v]->known_oldest_vertex;
	}
}

void
dfs(struct graph *g, int src)
{
	struct node *t;
	struct node *curr;
  
	assert(g != NULL);

    t = curr = g->array[src];
	curr->discovered = true;
	curr->entry_time = ++g->time;

 	process_vertex_early(g, src);
    for (curr = curr->next; curr != NULL; curr = curr->next) {
		if (g->array[curr->dest]->discovered == false) {
			g->array[curr->dest]->parent = src;
			process_edge(g, src, curr->dest);
			dfs(g, curr->dest);
			/* ??? */
		} else if ((! g->array[curr->dest]->processed) || g_directed) {
			process_edge(g, src, curr->dest);
		}
	}
	process_vertex_late(g, src);

	t->exit_time = ++g->time;
	t->processed = true;
}

void print_dfs(struct graph *g)
{
	int i;

	assert(g != NULL);
	printf("--------------------%s--------------------\n", 
			g_directed == true? "DIRECTED" : "UNDIRECTED");
	printf("%10s %10s %10s %10s %10s %10s\n", 
			"src", "parent", "discovered", "processed", 
			"entry_time", "exit_time");
	for (i = 0; i < g->nvertices; i++) {
		printf("%10d %10d %10s %10s %10lu %10lu\n",
				g->array[i]->id, g->array[i]->parent, 
				g->array[i]->discovered == true ? "TRUE" : "FALSE",
				g->array[i]->processed == true ? "TRUE" : "FALSE",
				g->array[i]->entry_time, g->array[i]->exit_time);
	}
}

struct graph *
build_graph(char *fname, int nvertices, int nedges)
{
	int i;
	int arr[nedges][2];
	FILE *fp;
	struct graph *g;

	if ((fp = fopen(fname, "r")) == NULL) {
		perror("ERROR : ");
		exit(EXIT_FAILURE);
	}

	g = create_graph(nvertices);

	for (i = 0; fscanf(fp, "%d %d", &arr[i][0], &arr[i][1]) != EOF; i++) {
		add_edge(g, arr[i][0], arr[i][1]);
		
		if (! g_directed)
			add_edge(g, arr[i][1], arr[i][0]);
	}	
	
	fclose(fp);
	return g;
}

void strongly_connencted_comp(struct graph *g)
{
	int i;

	for (i = 0; i < g->nvertices; i++) {
		if (g->array[i]->discovered == false) {
			dfs(g, i);
			printf("\n");
		}
	}
}

int main(int argc, char *argv[])
{
	struct graph *g;

	if (argc != 4) {
		fprintf(stderr, "Usage %s "
				"<edge_list_file_path> <number_of_vertices> <number_of_edges>\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}

	g = build_graph(argv[1], atoi(argv[2]), atoi(argv[3]));

	strongly_connencted_comp(g);

	destroy_graph(g);
	return 0;
}
