#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

enum EDGE_TYPE {
	UNCLASSIFIED,
	TREE,
	BACK,
	FORWARD,
	CROSS
};

struct node {
	int id;
	int dest;
	int weight;
	int parent;
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
process_vertex_early(struct graph *g, int v)
{

}

void 
process_edge(struct graph *g, int x, int y)
{

}

void 
process_vertex_late(struct graph *g, int v)
{

}

enum EDGE_TYPE 
edge_classifiaction(struct graph *g, int x, int y)
{
	if (g->array[x]->parent == y) {
		printf("EDGE (%d %d) is TREE Edge\n", x, y);
		return TREE;
	}
	if (g->array[y]->discovered && (! g->array[y]->processed)) {
		printf("EDGE (%d %d) is BACK Edge\n", x, y);
		return BACK;
	}
	if (g->array[y]->processed && (
				g->array[y]->entry_time > g->array[x]->entry_time)) {
		printf("EDGE (%d %d) is FORWARD Edge\n", x, y);
   		return FORWARD;
	}
	if (g->array[y]->processed && (
				g->array[y]->entry_time < g->array[x]->entry_time)) {
		printf("EDGE (%d %d) is FORWARD Edge\n", x, y);
		return CROSS;
	}

	printf("ERROR EDGE (%d %d) is UNCLASSIFIED Edge\n", x, y);
	return UNCLASSIFIED;
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
		} else if (! g->array[curr->dest]->processed) {
			process_edge(g, src, curr->dest);
		}
		if (g->finished)
			return;
	}
	
	process_vertex_early(g, src);
	g->time++;
	t->exit_time = g->time;
	t->processed = true;
}

struct graph *
build_graph(char *fname, int nvertices)
{
	int x;
	int y;
	FILE *fp;
	struct graph *g;

	if ((fp = fopen(fname, "r")) == NULL) {
		perror("ERROR : ");
		exit(EXIT_FAILURE);
	}

	g = create_graph(nvertices);

	while (fscanf(fp, "%d %d", &x, &y) != EOF) {
		printf("x = %d y = %d\n", x, y);
		add_edge(g, x, y);
	}	
	
	fclose(fp);
	return g;
}

int main(int argc, char *argv[])
{
	struct graph *g;

	if (argc != 3) {
		fprintf(stderr, "Usage %s <edge_list_file_path> <number_of_vertices>\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}

	g = build_graph(argv[1], atoi(argv[2]));

	print_graph(g);
	dfs(g, 0);
	
	print_graph(g);

	destroy_graph(g);
	return 0;
}
