#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/* bst compare function */
typedef int (*bst_cmp_func_t)(void *, void *);

/* bst node */
struct bst_node {
    void *bst_data;
    struct bst_node *bst_link[2];
};

/* bst */
struct bst_table {
    struct bst_node *bst_root;
    bst_cmp_func_t bst_cmp;
    void *bst_cmp_param;
    unsigned int bst_count;
};

void
bst_print(const struct bst_node *node)
{
    if (node == NULL)
        return;

    fprintf(stderr, "%p %d\n", node->bst_data, *(int *)node->bst_data);
    if (node->bst_link[0] != NULL || node->bst_link[1] != NULL) {
        fprintf(stderr, "(");
        bst_print(node->bst_link[0]);
        if (node->bst_link[1] != NULL) {
            fprintf(stderr, ",");
            bst_print(node->bst_link[1]);
        }
        fprintf(stderr, ")");
    }
}

struct bst_node *
get_node(void *data)
{
    struct bst_node *n;

    if ((n = (struct bst_node *) malloc(sizeof(struct bst_node *))) == NULL) {
        fprintf(stderr, "ERROR : malloc : %s\n", strerror(errno));
        return 0;
    }
    n->bst_data =  data;
    n->bst_link[0] = n->bst_link[1] = NULL;
    fprintf(stderr, "%s %p %d data %p\n", __func__, n->bst_data, *(int *) n->bst_data, data);
    return n;
}

struct bst_table *
bst_create(bst_cmp_func_t bst_cmp)
{
    struct bst_table *t;

    assert(bst_cmp != NULL);

    if ((t = (struct bst_table *) malloc(sizeof(struct bst_table))) == NULL) {
        fprintf(stderr, "ERROR : malloc : %s\n", strerror(errno));
        return NULL;
    }
    t->bst_root = NULL;
    t->bst_cmp = bst_cmp;
    t->bst_cmp_param = NULL;
    t->bst_count = 0;

    return t;
}

void
bst_insert(struct bst_table *t, void *item)
{
    int dir;
    int cmp;
    struct bst_node *n;
    struct bst_node *p;
    struct bst_node *q;

    assert(t != NULL && item != NULL);

    for (q = NULL, p = t->bst_root; p != NULL; q = p, p = p->bst_link[dir]) {
        cmp = t->bst_cmp(item, p->bst_data);
        if (cmp == 0) {
            return;
        }
        dir = cmp > 0;
    }

    n = get_node(item);
    if (n == NULL)
        return;

    if (q == NULL) {
        t->bst_root = n;
        //fprintf(stderr, "%s t->root->bst_data %p\n", __func__, t->bst_root->bst_data);
    } else {
        q->bst_link[dir] = n;
        fprintf(stderr, "%s n->bst_data %p item %p\n", __func__, n->bst_data, item);
        fprintf(stderr, "%s q->bst_link[%d]-> bst_data %p\n", __func__, dir,
                q->bst_link[dir]->bst_data);
    }
    t->bst_count++;
}

int bst_int_cmp(void *a, void *b)
{
    int ret;

    assert(a != NULL && b != NULL);

    ret = 0;
    if (*(int *)a > *(int *)b) {
        ret = 1;
    } else if (*(int *)a < *(int *)b) {
        ret = -1;
    }

    return ret;
}

int main(void)
{
    int i;
    int arr[3] = {50, 20, 30}; 
    struct bst_table *t;

    for (i = 0; i < 3; i++)
        fprintf(stderr, "%p\n", &arr[i]);
    t = bst_create(bst_int_cmp);

    bst_insert(t, &arr[0]);
    bst_insert(t, &arr[1]);
    bst_insert(t, &arr[2]);
    
//    bst_print(t->bst_root);

    return 0;
}
