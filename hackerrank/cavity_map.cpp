#include <sstream>	
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>

using namespace std;

class matrix {
	int r;
	int c;
	vector<int> rows;
	vector<vector<int> > mat;
	public:
	
	matrix(int r, int c)
	{
		this->r = r;
		this->c = c;
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				rows.push_back(0);
			}
			mat.push_back(rows);
		}
	}

	void read_matrix(void)
	{
		string line;

		for (int lines = 0; lines < this->r; lines++) {
			cin >> line;
			for (string::iterator i = line.begin(); i != line.end(); ++i) {
				if (isdigit(*i)) 
					mat[lines][i - line.begin()] = *i - '0';
			}
		}
	}

	void print_matrix(void)
	{
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				if (mat[i][j] == -1) {
					cout << "X";
				} else {
					cout << mat[i][j];
				}
			}
			cout << "\n";
		}
	}
	
	void check_cavity(void)
	{
		for (int i = 1 ; i < r - 1; i ++) {
			for (int j = 1; j < c - 1; j++) {
				if (mat[i][j] > mat[i - 1][j] &&
					mat[i][j] > mat[i + 1][j] &&
					mat[i][j] > mat[i][j - 1] &&
					mat[i][j] > mat[i][j + 1] 
					) {
					mat[i][j] = -1;
				}
			}
		}
	}
	~matrix(void)
	{             
		rows.clear();
		mat.clear();
	}
};

void
process(int n)
{
	class matrix m(n, n);

	m.read_matrix();
	m.check_cavity();
	m.print_matrix();
}

int main(void)
{
    int n;

	cin >> n;
	process(n);

	return 0;
}
