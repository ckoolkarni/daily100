#include <deque>
#include <iostream>

using namespace std;

void
front(void)
{
	deque<int> dq;
    int a[] = {3, 2, 1, 1, 1};
    int sum;
	sum = 0;

	for (int i = 0; i < 5; ++i) {
		dq.push_back(a[i]);
	}
	
	for (auto i = 0U; i < 5; i++) {
        sum += dq.back();
		dq.pop_back();
		dq[4 - i] = sum;
		cout << "sum " << sum << endl;
	}

	for (auto i = 0U; i < 5; i++) {
		cout << dq[i] << endl;
	}

	cout << endl;
}
int main(void)
{
	front();
	return 0;
}
