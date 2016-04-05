#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#define N 100

struct heap_node {
	int data;
	int key;
};

struct heap {
	int size;
	int cap;
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
alloc_heap_node(int key, int data) 
{
	struct heap_node *n;

	n = (struct heap_node *) alloc(sizeof(struct heap_node));

	n->key = key;
	n->data = data;

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
	h->array = (struct heap_node**) alloc(cap * sizeof(*(h->array)));

	for (i = 0; i < cap; i++) {
		h->array[i] = NULL;
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
	return ceil((i / 2.0) - 1);
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

void
min_heapify(struct heap *h, int i)
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
		swap_heap_node(&h->array[smallest], &h->array[i]);
		/* after swap smallest is pointing to the i */
		min_heapify(h, smallest);
	}
}

void
decrease_key(struct heap *h, int i, struct heap_node *n)
{
	if (h->array[i] != NULL && n->key > h->array[i]->key) {
		fprintf(stderr, "Error\n");
		exit(EXIT_FAILURE);
	}

	h->array[i] = n;

	while (i > 0 && h->array[parent(i)]->key > h->array[i]->key) {
		swap_heap_node(&h->array[i], &h->array[parent(i)]);
		i = parent(i);
	}

	print_queue(h);
}

int
search_index(struct heap *h, struct heap_node *n)
{
	int i;

	for (i = 0; i < h->size; i++) {
		if (h->array[i]->data == n->data)
			return i;
	}
	return h->size;
}

void
insert(struct heap *h, int key, int data)
{
    struct heap_node *n;

	h->size++;
	if (h->size > h->cap) {
		fprintf(stderr, "ERROR : heap storage limit exceeded\n");
		exit(EXIT_FAILURE);
	}

	n = (struct heap_node *) alloc_heap_node(key, data);
	decrease_key(h, h->size - 1, n);
}

struct heap_node *
extract_min(struct heap *h)
{
	struct heap_node *min;

	if (h->size <= 0) {
		fprintf(stderr, "ERROR : Heap Underflow\n");
		exit(EXIT_FAILURE);
	}

	min = h->array[0];
	h->array[0] = h->array[h->size - 1];
	h->size--;
	min_heapify(h, 0);

	return min;
}

void print_priority_queue(struct heap *h)
{
	struct heap_node *n;

	printf("%10s %10s\n", "KEY", "DATA");
	while (h->size) {
		n = extract_min(h);
		printf("%10d %10d\n", n->key, n->data);
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

	printf("%6s %10s %10s\n", "Adding", "KEY", "DATA");
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
