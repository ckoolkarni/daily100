#include <stdio.h>
#include <stdlib.h>

struct queue_node {
    void *data;
	struct queue_node *next;
	struct queue_node *prev;
};

struct queue {
	struct queue_node *head;
	struct queue_node *tail;
	unsigned long long current_length;
};

struct queue_node * 
alloc_node(void *data)
{
	struct queue_node *n;

	if ((n = (struct queue_node *) malloc(sizeof(struct queue_node))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	n->data = data;
	n->next = NULL;
	n->prev = NULL;

	return n;
}

void
free_node(struct queue_node *n)
{
	if (n != NULL)
		free(n);
}

struct queue *
create_queue(void)
{
	struct queue *q;

	if ((q = (struct queue *) malloc(sizeof (struct queue))) == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}

	q->head = NULL;
	q->tail = NULL;
    q->current_length = 0;
	return q;	
}

void
enqueue(struct queue *q, void *data)
{
	struct queue_node *n;

	n = alloc_node(data);

	if (q->head == NULL && q->tail == NULL) {
		q->head = q->tail = n;		
	} else {
		n->prev = q->head;
		q->head->next = n;
		q->head = n;
	}
    q->current_length++;
}

void *
dequeue(struct queue *q)
{
	void *data;
	struct queue_node *t;

	if (q->head == NULL || q->tail == NULL) {
		fprintf(stderr, "Queue Underflow\n");
		exit(EXIT_FAILURE);
	}

	t = q->tail;
	data = t->data;
	if (q->head == q->tail) {
		q->head = q->tail = NULL;		
	} else {
		q->tail = q->tail->next;
		q->tail->prev = NULL;
	}

	free_node(t);
    q->current_length--;

	return data;	
}

int
is_empty(struct queue *q)
{
	return q->head == NULL && q->tail == NULL;	
}

unsigned long long 
get_current_length(struct queue *q)
{
	return q->current_length;
}

void
print_queue(struct queue *q)
{
	struct queue_node *t;

	printf("HEAD ");
	for (t = q->head; t != NULL; t = t->prev)
		printf("<-%p ", t->data);
	printf("<-TAIL\n");
}

int main(void)
{
	struct queue *q;

	q = create_queue();

	print_queue(q);
	enqueue(q, (void *) 1);
	print_queue(q);
	enqueue(q, (void *) 2);
	print_queue(q);
	enqueue(q, (void *) 3);
	print_queue(q);
	enqueue(q, (void *) 4);
	print_queue(q);
	enqueue(q, (void *) 5);
	print_queue(q);
	dequeue(q);
	print_queue(q);
	dequeue(q);
	print_queue(q);
	dequeue(q);
	print_queue(q);
	dequeue(q);
	print_queue(q);
	dequeue(q);
	print_queue(q);
	return 0;
}



















