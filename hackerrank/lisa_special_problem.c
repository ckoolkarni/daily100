#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main(void)
{
	int i;
	int j;
	int n;
	int k;
	int *t;
	int flag;
	int page_count;
	int special_count;

	scanf("%d%d", &n, &k);
    if (n < 1 || n > 100 || k < 1 || k > 100) {
		fprintf(stderr, "invalid input\n");
		exit(EXIT_FAILURE);
	}

	if ((t = (int *) malloc(n * sizeof (int))) == NULL) {
		fprintf(stderr, "malloc %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


	for (i = 0; i < n; i++) {
		scanf("%d", &t[i]);
		if (t[i] < 1 || t[i] > 1000) {
			fprintf(stderr, "invalid input\n");
			exit(EXIT_FAILURE);
		}
	}

	page_count = 1;
	special_count = 0;

	for (i = 0; i < n; i++) {
		for (j = 1; j <= t[i]; j++) {
			if ((j % k) == 0) {
				if (j == page_count) {
					special_count++;
				}
				page_count++;
			} else {
				if (j == page_count) {
					special_count++;
				}
			}
		} 
		if ((j % k) != 0) {
			page_count++;
		}
	}
	printf("%d\n", special_count);	
	free(t);
	return 0;
}
