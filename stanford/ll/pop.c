
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
pop(struct node **sl)
{
	int i;
	struct node *t;

	if (*sl == NULL) {
		return NULL;
	}
	
	t = *sl;
	*sl = t->next;
	
	return t;
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *sl;

	sl = NULL;
	pop(&sl);

	if (sl != NULL)
		printf("%s FAIL\n", __func__);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *sl;
	struct node *nth;

	sl = NULL;
	append(&sl, 10);

	nth = pop(&sl);
	if (nth == NULL || nth->data != 10 || sl != NULL)
		printf("%s FAIL\n", __func__);

	free(nth);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	int i;
	int n;
	struct node *sl;
	struct node *nth;

	n = 0;
	sl = NULL;

	for (i = 1; i < 6; i++) {
		append(&sl, i * 10);
	}
    for (i = 1; i < 6 ; i++) {
		nth = pop(&sl);
		if (nth == NULL || nth->data != (i * 10))
			printf("%s FAIL\n", __func__);
		free(nth);
	}
    if (sl != NULL)
		printf("%s FAIL\n", __func__);
}


int main(int argc, char * argv[]) 
{

	test_case_1();
	test_case_2();
	test_case_3();
	return 0;
}
