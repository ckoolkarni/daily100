#include <stdio.h>

int main(int argc, char *argv[])
{
	int x;
	int y;
	unsigned int t;

	if (argc != 3)
		return -1;

	x = atoi(argv[1]);
	y = atoi(argv[2]);
    t = (x ^ y) >> 31;
	x = (x + t) ^ t;
	printf("%d\n", x);
	return 0;
}
