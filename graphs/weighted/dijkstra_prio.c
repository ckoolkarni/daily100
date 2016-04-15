#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

struct heap_node {
	int v;
	int key;
};

struct heap {
	int size;
	int cap;
	int *pos;
	struct graph *g;
	struct heap_node **array;
};

struct node {
	int v;
	int key;
	int dest;
	int weight;
	int parent;
	struct node *next;
	struct node *prev;
};

struct graph {
	int nvertices;
	bool directed;
	struct heap *h;
	struct node **array;
};

struct heap * pq_create_heap(struct graph *g, int cap);
void pq_destroy_heap(struct heap *h);

void
print_mst(struct graph *g)
{
	int i;
	int heap_size_flag;
    struct heap *h;

	h = g->h;
	heap_size_flag = 0;
	printf("\n%30s   |   %30s\n", "GRAPH", "HEAP");
	printf("%10s %10s %10s | %10s %10s %10s\n", 
								"V", "PARENT[]", "KEY[]", "V", "KEY", "POS");
	for (i = 0; i < h->cap; i++) {
		if (heap_size_flag == 0 && i >= h->size) {
			printf("______________________________________________________"
														"________________\n");
			heap_size_flag = 1;
		}
		printf("%10d %10d %10d | %10d %10d %10d\n", i, g->array[i]->v, 
				g->array[i]->key, h->array[i]->v, h->array[i]->key, h->pos[i]);
	}
}

void
print_graph(struct graph *g)
{
	int i;
	int flag;
	struct node *temp;

	assert(g != NULL);
	printf("--------------------%s--------------------\n",
			g->directed == true? "DIRECTED" : "UNDIRECTED");
	for (i = 0; i < g->nvertices; i++) {
		printf("HEAD-->");
		flag = 0;
		for (temp = g->array[i]; temp != NULL; temp = temp->next) {
			flag = 1;
			/*printf("[id %2d dest = %2d wt = %2d ]-->", temp->v,
					temp->dest, temp->weight);*/

			printf("[%2d %2d %2d]-->", temp->v,
					temp->dest, temp->weight);
		}
		printf("%s\n", flag == 1 ? "TAIL" : "NULL" );
	}
}

void *
alloc(size_t size)
{
	void *n;
	if ((n = malloc(size)) == NULL) {
		perror("malloc : ");
		exit(EXIT_FAILURE);
	}
	return n;
}

struct node *
alloc_node(int src, int dest, int weight)
{
	struct node *n;

	assert(src >= 0 && dest >= 0 && src != dest);

	n = (struct node *) alloc(sizeof(*n));

	n->v = src;
	n->key = INT_MAX;
	n->dest = dest;
	n->weight = weight;
	n->parent = INT_MAX;
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
    g = (struct graph *) alloc(sizeof (struct graph));

    g->h = pq_create_heap(g, nvertices); 
	g->array = (struct node **) alloc(nvertices * sizeof (struct node *));

    g->nvertices = nvertices;
	g->directed = false;
	for (i = 0; i < nvertices; i++) {
		g->array[i] = alloc_node(i, INT_MAX, INT_MAX);
	}

	return g;
}

void
add_edge(struct graph *g, int src, int dest, int weight)
{
	struct node *n;
	struct node *t;

	assert(g != NULL && src != INT_MAX && dest != INT_MAX);

	n = alloc_node(src, dest, weight);
	t = g->array[src];

	while (t->next != NULL) {
		if (n->v == t->v && n->dest == t->dest && n->weight == t->weight) {
			fprintf(stderr, "ERROR : "
					"duplicate edge src %d dest %d weight %d\n",
					n->v, n->dest, n->weight);
			exit(EXIT_FAILURE);
		}
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
	pq_destroy_heap(g->h);
	for (i = 0 ; i < g->nvertices; i++) {
		for (c = g->array[i]->next; c != NULL;
						t = c, c = c->next, free_node(t))
			;

		free_node(g->array[i]);
	}
	free(g->array);
	free(g);
}


struct heap_node *
pq_alloc_heap_node(int v, int key) 
{
	struct heap_node *n;

	n = (struct heap_node *) alloc(sizeof(struct heap_node));

	n->key = key;
	n->v = v;

	return n;
}

void
pq_free_heap_node(struct heap_node *n)
{
	if (n != NULL) {
		free(n);
		n = NULL;
	}
}

struct heap *
pq_create_heap(struct graph *g, int cap)
{
	int i;
	struct heap *h;

	h = (struct heap *) alloc(sizeof(struct heap));
	h->size = cap;
    h->cap = cap;
    h->pos = (int *) alloc(cap * sizeof(int));
	h->g = g;
	h->array = (struct heap_node**) alloc(cap * sizeof(*(h->array)));

	for (i = 0; i < cap; i++) {
		h->pos[i] = i;
		h->array[i] = pq_alloc_heap_node(i, INT_MAX);
	}
	return h;
}

void 
pq_destroy_heap(struct heap *h)
{
	int i;

	assert(h != NULL);
	for (i = 0; i < h->cap; i++) {
		pq_free_heap_node(h->array[i]);
	}

	free(h->array);
	free(h->pos);
	free(h);
}

void
pq_swap_heap_node(struct heap_node **a, struct heap_node **b)
{
	struct heap_node *t;

	t = *a, *a = *b, *b = t;
}

int 
pq_is_empty(struct heap *h)
{
    return h->size == 0;
}

int
pq_parent(int i)
{
	return ceil((i - 1)/ 2.0);
}

void
print_queue(struct heap *h)
{
	int i;

	printf("------------------------------\n");	
	for (i = 0; i < h->size; i++) {
		printf("[%d] = %d ", i, h->array[i]->key);
	}
	printf("------------------------------\n");	
}

int
pq_search_index(struct heap *h, struct heap_node *n)
{
	int i;

	for (i = 0; i < h->size; i++) {
		if (h->array[i]->v == n->v)
			return i;
	}
	return h->size;
}

int
pq_is_in_heap(struct heap *h, int v)
{
	return h->pos[v] < h->size;
}

void
pq_min_heapify(struct heap *h, int i)
{
	int l;
	int r;
	int smallest;

	l = 2 * i + 1;
	r = 2 * i + 2;
	smallest = i;

	if (l < h->size && h->array[l]->key < h->array[i]->key)
		smallest = l;

	if (r < h->size && h->array[r]->key < h->array[smallest]->key)
		smallest = r;

	if (smallest != i) {

		h->pos[h->array[i]->v] = smallest;
		h->pos[h->array[smallest]->v] = i;

		pq_swap_heap_node(&h->array[smallest], &h->array[i]);
		/* after swap smallest is pointing to the i */
		pq_min_heapify(h, smallest);
	}
}

void
pq_decrease_key(struct heap *h, int v, int key)
{
	int i;

	i = h->pos[v];
	
	h->array[i]->key = key;

	while (i > 0 && h->array[i]->key < h->array[pq_parent(i)]->key) {
		h->pos[h->array[i]->v] = pq_parent(i);
		h->pos[h->array[pq_parent(i)]->v] = i;
		pq_swap_heap_node(&h->array[i], &h->array[pq_parent(i)]);
		i = pq_parent(i);
	}
}

struct heap_node *
pq_extract_min(struct heap *h)
{
	struct heap_node *root;
	struct heap_node *last_node;

	if (pq_is_empty(h)) {
		fprintf(stderr, "ERROR : Heap Underflow\n");
		exit(EXIT_FAILURE);
	}
	
	root = h->array[0];
	last_node = h->array[h->size - 1];
	pq_swap_heap_node(&h->array[0], &h->array[h->size - 1]);
	h->pos[root->v] = h->size - 1;
	h->pos[last_node->v] = 0;

	--h->size;
	pq_min_heapify(h, 0);
	return root;
}

struct graph *
build_graph(char *fname, int nvertices, int nedges)
{
	int i;
	int src;
	int dest;
	int weight;
	FILE *fp;
	struct graph *g;

	if ((fp = fopen(fname, "r")) == NULL) {
		perror("ERROR : ");
		exit(EXIT_FAILURE);
	}

	g = create_graph(nvertices);
    for (i = 0; fscanf(fp, "%d %d %d", 
							&src, &dest, &weight) != EOF; i++) {
		printf("%10s %10s %10s\n", "SRC", "DEST", "WEIGHT");
		printf("%10d %10d %10d\n", src, dest, weight);
		add_edge(g, src, dest, weight);
		if (! g->directed)
			add_edge(g, dest, src, weight);
	
	}
	fclose(fp);
	return g;
}

void 
dijkstra(struct graph *g, int src)
{
	int i;
	int u;
	int v;
	struct heap *h;
	struct node *curr;
	struct heap_node *min;

	print_graph(g);

	h = g->h;
	g->array[src]->key = 0;
	pq_decrease_key(g->h, src, g->array[src]->key);

	while (! pq_is_empty(h)) {
		min = pq_extract_min(g->h);
		u = min->v;
        for (curr = g->array[u]->next; curr != NULL; curr = curr->next) {
			v = curr->dest;
			if (pq_is_in_heap(h, v) && curr->key != INT_MAX 
						&& curr->weight + g->array[u]->key < g->array[v]->key) {
				g->array[v]->key = curr->weight + g->array[u]->key;
				pq_decrease_key(h, v, g->array[v]->key);
			} 
		}
	}

	for (i = 1; i < g->nvertices; i++)
		printf("%d - %d\n", i, g->array[i]->key);
	
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

	dijkstra(g, 0);

	destroy_graph(g);
	return 0;
}
