#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct node {
	int data;
	struct node *next;
};

void print_ll(struct node *sl);

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
sorted_insert(struct node **sl, struct node *n)
{
	struct node *t;
	
	if (*sl == NULL) {
		*sl = n;
		return;
	}
    if ((*sl)->next == NULL) {
		if ((*sl)->data <= n->data) {
			(*sl)->next = n;
		} else if ((*sl)->data > n->data) {
		    n->next = *sl;
			*sl = n;
		}
	} else {
		if ((*sl)->data > n->data) {
		    n->next = *sl;
			*sl = n;
		} else if ((*sl)->data <= n->data && (*sl)->next->data >= n->data) {
			n->next = (*sl)->next;
			(*sl)->next = n;
		} else {
		   for (t = (*sl)->next; t->next != NULL; t = t->next) {
			   if (t->data <= n->data && t->next->data >= n->data) {
				   n->next = t->next;
				   t->next = n;
				   return;
			   }
		   }
		   if (t->next == NULL)
			   t->next = n;
		}
	}
}

void
insert_sort(struct node **sl)
{
	struct node *t;
	struct node *new_list;

    if (*sl == NULL || (*sl)->next == NULL)
		return;

	new_list = NULL;

	while (*sl != NULL) {
		t = *sl;
		*sl = (*sl)->next;
		t->next = NULL;
		sorted_insert(&new_list, t);
		print_ll(new_list);
	}
    
	*sl = new_list;
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
	struct node *sl;

	sl = NULL;
	insert_sort(&sl);
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
	print_ll(sl);
	insert_sort(&sl);
	print_ll(sl);
	delete(&sl);

}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	struct node *sl;

	sl = NULL;
	append(&sl, 50);
	append(&sl, 30);
	append(&sl, 20);
	append(&sl, 40);
	append(&sl, 10);
	print_ll(sl);
	
	insert_sort(&sl);
	delete(&sl);

	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 40);
	append(&sl, 50);
	print_ll(sl);
	
	insert_sort(&sl);
	delete(&sl);
}


int main(int argc, char * argv[]) 
{

	test_case_1();
	test_case_2();
	test_case_3();
	return 0;
}
