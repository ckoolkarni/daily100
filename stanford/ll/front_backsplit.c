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

void
front_back_split(struct node *sl, struct node **f, struct node **b)
{
	struct node *t1;
	struct node *t2;

	if (sl == NULL) {
		*f = NULL;
		*b = NULL;
		return;
	}

	if (sl->next == NULL) {
		*f = sl;
		b = NULL;
	} else if (sl->next->next == NULL) {
		*f = sl;
		sl = sl->next;
		(*f)->next = NULL;
		*b = sl;
	} else {
		*f = sl;
		t1 = sl;
		t2 = sl->next->next;
		while (1) {
			t1 = t1->next;
			if (t2->next == NULL || t2->next->next == NULL)
				break;
			t2 = t2->next->next;
		}
		*b = t1->next;
		t1->next = NULL;
	}
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *sl;
	struct node *f;
	struct node *b;

	sl = NULL;
	f = NULL;
	b = NULL;
	front_back_split(sl, &f, &b);
	print_ll(sl);
	print_ll(f);
	print_ll(b);
	delete(&sl);
	delete(&f);
	delete(&b);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *sl;
	struct node *f;
	struct node *b;

	sl = NULL;
	f = NULL;
	b = NULL;

	append(&sl, 10);
	front_back_split(sl, &f, &b);
	print_ll(sl);
	print_ll(f);
	print_ll(b);
	delete(&f);
	delete(&b);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	int i;
	struct node *sl;
	struct node *f;
	struct node *b;

	sl = NULL;
	f = NULL;
	b = NULL;

	append(&sl, 10);
	append(&sl, 20);

	front_back_split(sl, &f, &b);
	print_ll(sl);
	print_ll(f);
	print_ll(b);
	delete(&f);
	delete(&b);

	sl = NULL;
	f = NULL;
	b = NULL;

	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);

	front_back_split(sl, &f, &b);
	print_ll(sl);
	print_ll(f);
	print_ll(b);
	delete(&f);
	delete(&b);

	sl = NULL;
	f = NULL;
	b = NULL;

	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 40);

	front_back_split(sl, &f, &b);
	print_ll(sl);
	print_ll(f);
	print_ll(b);
	delete(&f);
	delete(&b);

	sl = NULL;
	f = NULL;
	b = NULL;

	for (i = 0; i < 11; i++)
		append(&sl, i * 10);
    printf("Original list is :- \n");
    print_ll(sl);
	front_back_split(sl, &f, &b);
	print_ll(sl);
    printf("Front list is :- \n");
	print_ll(f);
    printf("Back list is :- \n");
	print_ll(b);
	delete(&f);
	delete(&b);

}


int main(int argc, char * argv[]) 
{
	test_case_1();
	test_case_2();
	test_case_3();
	return 0;
}
