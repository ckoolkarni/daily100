#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

struct adjl_node {
	int dest;
	int weight;
	struct adjl_node *next;
};

struct adj_list {
	struct adjl_node *head;
};

struct graph {
	int num_vertex;
	struct adj_list *array;

};

struct adjl_node*
get_adjl_node(int dest, int weight)
{
	struct adjl_node *n;

	if ((n = (struct adjl_node*) malloc(sizeof (*n))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
	n->dest = dest;
	n->weight = weight;

	return n;
}

struct graph *
create_graph(int num_vertex)
{
	int i;
	struct graph *g;

	if ((g = (struct graph*) malloc(sizeof (*g))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}
    g->num_vertex = num_vertex;
	if ((g->array = (struct adj_list*)malloc(
					num_vertex * sizeof (g->array))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	for (i = 0; i < num_vertex; i++) {
		g->array[i].head = NULL;
	}
	
	return g;
}

void
add_edge(struct graph *g, int src, int dest, int weight)
{
	struct adjl_node *n;

	assert(g != NULL);
	if ((n = get_adjl_node(dest, weight)) == NULL)
		exit(EXIT_FAILURE);


	n->next = g->array[src].head;
	g->array[src].head->next = n;

	if ((n = get_adjl_node(src, weight)) == NULL)
		exit(EXIT_FAILURE);
	n->next = g->array[dest].head;
	g->array[dest].head = n;
}

struct min_heap_node {
	int v;
	int dist;
};

struct min_heap {
	int size;
	int cap;
    int *pos;
	struct min_heap_node **array;
};

struct min_heap_node *
new_min_heap_node(int v, int dist)
{
	struct min_heap_node *n;

	if ((n = (struct min_heap_node *) malloc(sizeof (*n))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

    n->v = v;
    n->dist = dist;
	return n;
}

struct min_heap *
create_min_heap(int cap)
{
	struct min_heap *mh;

	if ((mh = malloc(sizeof (*mh))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	mh->size = 0;
	mh->cap = cap;
	if ((mh->pos = (int *) malloc(cap * sizeof (int))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	if ((mh->array = (struct min_heap_node **) malloc(
						cap * sizeof (struct min_heap_node *))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	return mh;
}	

void
swap_min_heap_node(struct min_heap_node **a, struct min_heap_node **b)

{
	struct min_heap_node *t;

	assert(a != NULL && b != NULL);

	t = *a, *a = *b, *b = t;
}

void
min_heapify(struct min_heap *mh, int idx)
{
	int left;
	int right;
	int smallest;
	struct min_heap_node *idx_node;
    struct min_heap_node *smallest_node;

	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < mh->size && mh->array[left]->dist < mh->array[idx]->dist)
		smallest = left;
	else 
		smallest = idx;

	if (right < mh->size && mh->array[right]->dist < mh->array[idx]->dist)
		smallest = right;
	
	if (idx != smallest) {
		idx_node = mh->array[idx];
		smallest_node = mh->array[smallest];
	
		mh->pos[idx_node->v] = smallest;
		mh->pos[smallest_node->v] = idx;

		swap_min_heap_node(&mh->array[smallest], &mh->array[idx]);
		min_heapify(mh, smallest);
	}
}

int
is_empty(struct min_heap *mh)
{
	return mh->size == 0;
}

struct min_heap_node *
extract_min(struct min_heap *mh)
{
	struct min_heap_node *root;
    struct min_heap_node *last_node;

	root = mh->array[0];
    last_node = mh->array[mh->size - 1];
	mh->array[0] = last_node;
	--mh->size;
	min_heapify(mh, 0);

	return root;
}


void
decrease_key(struct min_heap *mh, int v, int dist)
{
	int i;
    int parent_i;

	
	i = mh->pos[v];
    parent_i = (i - 1) / 2;

	mh->array[i]->dist = dist;
	
	while (i && mh->array[i]->dist < mh->array[parent_i]->dist) {
		mh->pos[mh->array[i]->v] = parent_i;
		mh->pos[mh->array[parent_i]->v] = i;
		swap_min_heap_node(&mh->array[i], &mh->array[parent_i]);
	}
}

int
is_min_heap(struct min_heap *mh, int v)
{
	return mh->pos[v] < mh->size;
}

// A utility function used to print the solution
void 
print_arr(int dist[], int n)
{
    int i;

    printf("Vertex   Distance from Source\n");
    for (i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
 
int
main(int argc, char *argv[])
{
    int i;
	int v = 9;
	int dist[v];
    struct graph *graph; 
    struct min_heap* mh;
    
	if ((mh = create_min_heap(v)) == NULL)
		return -1;

	if ((graph = create_graph(v)) == NULL)
		return -1;


    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 7, 8);
    add_edge(graph, 1, 2, 8);
    add_edge(graph, 1, 7, 11);
    add_edge(graph, 2, 3, 7);
    add_edge(graph, 2, 8, 2);
    add_edge(graph, 2, 5, 4);
    add_edge(graph, 3, 4, 9);
    add_edge(graph, 3, 5, 14);
    add_edge(graph, 4, 5, 10);
    add_edge(graph, 5, 6, 2);
    add_edge(graph, 6, 7, 1);
    add_edge(graph, 6, 8, 6);
    add_edge(graph, 7, 8, 7);

	if (create_min_heap(v) == NULL)
		return -1;

    for (i = 0; i < v; ++i) {
        dist[i] = INT_MAX;
        mh->array[i] = new_min_heap_node(i, dist[i]);
        mh->pos[i] = i;
    }
	print_arr();
	return 0;
}
