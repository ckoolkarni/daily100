#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

uint64_t swap_cnt;
uint64_t compare_cnt;

int bubble_sort(int a[], int len)
{
	int i;
	int j;
	int t;
	bool swapped;

	for (i = 0; i < len - 1; i++) {
		swapped = false;
		for (j = 0; j < (len - i - 1); j++) {

			if (++compare_cnt && a[j] > a[j + 1]) {
				t = a[j], a[j] = a[j + 1], a[j + 1] = t;
				swapped = true;
				swap_cnt++;
			}
		}
		if (swapped == false)
			break;
	}
	return 0;
}


void print(int a[], int len)
{
	int i;
	for (i = 0; i < len; i++)
		printf("%d ", a[i]);
	printf("\n");
}


int main(int argc, char *argv[])
{
	int a[] = {1, 2, 3, 4, 5};
	int b[] = {5, 4, 3, 2, 1};

	print(b, sizeof(b)/ sizeof(b[0]));
	bubble_sort(b, sizeof(b)/sizeof(b[0]));
	print(b, sizeof(b)/ sizeof(b[0]));
	printf("compare_cnt %"PRIu64" swap_cnt %"PRIu64"\n", compare_cnt, swap_cnt);
	return 0;
}
