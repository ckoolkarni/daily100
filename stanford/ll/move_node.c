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
		fprintf(stderr, "error : malloc : %s\n", strerror(errno));
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

void
move_node(struct node **a, struct node **b)
{
	struct node *t;

	if ((*a == NULL && *b == NULL) || (*a != NULL && *b == NULL))
		return;

   	t = *b;
   	*b = (*b)->next;
	t->next = *a == NULL ? NULL : *a;
	*a = t; 
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

void
test_case_1(void)
{
	struct node *a;
	struct node *b;
	
	a = NULL;
	b = NULL;
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	append(&a, 10);
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	append(&b, 10);
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);
	delete(&a);
	delete(&b);
}

void 
test_case_2(void) 
{
	struct node *a;
	struct node *b;
	
	a = NULL;
	b = NULL;

	append(&a, 20);
	append(&a, 30);
	append(&b, 10);
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;

	append(&a, 30);
	append(&b, 20);
	append(&b, 10);
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);
	delete(&a);
	delete(&b);
}

void 
test_case_3(void) 
{
	int i;
	struct node *a;
	struct node *b;
	
	a = NULL;
	b = NULL;

	append(&b, 10);
	append(&b, 20);
	append(&b, 30);
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);

	append(&b, 40);
	append(&b, 50);
	append(&b, 60);
	append(&b, 70);
    print_ll(a);
	print_ll(b);
	move_node(&a, &b);
	move_node(&a, &b);
	move_node(&a, &b);
    print_ll(a);
	print_ll(b);
	
	delete(&a);
	delete(&b);

	a = NULL;
	b = NULL;
	for (i = 0; i < 10; i++)
		append(&b, i * 10);

    print_ll(a);
	print_ll(b);
	for (i = 0; i < 11; i++)
		move_node(&a, &b);
    print_ll(a);
	print_ll(b);
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
