#include <set>
#include <iostream>

using namespace std;

class set_test {
	private:
		int k;
		set<int> s;
		set<int> subset;
	public:
		set_test() {}
		void read_set(void)
		{
			int n;
			int element;

			cin >> n >> k; 
			for (auto i = 0; i < n; i++ ) {
				cin >> element;
				s.insert(element);
			}
		}
		void print_set(void)
		{
			for (auto i = s.begin(); i != s.end(); ++i) {
				cout << *i << " ";
			}
			cout << endl;
		}
		void print_subset(void)
		{
			for (auto i = subset.begin(); i != subset.end(); ++i) {
				cout << *i << " ";
			}
			cout << endl;
		}
		bool present(int n)
		{
			auto e = subset.find(n);
			
			return e != subset.end() ? true : false;
		}
		int not_evenly_divisible(int n)
		{
			return (n % k) != 0 ? true : false;
		}
		bool process_element(int ele)
		{
			bool insert;

			insert = true;

			if (! present(ele)) {
				for (auto k = subset.begin(); k != subset.end(); ++k) {
					if (! not_evenly_divisible(ele + *k)) {
						insert = false;
						break;
					}	
				}
				if (insert == true)
					subset.insert(ele);
			}

			return insert;
		}
		void find_set(void) 
		{
			int val;

			for (auto i = s.begin(); i != s.end(); ++i) {
                val = *i;
				for (auto j = s.begin(); j != s.end(); ++j) {
					if (val == *j)
						continue;
					//cout << val << " + " << *j << " " << val + *j << " ";
					if (not_evenly_divisible(val + *j)) {
						process_element(val);
						process_element(*j);
					}
				}
				//cout << endl;
			}

			print_set();
			print_subset();
			cout << subset.size() << endl;
		}
};

int main(void)
{
	class set_test s;

	s.read_set();
	s.find_set();
	return 0;
}
