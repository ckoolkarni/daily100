#include <iostream>

using namespace std;

struct test {
	int i;
	int j;
};

void 
process(struct test t[], int nt, int w[], int n)
{
	int i;
	int vehicle;
	for (int test = 0; test < nt; test++) {
		for (vehicle = 3; vehicle > 0; vehicle--) {
			for (i = t[test].i; i <= t[test].j; i++) {
				if (vehicle > w[i])
					break;
			}
			if (i > t[test].j) {
				cout << vehicle << endl;
				break;
			} 
#if 0
			if (i > t[test].j) {
				cout << "Accepted : "<< vehicle << endl;
			} else {
				cout << "Not Accepted : "<< vehicle << endl;
			}
#endif
		}
	}
}


int main(void)
{
	int n;
	int tests;
	int *w;
	struct test *t;

	cin >> n >> tests;
	w = new int[n];
	t = new test[n];

	for (int i = 0; i < n; i++) {
		cin >> w[i];
	}
	for (int i = 0; i < tests; i++) {
		cin >> t[i].i >> t[i].j; 	
	}
	process(t, tests, w, n);
	delete [] t;
	delete [] w;
	return 0;
}
