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
remove_dup(struct node **sl)
{
	struct node *t;
	struct node *node;

	if (*sl == NULL)
		return;

	if ((*sl)->next == NULL)
		return;
	else {
		t = *sl;
		while (t != NULL && t->next != NULL) {
			if (t->data == t->next->data) {
				node = t->next;
				t->next = t->next->next;
				free(node);
			} else {
				t = t->next;
			}
		}
	}
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *sl;

	sl = NULL;
	remove_dup(&sl);
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
	append(&sl, 10);
	remove_dup(&sl);
	print_ll(sl);
	delete(&sl);

	sl = NULL;

	append(&sl, 10);
	append(&sl, 20);
	remove_dup(&sl);
	print_ll(sl);
	delete(&sl);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	struct node *sl;

	sl = NULL;

	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 40);
	append(&sl, 50);
	remove_dup(&sl);
	print_ll(sl);
	delete(&sl); 

	sl = NULL;

	append(&sl, 10);
	append(&sl, 10);
	append(&sl, 10);
	append(&sl, 10);
	append(&sl, 10);
	remove_dup(&sl);
	print_ll(sl);
	delete(&sl); 

	sl = NULL;

	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 30);
	append(&sl, 40);
	remove_dup(&sl);
	print_ll(sl);
	delete(&sl); 

 	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 30);
	append(&sl, 40);
	append(&sl, 40);
	append(&sl, 50);
	append(&sl, 60);
	append(&sl, 60);
	append(&sl, 60);
	remove_dup(&sl);
	print_ll(sl);
	delete(&sl); 
}


int main(int argc, char * argv[]) 
{
	//test_case_1();
    //test_case_2();
    test_case_3();
	return 0;
}
