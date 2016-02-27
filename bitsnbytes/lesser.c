#include <stdio.h>

void print_bits(unsigned int num)
{
	int bit;
	int arr[sizeof(unsigned int) * 8];

	for (bit = 0; bit < (sizeof(unsigned int) * 8); bit++) {
        arr[(sizeof(unsigned int) * 8) - bit - 1] = num & 0x01;
		num = num >> 1;
	}
	for (bit = 0; bit < (sizeof(unsigned int) * 8); bit++) {
		printf("%d ", arr[bit]);
	}
	putchar(10);
}


int main(int argc, char *argv[])
{
	int x;
	int y;
	int res;

	if (argc != 3)
		return -1;

	x = atoi(argv[1]);
	y = atoi(argv[2]);
	/* x <= y -1, 0 otherwise */
	res = ((x | ~y) & ((x ^ y) | ~(y - x))) >> 31;
	printf("X	%5d: ", x), print_bits(x);
	printf("Y	%5d: ", y), print_bits(y);
	printf("RES	%5d: ", res), print_bits(res);
	return 0;
}
