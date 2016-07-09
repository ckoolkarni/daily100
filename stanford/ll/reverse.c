#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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
reverse(struct node **sl)
{
	struct node *t;
	struct node *t1;
	struct node *t2;
	struct node *tstart;

	if (*sl == NULL)
		return;
    if ((*sl)->next == NULL)
        return;

    t = *sl;
    t1 = t->next;
    t2 = t1->next;
    (*sl)->next = NULL;
    
    while (t2 != NULL) {
        t1->next = t;
        t = t1;
        t1 = t2;
        t2 = t2->next;
    }
    t1->next = t;
    tstart = t1;
    *sl = tstart;
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *a;

	a = NULL;
	reverse(&a);
	print_ll(a);
	delete(&a);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *a;

	a = NULL;
	append(&a, 10);
	reverse(&a);
	print_ll(a);
	
	delete(&a);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	int i;
	struct node *a;

	a = NULL;
	for (i = 0; i < 10; i++)
		append(&a, i * 10);
	print_ll(a);
	reverse(&a);
	print_ll(a);

	delete(&a);
}

int main(void)
{
    test_case_1();
    test_case_2();
    test_case_3();

    return 0;
}
