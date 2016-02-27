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

	res = ((x - y | y - x)) >> 31;
	print_bits(x);
	print_bits(y);
	print_bits(x - y);
	print_bits(y - x);
	print_bits(res);
	return 0;
}
