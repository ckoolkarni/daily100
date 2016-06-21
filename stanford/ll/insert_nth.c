#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node {
	int data;
	struct node *next;
};

struct node *
get_node(int data)
{
	struct node *n;

	n = (struct node *) malloc(sizeof (struct node));
	if (n == NULL) {
		fprintf(stderr, "ERROR : malloc : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	n->data = data;
	n->next = NULL;
	return n;
}

void 
append(struct node **sl, int data)
{
	struct node *n;
    struct node *t;

	n = get_node(data);

	if (*sl == NULL) {
		*sl = n;
	} else {
		for (t = *sl; t->next != NULL; t = t->next)
			;
		t->next = n;
	}
}

void
delete(struct node **sl)
{
	struct node *t;
	struct node *temp;
	
	if (*sl == NULL)
		return;

	t = *sl;

	for (temp = t->next; temp != NULL; temp = temp->next) {
		free(t);
		t = temp;
	}

	free(t);
	*sl = NULL;
}

struct node *
insert_nth(struct node **sl, int pos, int data)
{
	int i;
	struct node *t;
	struct node *n;

	assert(pos > 0);
	if (*sl == NULL && pos > 1)
		return NULL;

	n = get_node(data);
    
	if (pos == 1) {
		if (*sl == NULL) {
			*sl = n;
		} else {
			assert(*sl != NULL);
			n->next = *sl;
			*sl = n;
		}
   		return n;
	}
	
	for (t = *sl, i = 1; t != NULL && i < (pos - 1); t = t->next, i++)
		;

	if (t != NULL && i == (pos - 1)) {
		n->next = t->next;
		t->next = n;
		return n;
	}
    free(n);
	return NULL;
}

void
print_ll(struct node *sl)
{
	while (sl) {
		printf("%d ", sl->data);
		sl = sl->next;
	}
	puts("X");
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *t;
	struct node *sl;

	sl = NULL;

	t = insert_nth(&sl, 2, 10);
	if (t != NULL)
		printf("%s FAIL\n", __func__);

	t = insert_nth(&sl, 1, 10);
	if ( t == NULL || t->data != 10)
		printf("%s FAIL\n", __func__);
	delete(&sl);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *t;
	struct node *sl;

	sl = NULL;
	printf("++ %s\n", __func__);
	t = insert_nth(&sl, 1, 10);
	if (t == NULL || t->data != 10 && t->next != NULL)
		printf("%s FAIL\n", __func__);
	print_ll(sl);
	
	t = insert_nth(&sl, 2, 30);
    if (t == NULL || t->data != 30)
        printf("%s FAIL\n", __func__);
	print_ll(sl);

	t = insert_nth(&sl, 2, 20);
    if (t == NULL || t->data != 20 || t->next->data != 30)
        printf("%s FAIL\n", __func__);
	print_ll(sl);

	t = insert_nth(&sl, 5, 50);
	if (t != NULL)
		printf("%s FAIL\n", __func__);

	t = insert_nth(&sl, 4, 40);
    if (t == NULL || t->data != 40 || t->next != NULL)
        printf("%s FAIL\n", __func__);

	print_ll(sl);

	delete(&sl);
	printf("-- %s\n", __func__);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	struct node *sl;

	sl = NULL;

	printf("++ %s\n", __func__);
	insert_nth(&sl, 1, 50);
	print_ll(sl);
	insert_nth(&sl, 1, 40);
	print_ll(sl);
	insert_nth(&sl, 1, 30);
	print_ll(sl);
	insert_nth(&sl, 1, 20);
	print_ll(sl);
	insert_nth(&sl, 1, 10);
	print_ll(sl);
	delete(&sl);
	printf("-- %s\n", __func__);
}


int main(int argc, char * argv[]) 
{

	test_case_1();
	test_case_2();
	test_case_3();
	return 0;
}
