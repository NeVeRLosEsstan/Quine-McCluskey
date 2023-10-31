#ifndef PET
#define PET

#include "imp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

typedef struct map_element {
	int axis;
	set<int> x_loc;
} map_element_struct;

struct Node {
	int start;
	set<int> imp;
	vector<unsigned int> mcov;
	vector<vector<unsigned int> > map;
	vector<int> axis;
	vector<Node *> children;
} ;

class Petrick{
	private:
		int N;
		vector<map_element_struct> map_row;
		vector<map_element_struct> map_col;
		vector<implicant_struct> prime_implicants;
		vector<implicant_struct> essential_implicants;
		vector<set<int> > all_min;
		vector<unsigned int> on_set;

		bool EPI();
		bool RD();
		bool CD();
		bool compare_prime_on(implicant_struct, unsigned int);
        bool check_duplicate(vector<implicant_struct>, implicant_struct);

	public:
		void Init( vector<implicant_struct>, vector<unsigned int>, int);
		sort_element_struct solve();

};

#endif