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
front_back_split(struct node **sl, struct node **a, struct node **b)
{
	struct node *t;
	struct node *t1;

	*a = NULL;
	*b = NULL;

	if (*sl == NULL) {
		return;
	} if ((*sl)->next == NULL) {
		*a = *sl;
	} else if ((*sl)->next->next == NULL) {
		*a = *sl;
		*sl = (*sl)->next;
		*b = *sl;
		(*a)->next = (*b)->next = NULL;
		*sl = NULL;
	} else {
		t = *sl;
		t1 = t->next->next;
		while (1) {
			t = t->next;
			if (t1->next == NULL || t1->next->next == NULL) {
				break;
			}
			t1 = t1->next->next;
		}
		*a = *sl;
		*b = t->next;
		t->next = NULL;
	}
	*sl = NULL;
}

struct node *
sorted_merge(struct node **a, struct node **b)
{
	struct node *t;
	struct node *ta;
	struct node *tb;
	struct node *newlist;

	if (*a == NULL && *b == NULL)
		return;

	if (*a != NULL && *b == NULL) {
		newlist = *a;
	} else if (*a == NULL && *b != NULL) {
		newlist = *b;
	} else {
		ta = *a;
		tb = *b;

		if (ta->data > tb->data) {
			newlist = t = tb;
			tb = tb->next;
		} else {
			newlist = t = ta;
			ta = ta->next;
		}
		t->next = NULL;

		while (ta != NULL && tb != NULL) {
			if (ta->data > tb->data) {
				t->next = tb;
				tb = tb->next;
			} else {
				t->next = ta;
				ta = ta->next;
			}
			t = t->next;
			t->next = NULL; 
		}

		if (ta != NULL) {
			t->next = ta;
		} else {
			assert(tb != NULL);
			t->next = tb;
		}
	}
	*a = *b = NULL;
	return newlist;
}

void 
merge_sort(struct node **sl)
{
	struct node *a;
	struct node *b;

	if (*sl == NULL || (*sl)->next == NULL) {
		return;
	}

	a = NULL;
	b = NULL;
	front_back_split(sl, &a, &b);
    
	merge_sort(&a);
	merge_sort(&b);
 	
	*sl = sorted_merge(&a, &b);
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *sl;

	sl = NULL;
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *sl;

	sl = NULL;
	append(&sl, 10);
	printf("Sorted merge :- \n");
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);

 	sl = NULL;

	append(&sl, 10);
	append(&sl, 20);
	printf("Sorted merge :- \n");
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);


 	sl = NULL;
	append(&sl, 20);
	append(&sl, 20);
	printf("Sorted merge :- \n");
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);
}

/* Multiple Nodes */
void 
test_case_3_1(void) 
{
	struct node *sl;

	sl = NULL;
	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);

	printf("Sorted merge :- \n");
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);

}

void 
test_case_3_2(void) 
{
	struct node *sl;

	sl = NULL;

	append(&sl, 40);
	append(&sl, 20);
	append(&sl, 10);
	append(&sl, 30);

	printf("Sorted merge :- \n");
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);
}

void 
test_case_3_3(void) 
{
	struct node *sl;

	sl = NULL;

	append(&sl, 10);
	append(&sl, 70);
	append(&sl, 50);
	append(&sl, 30);
	append(&sl, 40);
	append(&sl, 20);
	append(&sl, 60);

	printf("Sorted merge :- \n");
	merge_sort(&sl);
	print_ll(sl);
	delete(&sl);
}

int main(int argc, char * argv[]) 
{
	test_case_1();
	test_case_2();
	test_case_3_1();
	test_case_3_2();
	test_case_3_3();
	return 0;
}
