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
get_nth(struct node *s, int n)
{
	int i;
	struct node *t;

	assert(n > 0);
	if (s == NULL) {
		return NULL;
	}

	for (i = 1, t = s; t != NULL && i < n; i++, t = t->next)
		;

	if (i == n && t != NULL) 	
		return t;

	return NULL;
}

/* Empty Linked List */
void
test_case_1(void)
{
	int n;
	struct node *sl;
	struct node *nth;

	n = 1;
	sl = NULL;
	nth = get_nth(sl, n);
	if (nth != NULL) {
		printf("%s FAIL\n", __func__);
	} 
}

/* Single Node */
void 
test_case_2(void) 
{
	int n;
	struct node *sl;
	struct node *nth;

	n = 0;
	sl = NULL;

	append(&sl, 10);

	nth = get_nth(sl, 1);
	if (nth == NULL) {
		printf("%s FAIL\n", __func__);
	}
	nth = get_nth(sl, 2);
	if (nth != NULL) {
		printf("%s FAIL\n", __func__);
	}

	nth = get_nth(sl, 3);
	if (nth != NULL) {
		printf("%s FAIL\n", __func__);
	}

}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	int n;
	struct node *sl;
	struct node *nth;

	n = 0;
	sl = NULL;

	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 40);
	append(&sl, 50);
	append(&sl, 60);

	nth = get_nth(sl, 1);
	if (nth == NULL)
		printf("%s FAIL\n", __func__);
		
	if (nth->data != 10)
		printf("%s FAIL\n", __func__);
}


int main(int argc, char * argv[]) 
{

	test_case_1();
	test_case_2();
	test_case_3();
	return 0;
}
