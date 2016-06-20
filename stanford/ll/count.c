#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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
count(struct node *s)
{
	int c;

	for (c = 0; s != NULL; c++, s = s->next)
		;
	printf("Count %d\n", c);
}


int main(void) 
{
	struct node *s;

	append(&s, 10);
	append(&s, 20);
	append(&s, 30);
	append(&s, 40);
	append(&s, 50);

	count(s);
}
