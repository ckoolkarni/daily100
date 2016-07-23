#include <cassert>
#include <iostream>
#include <stack>

using namespace std;

class cylinder {
	private:
		int n;
		int *arr;
		stack<int>aggr;
	public:
		cylinder(void) { }
		void set_size(int n) {
			this->n = n;
			arr = new int[this->n];
		}
		void read_elements(void)
		{
			int sum;

			sum = 0;
			for (int i = 0; i < n; i++) {
				cin >> arr[i];
				sum += arr[i];
				aggr.push(sum);
			}
		}

		int aggr_top(void)
		{
			return aggr.top();
		}
		void aggr_pop(void)
		{
			if (! aggr.empty())
				aggr.pop();
			else 
				cerr << "Stack underflow\n";
		}
		bool aggr_empty(void)
		{
			return aggr.empty();
		}
		void print(void)
		{
			for (int i = 0; i < n; i++) {
				cout << arr[i] << endl;
			}
		}
		~cylinder(void) 
		{ 
			delete [] arr;   
		}
};

void process(class cylinder &c1, class cylinder &c2, class cylinder &c3)
{
	int a;
	int b;
	int c;

	while ((!c1.aggr_empty()) && (!c2.aggr_empty()) && (!c3.aggr_empty())) {

		a = c1.aggr_top();	 
		b = c2.aggr_top();	 
		c = c3.aggr_top();	 

		if (a == b && b == c) {
			cout << ":)" << a << endl;
			return;
		} else if (a == b) {
			if (a > c) {
				/* remove a & b */
				c1.aggr_pop();
				c2.aggr_pop();
			} else {
				/* remove c */
				c3.aggr_pop();
			}
		} else if (b == c) {
			if (b > a) {
				/* remove b & c */
				c2.aggr_pop();
				c3.aggr_pop();
			} else {
				/* remove a */
				c1.aggr_pop();
			}
		} else if (a == c) {
			if (a > b) {
				/* remove a & c */
				c1.aggr_pop();
				c3.aggr_pop();
			} else {
				/* remove b */
				c2.aggr_pop();
			}
		} else {
			assert(a != b && b != c && c != a);
			if (a > b) {
				c1.aggr_pop();
				b > c ? c2.aggr_pop() : c2.aggr_pop();
			} else {
				c2.aggr_pop();
				a > c ? c1.aggr_pop() : c1.aggr_pop();
			}
		}
	}
	cout << "(:" << endl;
}

int main(void)
{
	int s1;
	int s2;
	int s3;
	class cylinder c1;
	class cylinder c2;
	class cylinder c3;

	cin >> s1 >> s2 >> s3;
	c1.set_size(s1);
	c1.read_elements();
	c2.set_size(s2);
	c2.read_elements();
	c3.set_size(s3);
	c3.read_elements();
	
	c1.set_stack();
	c2.set_stack();
	c3.set_stack();
	process(c1, c2, c3);
	return 0;
}
