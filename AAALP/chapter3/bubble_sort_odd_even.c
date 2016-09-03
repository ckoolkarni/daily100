#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

uint64_t swap_cnt;
uint64_t compare_cnt;


bool smaller_to_top(int a[], int len, int i)
{
	int j;
	int t;
	bool swapped;

	swapped = false;
	for (j = len - 1; j > 0; j--){
		if (++compare_cnt && a[j - 1] > a[j]) {
			t = a[j], a[j] = a[j - 1], a[j - 1] = t;
			swapped = true;
			swap_cnt++;
		}
	}
	return swapped;
}

bool larger_to_bottom(int a[], int len, int i)
{
	int j;
	int t;
	bool swapped;

	swapped = false;
	for (j = 0; j < (len - i - 1); j++) {
		if (++compare_cnt && a[j] > a[j + 1]) {
			t = a[j], a[j] = a[j + 1], a[j + 1] = t;
			swapped = true;
			swap_cnt++;
		}
	}
	return swapped;
}

void bubble_sort(int a[], int len)
{
	int i;
    bool swapped;

	for (i = 0; i < len; i++) {
		swapped = false;
		if ((i % 2) == 0)
			swapped = larger_to_bottom(a, len, i);
		else
			swapped = smaller_to_top(a, len, i);

		if (swapped == false)
			break;
	}
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
	int b[] = {5, 4, 3, 2, 1};

	print(b, 5);
	bubble_sort(b, 5);
	print(b, 5);
	printf("compare_cnt %"PRIu64" swap_cnt %"PRIu64"\n", compare_cnt, swap_cnt);
	return 0;
}
