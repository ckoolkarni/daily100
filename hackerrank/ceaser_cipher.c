#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>

int rotate_lower(char c, int k) 
{
	assert(c >= 'a' && c <= 'z');
    return 'a' + (((c - 'a') + k) % ('z' - 'a' + 1));
}

int rotate_upper(char c, int k) 
{
	assert(c >= 'A' && c <= 'Z');
    return 'A' + (((c - 'A') + k) % ('Z' - 'A' + 1));
}

int main(void)
{
	int i;
	int n;
	int k;
	char *s;

	scanf("%d", &n);
	if ((s = (char *) malloc(n * sizeof(char)))  == NULL) {
		fprintf(stderr, "ERROR : malloc : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	scanf("%s", s); 
	scanf("%d", &k);

	if (n < 1 || n > 100 || k < 0 || k > 100) {
		fprintf(stderr, "invlid input\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < n; i++) {
		if (islower(s[i]))
			s[i] = rotate_lower(s[i], k);
		else if (isupper(s[i]))
			s[i] = rotate_upper(s[i], k);
	}

	printf("%s\n", s);

	free(s);
	return 0;	
}
