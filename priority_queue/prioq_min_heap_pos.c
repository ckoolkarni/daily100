#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

struct heap_node {
	int v;
	int key;
};

struct heap {
	int size;
	int cap;
	int *pos;
	struct heap_node **array;
};

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

struct heap_node *
alloc_heap_node(int v, int key) 
{
	struct heap_node *n;

	n = (struct heap_node *) alloc(sizeof(struct heap_node));

	n->v = v;
	n->key = key;

	return n;
}

void
free_heap_node(struct heap_node *n)
{
	if (n != NULL)
		free(n);
}

struct heap *
create_min_heap(int cap)
{
	int i;
	struct heap *h;

	h = (struct heap *) alloc(sizeof(struct heap));
	h->size = 0;
    h->cap = cap;
	h->pos = (int *) alloc(cap * sizeof(int));
	h->array = (struct heap_node**) alloc(cap * sizeof(*(h->array)));

	for (i = 0; i < cap; i++) {
		h->array[i] = NULL;
		h->pos[i] = i;
	}
	return h;
}

void destroy_min_heap(struct heap *h)
{

	int i;

	assert(h != NULL);
	for (i = 0; i < h->cap; i++) {
		free_heap_node(h->array[i]);
	}
	free(h->array[i]);
}

void
swap_heap_node(struct heap_node **a, struct heap_node **b)
{
	struct heap_node *t;

	t = *a, *a = *b, *b = t;
}

int 
is_empty(struct heap *h)
{
    return h->size == 0;
}

int
parent(int i)
{
	return ceil((i - 1) / 2.0);
}

void
print_queue(struct heap *h)
{
	int i;

	printf("--------------POS---------------\n");	
	for (i = 0; i < h->cap; i++) {
		printf("|pos [%d] = %d", i, h->pos[i]);
	}
	printf("\n--------------ARR----------------\n");	
	for (i = 0; i < h->size; i++) {
		printf("|v %d = key %d", h->array[i]->v, h->array[i]->key);
	}
	printf("\n------------------------------\n");	
}

void
min_heapify(struct heap *h, int i)
{
	int l;
	int r;
	int smallest;

	l = 2 * i + 1;
	r = 2 * i + 2;
	smallest = i;

	if (l < h->size && h->array[l]->key < h->array[smallest]->key)
		smallest = l;

	if (r < h->size && h->array[r]->key < h->array[smallest]->key)
		smallest = r;

	if (smallest != i) {

		h->pos[h->array[smallest]->v] = i;
		h->pos[h->array[i]->v] = smallest;

		swap_heap_node(&h->array[smallest], &h->array[i]);
		/* after swap smallest is pointing to the i */
		min_heapify(h, smallest);
	}
}

void
decrease_key(struct heap *h, int v, struct heap_node *n)
{
	int i;

	h->array[v] = n;
	i = h->pos[v];

	while (i > 0 && h->array[i]->key < h->array[parent(i)]->key) {
		h->pos[h->array[i]->v] = parent(i);
		h->pos[h->array[parent(i)]->v] = i;

		swap_heap_node(&h->array[i], &h->array[parent(i)]);
		i = parent(i);
	}

	print_queue(h);
}

struct heap_node *
extract_min(struct heap *h)
{
	struct heap_node *root;

	if (is_empty(h)) {
		fprintf(stderr, "ERROR : Heap Underflow\n");
		exit(EXIT_FAILURE);
	}

	root = h->array[0];
	h->array[0] = h->array[h->size - 1];

	h->pos[root->v] = h->size - 1;
	h->pos[h->size - 1] = 0;

	h->size--;
	min_heapify(h, 0);

	return root;
}

int
search_index(struct heap *h, struct heap_node *n)
{
	int i;

	for (i = 0; i < h->size; i++) {
		if (h->array[i]->v == n->v)
			return i;
	}
	return h->size;
}

void
insert(struct heap *h, int v, int key)
{
    struct heap_node *n;

	h->size++;
	if (h->size > h->cap) {
		fprintf(stderr, "ERROR : heap storage limit exceeded\n");
		exit(EXIT_FAILURE);
	}

	n = (struct heap_node *) alloc_heap_node(v, key);
	decrease_key(h, h->size - 1, n);
}


void print_priority_queue(struct heap *h)
{
	struct heap_node *n;

	printf("%10s %10s\n", "V", "KEY");
	while (h->size) {
		n = extract_min(h);
		printf("%10d %10d\n", n->v, n->key);
		free_heap_node(n);
	}
}

struct heap *
build_priority_queue(char *fname, int heap_cap)
{
	int i;
	int arr[heap_cap][2];
	FILE *fp;
	struct heap *h;

	if ((fp = fopen(fname, "r")) == NULL) {
		perror("ERROR : ");
		exit(EXIT_FAILURE);
	}

	h = create_min_heap(heap_cap);

	printf("%6s %10s %10s\n", "Adding", "V", "KEY");
	for (i = 0; fscanf(fp, "%d %d", &arr[i][0], &arr[i][1]) != EOF; i++) {
		printf("%6s %10d %10d\n", "Adding", arr[i][0], arr[i][1]);
		insert(h, arr[i][0], arr[i][1]);
	}	
	
	fclose(fp);
	return h;
}


int
main(int argc, char * argv[])
{
	struct heap *h;

	if (argc != 3) {
		fprintf(stderr, "Usage %s <input_file_path> <heap_size>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	h = build_priority_queue(argv[1], atoi(argv[2]));

	print_priority_queue(h);
	return 0;
}
