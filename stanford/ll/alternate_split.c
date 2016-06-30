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
alternate_split(struct node **sl, struct node **a, struct node **b)
{
	int flag;
	struct node *t;
	struct node *t1;
	struct node *t2;

	*a = NULL;
	*b = NULL;
    t1 = NULL;
	t2 = NULL;
	if (*sl == NULL)
		return;
	if ((*sl)->next == NULL) {
		*a = *sl;
	} else {
		for (flag = 1, t = *sl; t != NULL; flag = !flag) {
			if (flag) {
				if (t1 == NULL) {
					*a = t1 = t;
					t = t->next;
					t1->next = NULL;
				} else {
					t1->next = t;
					t = t->next;
					t1 = t1->next;
					t1->next = NULL;
				}
			} else {
				if (t2 == NULL) {
					*b = t2 = t;
					t = t->next;
					t2->next = NULL;
				} else {
					t2->next = t;
					t = t->next;
					t2 = t2->next;
					t2->next = NULL;
				} 
			}
		}
	}
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *a;
	struct node *b;
	struct node *sl;

	sl = NULL;
	alternate_split(&sl, &a, &b);
	print_ll(sl);
	print_ll(a);
	print_ll(b);

	delete(&sl);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *a;
	struct node *b;
	struct node *sl;

	sl = NULL;     
	append(&sl, 10);
	alternate_split(&sl, &a, &b);
	print_ll(sl);
	print_ll(a);
	print_ll(b);

	delete(&a);
	delete(&b);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	int i;
	struct node *a;
	struct node *b;
	struct node *sl;

	sl = NULL;     
	append(&sl, 10);
	append(&sl, 20);
	alternate_split(&sl, &a, &b);
	print_ll(sl);
	print_ll(a);
	print_ll(b);

	delete(&a);
	delete(&b);
	sl = NULL;     
	for (i = 0; i < 10; i++)
		append(&sl, i * 10);

	print_ll(sl);
	alternate_split(&sl, &a, &b);
	print_ll(sl);
	print_ll(a);
	print_ll(b);

	delete(&a);
	delete(&b);

	sl = NULL;     
	for (i = 0; i < 17; i++)
		append(&sl, i * 10);

	print_ll(sl);
	alternate_split(&sl, &a, &b);
	print_ll(a);
	print_ll(b);

	delete(&a);
	delete(&b);

	sl = NULL;     
	for (i = 0; i < 22; i++)
		append(&sl, i * 10);

	print_ll(sl);
	alternate_split(&sl, &a, &b);
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
