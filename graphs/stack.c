#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stack_node {
	int data;
	struct stack_node *next;
	struct stack_node *prev;

};

struct stack {
	int current_count;
	struct stack_node *top;
};

void
print_stack(struct stack *s)
{
	struct stack_node *temp;

	assert(s != NULL);
    printf("TOP <-");
	for (temp = s->top; temp != NULL; temp = temp->prev) {
		printf("<-%2d", temp->data);
	}
    printf("<-BOTTOM\n");
}

struct stack_node *
alloc_stack_node(int data)
{
	struct stack_node *n;

	if ((n = (struct stack_node *) malloc(sizeof(struct stack_node))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	n->data = data;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

void
free_stack_node(struct stack_node *n)
{
	if (n != NULL)
		free(n);
}

struct stack *
creat_stack(void) 
{
	struct stack *s;

	if ((s = (struct stack *) malloc(sizeof (struct stack))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	s->top = NULL;
	s->current_count = 0;

	return s;
}

void 
push (struct stack *s, int data)
{
    struct stack_node *n;

	assert(s != NULL);
	n = alloc_stack_node(data);

	if (s->top == NULL) {
		s->top = n;
	} else {
		s->top->next = n;
		n->prev = s->top;
		s->top = n;
	}
	s->current_count++;
}

int
pop(struct stack *s)
{
	int data;
	struct stack_node *t;

	assert(s != NULL);
	if (s->top == NULL) {
		fprintf(stderr, "stack underflow\n");
		return -1;
	}

	t = s->top;
	s->top = s->top->prev;
	/* Last element in the stack */
	if (s->top != NULL)
		s->top->next = NULL;
    data = t->data;
	free_stack_node(t);
	s->current_count--;

	return data;
}

int
is_empty(struct stack *s) 
{
	return s->top == NULL;
}

int main(void)
{
    struct stack *s;

	s = creat_stack();

	print_stack(s);
    push(s, 10);
	print_stack(s);
    push(s, 11);
	print_stack(s);
    push(s, 12);
	print_stack(s);
    push(s, 13);
	print_stack(s);
    push(s, 14);
	print_stack(s);
    push(s, 15);
	print_stack(s);
    push(s, 16);
	print_stack(s);
	if (! is_empty(s))
		printf("Not Empty\n");
	else 
		printf("Empty\n");

    pop(s);
	print_stack(s);
    pop(s);
	print_stack(s);
    pop(s);
	print_stack(s);
    pop(s);
	print_stack(s);
    pop(s);
	print_stack(s);
    pop(s);
	print_stack(s);
    pop(s);
	print_stack(s);

	if (! is_empty(s))
		printf("Not Empty\n");
	else 
		printf("Empty\n");
	return 0;
}
