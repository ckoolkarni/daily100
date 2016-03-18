#include <stdio.h>

#define TRUE	1
#define FALSE	0
#define MAXNODES 6

int adj[MAXNODES][MAXNODES];

void
print_adj_list(int adj[][MAXNODES])
{
	int i, j;

	for (i = 0; i < MAXNODES; i++) {
		for (j = 0; j < MAXNODES; j++) {
			printf("adj[%d][%d] = %d\t", i, j, adj[i][j]);
		}
		printf("\n");
	}
}


void
join(int adj[][MAXNODES], int node1, int node2)
{
	adj[node1][node2] = TRUE;
}

void
remv(int adj[][MAXNODES], int node1, int node2)
{
	adj[node1][node2] = FALSE;
}

int
adjecent(int adj[][MAXNODES], int node1, int node2)
{
	return adj[node1][node2] == TRUE ? TRUE : FALSE;
}

int 
findpath(int nr, int a, int b)
{
	int c;

	if (nr == 1) 
		return adjecent(adj, a, b);
	/* Determine if there is apth through C */
	for (c = 0; c < nr; c++) {
		if (adjecent(adj, a, c) && findpath(nr - 1, c, b))
			return TRUE;
	}
    return FALSE;
}

int
main(void)
{
	int n, a, b, nr;
	int city1, city2;

	scanf("%d%d%d%d", &n, &a, &b, &nr);
	/* create n nodes and label them from 0 to n - 1*/
	print_adj_list(adj);
	while (scanf("%d %d", &city1, &city2) != EOF) {
		join(adj, city1, city2);
	}
	print_adj_list(adj);
	if (findpath(nr, city1, city2)) {
		printf("Path Found\n");
	} else {
		printf("Path not Found\n");
	}
	return 0;
}
