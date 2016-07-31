#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* http://www.geeksforgeeks.org/add-elements-given-arrays-given-constraints/ */

int split_print(int n)
{
	int i;
	int d1;
	int d2;
	int cnt;
	int arr[32];

	if (n > 9) {
		cnt = 0;
		while (n > 0) {
			d1 = n % 10;
			n /= 10;
			arr[cnt++] = d1;
			if (cnt > 32) {
				printf("ERROR\n");
				exit(EXIT_FAILURE);
			}
		}
		for (i = cnt - 1; i >= 0 ; i--) {
			printf("%d ", arr[i]);
		}
	}
}

void process_array(int a[], int asize, int b[], int bsize)
{
    int i;
	int j;
	int sum;

	if ((a == NULL && b == NULL) || (asize == 0 && bsize == 0)) {	
		fprintf(stderr, "a dn b are both empty\n");
		return;
	}

	if (asize != 0 && bsize == 0) {
		for (i = 0; i < asize; i++) {
			a[i] > 9 ? split_print(a[i]) : printf("%d", a[i]);
		}
	} else if (asize == 0 && bsize != 0) {
		for (i = 0; i < bsize; i++) {
			b[i] > 9 ? split_print(b[i]) : printf("%d ", b[i]);
		}
	} else {
		for (i = 0, j = 0; i < asize && j < bsize; i++, j++) {
			sum = a[i] + b[j];
			sum > 9 ? split_print(sum) : printf("%d ", sum);
		}

		if (asize > bsize) {
			while (i < asize) {
				a[i] > 9 ? split_print(a[i]) : printf("%d ", a[i]);
				i++;
			}
		} else if (asize < bsize) {
			while (j < bsize) {
				b[j] > 9 ? split_print(b[j]) : printf("%d ", b[j]);
				j++;
			}
		}
	}
	printf("\n");
}

void test1(void)
{
	int *a;
	int *b;
	int asize;
	int bsize;

	a = NULL;
	b = NULL;
	asize = 0;
	bsize = 0;
	process_array(a, asize, b, bsize);
}

void test_2_1(void)
{
	int a [] = {1};
	int *b;
	int asize;
	int bsize;

	b = NULL;
	asize = 1;
	bsize = 0;
	process_array(a, asize, b, bsize);

}

void test_2_2(void)
{
	int *a;
	int b[] = {1};
	int asize;
	int bsize;

	a = NULL;
	asize = 0;
	bsize = 1;
	process_array(a, asize, b, bsize);
}

void test2(void)
{
	test_2_1();
	test_2_2();
}

void test_3_1(void)
{
	int a[] = {9, 2, 3, 7, 9, 6};
	int b[] = {3, 1, 4, 7, 8, 7, 6, 9};
	int asize;
	int bsize;
    
	asize = 6;
	bsize = 8;
	process_array(a, asize, b, bsize);
}

void test_3_2(void)
{
	int a[] = {9343, 2, 3, 7, 9, 6};
	int b[] = {34, 11, 4, 7, 8, 7, 6, 99};
	int asize;
	int bsize;
    
	asize = 6;
	bsize = 8;
	process_array(a, asize, b, bsize);
}

void test_3_3(void)
{
   	int a[] = {};
	int b[] = {11, 2, 3 };

	int asize;
	int bsize;
    
	asize = 0;
	bsize = 3;
	process_array(a, asize, b, bsize);
}

void test_3_4(void)
{
    int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
	int b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int asize;
	int bsize;
    
	asize = 9;
	bsize = 9;
	process_array(a, asize, b, bsize);
}            

void test3(void)
{
	test_3_1();	
    test_3_2();	
	test_3_3();	
	test_3_4();	
}

int main(void)
{
	test1();
	test2();
	test3();

	return 0;
}
