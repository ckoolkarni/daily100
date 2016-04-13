#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int *
init_union(int n)
{
	int *parent;

	if ((parent = (int *) malloc(n * sizeof (int))) == NULL) {
		perror("malloc : ");
		exit(EXIT_FAILURE);
	}

	memset(parent, -1, n * sizeof (int));

	return parent;
}

void
make_union(int *parent, int a, int b)
{
	assert(parent != NULL);
	/* change the smaller elemets parent to larger one */
	if (a > b)
		parent[b] = a;
	else 
		parent[a] = b;
}

int 
find_set(int *parent, int n)
{
	assert (parent != NULL);

	if (parent[n] == -1)
		return n;
	return find_set(parent, parent[n]);
}

int main(int argc, char *argv[])
{
	int i;
    int n;
	int *parent;

	n = 10;
	parent = init_union(n);

    make_union(parent, 0, 1);
    make_union(parent, 2, 3);
    make_union(parent, 3, 4);
    make_union(parent, 8, 9);
    make_union(parent, 4, 7);

	for (i = 0; i < n; i++) {
		printf("set %d element %d\n", find_set(parent, i), i);
	}
	return 0;
}
