#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node {
	int data;
	struct node *next;
};

void
print_ll(struct node *sl)
{
	while (sl) {
		printf("%d ", sl->data);
		sl = sl->next;
	}
	puts("X");
}

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
delete_node(struct node **sl)
{
    struct node *t;

	if (*sl == NULL)
		return NULL;

	t = *sl;
	*sl = (*sl)->next;
	t->next = NULL;
	return t;	
}

struct node *
sorted_merge(struct node *a, struct node *b)
{
	struct node *t;
	struct node *newlist;
	
	if (a == NULL && b == NULL)
		return NULL;

	if (a != NULL && b == NULL)
		return a;

	if (a == NULL && b != NULL)
		return b;

	if (a->data < b->data) {
		t = newlist = delete_node(&a);
	} else {
		t = newlist = delete_node(&b);
	}
	while (a != NULL && b != NULL) {
		t->next = (a->data < b->data) ? 
					delete_node(&a) : delete_node(&b);
		t = t->next;
	}
	if (a != NULL && b == NULL) {
		while (a != NULL) {
			t->next = delete_node(&a);
			t = t->next;
		}
	}
 	if (a == NULL && b != NULL) {
		while (b != NULL) {
			t->next = delete_node(&b);
			t = t->next;
		}
	}
	return newlist;
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *a;
	struct node *b;
	struct node *newlist;

	a = NULL;
	b = NULL;
	newlist = sorted_merge(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);

	delete(&newlist);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *a;
	struct node *b;
	struct node *newlist;

	a = NULL;
	b = NULL;
	append(&a, 10);
	newlist = sorted_merge(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	
	delete(&newlist);

	a = NULL;
	b = NULL;
	append(&b, 10);
	newlist = sorted_merge(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	
	delete(&newlist);

	a = NULL;
	b = NULL;
	append(&a, 10);
	append(&b, 20);
	newlist = sorted_merge(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	
	delete(&newlist);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	int i;
	struct node *a;
	struct node *b;
	struct node *newlist;

	a = NULL;
	b = NULL;
	for (i = 0; i < 5; i++)
		append(&a, i * 10);
	print_ll(a);
	print_ll(b);
	newlist = sorted_merge(a, b);
	print_ll(newlist);

	delete(&newlist);
	a = NULL;
	b = NULL;
	for (i = 0; i < 5; i++)
		append(&b, i * 10);
	print_ll(a);
	print_ll(b);
	newlist = sorted_merge(a, b);
	print_ll(newlist);

	delete(&newlist);
	a = NULL;
	b = NULL;
	for (i = 1; i <= 1; i++)
		append(&a, i * 10);
	for (i = 0; i < 5; i++)
		append(&b, i);
	print_ll(a);
	print_ll(b);
	newlist = sorted_merge(a, b);
	print_ll(newlist);

	delete(&newlist);
    
	a = NULL;
	b = NULL;
	for (i = 1; i <= 5; i++)
		append(&a, i * 10);
	for (i = 0; i < 1; i++)
		append(&b, i);
	print_ll(a);
	print_ll(b);
	newlist = sorted_merge(a, b);
	print_ll(newlist);

	delete(&newlist);

	a = NULL;
	b = NULL;
	for (i = 1; i <= 5; i++)
		append(&a, i * 10);
	for (i = 0; i < 5; i++)
		append(&b, i);
	print_ll(a);
	print_ll(b);
	newlist = sorted_merge(a, b);
	print_ll(newlist);

	delete(&newlist);

	a = NULL;
	b = NULL;
	for (i = 1; i <= 5; i++)
		append(&a, i * 10);
	for (i = 1; i <= 5; i++)
		append(&b, i * 10 + 5);
	print_ll(a);
	print_ll(b);
	newlist = sorted_merge(a, b);
	print_ll(newlist);

	delete(&newlist);
}


int main(int argc, char * argv[]) 
{
	//test_case_1();
    //test_case_2();
    test_case_3();
	return 0;
}
