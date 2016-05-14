#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

struct test {
	int n;
	int c;
	int m;
	int total;
};

int main(void)
{
	int i;
	int n;
	int c;
	int w;
	int wrappers;
	int chocolates;
	struct test *t;

	scanf("%d", &n);

	if (n < 0 && n > 1000) {
		fprintf(stderr, "ERROR invalid test number\n");
		exit(EXIT_FAILURE);
	}

    if ((t = (struct test *) malloc(n * sizeof(*t))) == NULL) {
		fprintf(stderr, "ERROR malloc : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < n; i++) {
		scanf("%d%d%d", &t[i].n, &t[i].c, &t[i].m);
		if (t[i].n < 2 || t[i].n > 1000000 || t[i].c < 0 || t[i].c > t[i].n || 
											t[i].m < 2 || t[i].m > t[i].n) {
			fprintf(stderr, "invalid input\n");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < n ; i++) {

		chocolates = t[i].n / t[i].c;
		wrappers = chocolates;
        
		while (wrappers >= t[i].m) {
			c = wrappers / t[i].m;
			w = wrappers % t[i].m;
			wrappers = c + w;
			chocolates += c;
		}
		t[i].total = chocolates;
	}

    for (i = 0; i < n; i++) {
		printf("%d\n", t[i].total);
	}
	free(t);

	return 0;
}
