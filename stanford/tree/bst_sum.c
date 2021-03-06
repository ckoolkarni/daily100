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

	/*fprintf(stderr, " %s %p %d\n", __func__, 
	node->bst_data, *(int *)node->bst_data);*/
	fprintf(stderr, "%d", *(int *)node->bst_data);
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
get_node(void *bst_data)
{
    struct bst_node *n;

    if ((n = (struct bst_node *) malloc(sizeof(struct bst_node))) == NULL) {
        fprintf(stderr, "ERROR : malloc : %s\n", strerror(errno));
        return 0;
    }
    n->bst_data = bst_data;
    n->bst_link[0] = n->bst_link[1] = NULL;
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
    } else {
        q->bst_link[dir] = n;
    }
    t->bst_count++;
}

void
bst_delete(struct bst_table *t)
{
	struct bst_node *p;
	struct bst_node *q;

	if (t == NULL || t->bst_root == NULL)
		return;

	for (p = t->bst_root; p != NULL; p = q) {
		if (p->bst_link[0] != NULL) {
			q = p->bst_link[0];
			p->bst_link[0] = q->bst_link[1];
			q->bst_link[1] = p;
		} else {
			q = p->bst_link[1];
			free(p);
		}
	}
}

void
bst_print_sum(struct bst_node *r, int sum, int curr_sum)
{
	if (r == NULL) {
		return;
	}

	curr_sum += *(int *) r->bst_data;

	if (r->bst_link[0] == NULL && r->bst_link[1] == NULL) {
		if (sum == curr_sum)
			fprintf(stderr, "Sum found at %d\n", *(int *) r->bst_data);
	} else {
		if (curr_sum < sum) {
			bst_print_sum(r->bst_link[0], sum, curr_sum);
			bst_print_sum(r->bst_link[1], sum, curr_sum);
		}
	}
}

void 
bst_print_path_sum(struct bst_table *t, int sum)
{
	if (t == NULL)
		return;

	bst_print_sum(t->bst_root, sum, 0);
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

void
test_case_1(void)
{
	int sum;
    struct bst_table *t;

    t = NULL;
    sum = 0;
    bst_print_path_sum(t, sum);
	bst_delete(t);
    free(t);
    t = bst_create(bst_int_cmp);
	sum = 0;
    bst_print_path_sum(t, sum);
	bst_delete(t);
    free(t);
}

void
test_case_2(void)
{
	int sum;
	int arr[1] = {50}; 
    struct bst_table *t;

    t = bst_create(bst_int_cmp);
    bst_insert(t, &arr[0]);
    bst_print(t->bst_root);
    putchar('\n');
	sum = 100;
    bst_print_path_sum(t, sum);
	sum = 50;
    bst_print_path_sum(t, sum);
	bst_delete(t);
    free(t);
}

void
test_case_3_1(void)
{
	int i;
	int sum;
	int arr[7] = {50, 25, 75, 12, 35, 65, 85}; 
    struct bst_table *t;

    t = bst_create(bst_int_cmp);
	for (i = 0; i < 7; i++) {
		bst_insert(t, &arr[i]);
	}
    bst_print(t->bst_root);
    putchar('\n');
    sum = 100;
    bst_print_path_sum(t, sum);
	sum = 110;
	bst_print_path_sum(t, sum);
    sum = 75;
    bst_print_path_sum(t, sum);
	bst_delete(t);
    free(t);
}

void
test_case_3_2(void)
{
	int i;
	int sum;
	int arr[8] = {50, 20, 15, 10, 3, 2, 30, 100 }; 
    struct bst_table *t;

    t = bst_create(bst_int_cmp);
	for (i = 0; i < 9; i++) {
		bst_insert(t, &arr[i]);
	}
    bst_print(t->bst_root);
    putchar('\n');
    sum = 100;
    bst_print_path_sum(t, sum);
	bst_delete(t);
    free(t);
}


void
test_case_3(void)
{
	test_case_3_1();
	test_case_3_2();
}

int main(void)
{
	//test_case_1();
	//test_case_2();
	test_case_3();

    return 0;
}
