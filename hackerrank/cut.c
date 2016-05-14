#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int
compare(const void *a, const void *b)
{
	return (*(int *) a - *(int *)b);
}

int get_lowest_element(int arr[], int n)
{
	int i;
	int min;

	min = INT_MAX;
	for (i = 0; i < n; i++) {
		if (arr[i] != 0 && min > arr[i])
			min = arr[i];
	}
	return min;
}

int main(void) 
{
	int i;
	int n;
	int min;
	int *arr;
	int count;

	scanf("%d", &n);

	if (n < 1 || n > 1000) {
		fprintf(stderr, "invlid n %d\n", n);
		return -1;
	}

	if ((arr = (int *) malloc(n * sizeof (int))) == NULL) {
		fprintf(stderr, "malloc %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	for (i =0; i < n; i++) {
		scanf("%d", &arr[i]);
		if (arr[i] < 1 || arr[i] > 1000) {
			fprintf(stderr, "element invalid range %d\n", arr[i]);
			return -1;
		}
	}
	qsort(arr, n, sizeof(int), compare);

	while (1) {
		if ((min = get_lowest_element(arr, n)) == INT_MAX)
			break;
		count = 0;
		for (i = 0; i < n; i++) {
			if (arr[i] != 0) {
				arr[i] -= min;
				count++;
			}
		}
		if (count == 0)
			break;
		printf("%d\n", count);
	}
    free(arr);
	return 0;
}
