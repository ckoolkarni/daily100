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

struct node *
get_tail(struct node *sl) 
{
	if (sl == NULL)
		while (sl->next != NULL)
			sl = sl->next;

	return sl;
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

void
process_lists(struct node **a, struct node **b, struct node **newlist)
{
	int n;
    struct node *t;

	while (*a != NULL && *b != NULL) {
		if ((*a)->data == (*b)->data) {
			n = (*a)->data;
			append(newlist, n);

			*a = (*a)->next;
			while (*a && (*a)->data == n) {
				*a = (*a)->next;
			}
			*b = (*b)->next;
			while (*b && (*b)->data == n) {
				*b = (*b)->next;
			}
		} else {
			if ((*a)->data > (*b)->data) {
				append(newlist, (*b)->data);
				*b = (*b)->next;
			} else {
				append(newlist, (*a)->data);
				*a = (*a)->next;
			}
		}
	}

    if (*a != NULL) {
		assert(*b == NULL);
		while (*a != NULL) {
			n = (*a)->data;
			t = get_tail(*newlist);
			if (t->data != n) {
				append(newlist, n);
			}
			*a = (*a)->next;
			while (*a && (*a)->data == n) {
				*a = (*a)->next;
			}
		}
	} else {
		assert(*a == NULL);
		while (*b != NULL) {
			n = (*b)->data;
			t = get_tail(*newlist);
			if (t->data != n) {
				append(newlist, n);
			}
			*b = (*b)->next;
			while (*b && (*b)->data == n) {
				*b = (*b)->next;
			}
		}
	}
}

struct node *
sorted_union(struct node *a, struct node *b)
{
	int n;
	struct node *t;
	struct node *newlist;

	t = NULL;
	newlist = NULL;
	if (a == NULL && b == NULL)
		return;

	if (a != NULL && b == NULL)
		t = a;
	else if (a == NULL && b != NULL)
		t = b;

	if (t) {
		while (t != NULL) {
			append(&newlist, t->data);
			t = t->next;
		}
	} else {
		process_lists(&a, &b, &newlist);
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
	 newlist = NULL;
	 newlist = sorted_union(a, b);
	 print_ll(a);
	 print_ll(b);
	 print_ll(newlist);
	 delete(&newlist);
	 delete(&a);
	 delete(&b);
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
	newlist = NULL;

	append(&a, 10);
	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

 	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&b, 10);
	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);   

	a = NULL;
	b = NULL;
	newlist = NULL;
	append(&a, 10);
	append(&b, 20);
	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 20);
	append(&b, 10);
	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

 	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&b, 10);
	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b); 
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	struct node *a;
	struct node *b;
	struct node *newlist;

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);
	append(&a, 30);
	append(&a, 40);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&b, 10);
	append(&b, 20);
	append(&b, 30);
	append(&b, 40);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);

	append(&b, 30);
	append(&b, 40);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);
	append(&b, 10);
	append(&b, 40);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);
	append(&b, 5);
	append(&b, 10);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);
	append(&b, 10);
	append(&b, 20);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 10);
	append(&b, 10);
	append(&b, 10);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);
	append(&b, 15);
	append(&b, 20);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	newlist = NULL;

	append(&a, 10);
	append(&a, 20);
	append(&a, 20);
	append(&a, 20);
	append(&a, 30);
	append(&a, 60);
	append(&a, 70);

	append(&b, 10);
	append(&b, 20);
	append(&b, 20);
	append(&b, 20);
	append(&b, 35);
	append(&b, 70);
	append(&b, 80);

	newlist = sorted_union(a, b);
	print_ll(a);
	print_ll(b);
	print_ll(newlist);
	delete(&newlist);
	delete(&a);
	delete(&b);
}


int main(int argc, char * argv[]) 
{
	test_case_1();
	test_case_2();
	test_case_3();
	return 0;
}
