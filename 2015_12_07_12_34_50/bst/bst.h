#ifndef BST_H
#define BST_H 1

#include <stddef.h>

/* Function types */
typedef int bst_comparison_func(const void * bst_a, const void *bst_b,
				void *bst_cmp_param);
typedef void bst_item_func(void *bst_item, void *bst_item_param);
typedef void bst_copy_func(void *bst_item, void *bst_cpoy_param);

/* Memory Allocator */
#ifndef LIBAVL_ALLOCATOR
#define LIBAVL_ALLOCATOR
struct libavl_allocator {
	void* (*libavl_alloc)(struct libavl_allocator *, size_t s);
	void (*libavl_free)(struct libavl_allocator *, void *p);	
};
#endif /* LIBAVL_ALLOCATOR */

/* Default memory allcator */
struct libavl_allocator bst_allocator_default;
void* bst_malloc(struct libavl_allocator *, size_t s);
void bst_free(struct libavl_allocator *, void *p);

/* Maximum BST height */
#ifndef BST_MAX_HEIGHT
#define BST_MAX_HEIGHT 32
#endif /* BST_MAX_HEIGHT */

struct bst_node {
	struct bst_node *bst_link[2];
	void *bst_data;
};

struct bst_table {
	struct bst_node *bst_root;
	bst_comparison_func *bst_compare;
	void *bst_param;
	struct libavl_allocator *bst_alloc;
	size_t bst_count;
	unsigned long bst_generation;
};

/* Table function */
struct bst_table* bst_create(bst_comparison_func *bst_compare, void *bst_param,
				struct libavl_allocator *);
void* bst_insert(struct bst_table *table, void *item);
void* bst_find(const struct bst_table *table, void *item);
void* bst_replace(struct bst_table *, void*);
void* bst_delete(struct bst_table*, const void*);
void** bst_probe(struct bst_table *, void*);
struct bst_table* bst_copy(struct bst_table *, bst_copy_func *, bst_item_func *,
				struct libavl_allocator*);
void bst_destroy(struct bst_table, bst_item_func *bst_item);

#endif /* BST_H */
