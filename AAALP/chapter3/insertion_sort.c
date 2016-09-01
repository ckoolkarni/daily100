#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

uint64_t cmp_cnt;
uint64_t copy_cnt;

void insertion_sort(int a[], int len)
{
	int32_t i;
	int32_t j;
	int32_t key;

	for (j = 1; j < len; j++) {
		key = a[j];
		i = j - 1;
		while (++cmp_cnt && i > -1 && ++cmp_cnt && a[i] > key) {
			copy_cnt++;
			a[i + 1] = a[i];
			i = i - 1;
		}
		a[i + 1] = key;
		copy_cnt++;
	}
}

int main(int argc, char *argv[])
{
	int32_t i;
	int32_t *a;
	int32_t len;

	scanf("%d", &len);
    if ((a = (int32_t *) malloc(sizeof (int32_t) * len)) == NULL) {
		fprintf(stderr, "ERROR : malloc : %s\n" , strerror(errno));
		return errno;
	}
	for (i = 0; i < len; i++) {
		scanf("%d", &a[i]);
	}
	insertion_sort(a, len);
	for (i = 0; i < len; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	printf("Compare_count %"PRIu64" copy_cnt %"PRIu64"\n", cmp_cnt, copy_cnt);
    free(a);
	return 0;
}
