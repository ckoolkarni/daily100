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
recursive_reverse(struct node *sl, struct node **newstart)
{
    struct node *t;

    if (sl == NULL) {
        return NULL;
    }

    t = recursive_reverse(sl->next, newstart);
    if (t == NULL) {
        *newstart = sl;
    } else {
        t->next = sl;
        sl->next = NULL;
    }
    return sl;
}

/* Empty Linked List */
void
test_case_1(void)
{
	struct node *sl;
	struct node *newstart;

	sl = NULL;
	newstart = NULL;
	recursive_reverse(sl, &newstart);
	print_ll(sl);

	delete(&sl);
}

/* Single Node */
void 
test_case_2(void) 
{
	struct node *sl;
	struct node *newstart;

	sl = NULL;
	newstart = NULL; 
	append(&sl, 10);
	recursive_reverse(sl, &newstart);
	print_ll(sl);

	delete(&sl);
}

/* Multiple Nodes */
void 
test_case_3(void) 
{
	struct node *sl;
	struct node *newstart;

	sl = NULL;     
	newstart = NULL;     
	append(&sl, 10);
	append(&sl, 20);
	append(&sl, 30);
	append(&sl, 40);
	print_ll(sl);
	recursive_reverse(sl, &newstart);
	print_ll(sl);
	print_ll(newstart);
	delete(&sl);
}


int main(int argc, char * argv[]) 
{
	test_case_1();
    test_case_2();
    test_case_3();
	return 0;
}
