#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>

int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void
validate_date(int d, int m, int y)
{
	if (d < 1 || d > 31) {
		fprintf(stderr, "invalid date\n");
		exit(EXIT_FAILURE);
	}
	if (m < 1 || m > 12) {
		fprintf(stderr, "invalid month\n");
		exit(EXIT_FAILURE);
	}
	if (y < 1 || y > 3000) {
		fprintf(stderr, "invalid month\n");
		exit(EXIT_FAILURE);
	}
}

int 
is_leap(int y)
{
	if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
		return 1;
	return 0;
}

/* call this functions when year is same and months are different */
int
days_difference(int d, int m, int y, int dd, int mm, int yy)
{
	int i;
	int days;
	int isleap;

	assert(mm > m);

	isleap = is_leap(y);

	if (m == 2 && isleap) {
		days = months[m] + 1 - d;
	} else {
		days = months[m] - d;
	}

	for (i = m + 1; i < mm; i++) {
		days += months[i];
		if (m == 2 && isleap)
			days++;
	}

	days += dd;

	return days;	
}

int main(void)
{
	int d;
	int m;
	int y;
	int dd;
	int mm;
	int yy;
	int fine;

	scanf("%d%d%d", &dd, &mm, &yy);
	scanf("%d%d%d", &d, &m, &y);
	
	validate_date(d, m, y);
	validate_date(dd, mm, yy);
	fine = 0;
	if (yy <= y) {
	    if (mm <= m) {
			if (dd <= d) {
				fine = 0;
			} else {
				if (mm == m) {
					fine = 15 * (dd - d);	
				}
			}
		} else {
			if (yy == y)
				fine = 500 * (mm - m);
		}
	} else {
		fine = 10000;
	}
	printf("%d\n", fine);

	return 0;	
}
