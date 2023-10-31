#include "Quine_McCluskey.h"
#include "imp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <set>

using namespace std;


vector<implicant_struct> Quine_McCluskey::get_prime() {
	return this->prime_implicants;
}

vector<unsigned int> Quine_McCluskey::get_on_set() {
	return this->on_set;
}

int Quine_McCluskey::get_N() {
	return this->N;
}

int Quine_McCluskey::get_num_of_1(implicant_struct x) {
	return __builtin_popcount(x.data);
}

bool Quine_McCluskey::compare_implicants(implicant_struct x, implicant_struct y) {
	unsigned int dash_com = x.dashes ^ y.dashes;
	return ((__builtin_popcount(x.data ^ y.data)==1) && (__builtin_popcount(dash_com)==0));
}

implicant_struct Quine_McCluskey::combine_implicants(implicant_struct x, implicant_struct y) {
	implicant_struct tmp;
	tmp.dashes = x.dashes | (x.data ^ y.data);
	tmp.data = x.data & (~(tmp.dashes));
	return tmp;
}

bool Quine_McCluskey::table_empty() {
	for (int i = 0; i < this->table.size(); i++) {
		if (table[i].size() != 0) {
			return true;
		}
	}
	return false;
}

bool Quine_McCluskey::check_duplicate(vector<implicant_struct> x, implicant_struct y) {
	for (int i = 0; i < x.size(); i++) {	
		if (((x[i].data) == (y.data))&&((x[i].dashes) == (y.dashes))) {
			return true;
		}
	}
	return false;
}

void Quine_McCluskey::Init(ifstream &input_file) {

    char key;
	set<unsigned int> on_dc_set;
	implicant imp_tmp;
	imp_tmp.dashes = 0;

	string s;
	while( getline(input_file, s) ) {
		stringstream ss(s);
		ss >> key;
		if( key=='.' ) {
			ss >> key;
            if( key=='i' ) {
				getline(input_file, s);
		    	this->N = atoi(s.c_str());
		    }
		    else if( key=='m' ) {
				getline(input_file, s);
				stringstream ss(s);
				while (getline(ss, s, ' ')) {
					this->on_set.push_back(atoi(s.c_str()));
					on_dc_set.insert(atoi(s.c_str()));
				}
		    }
		    else if( key=='d' ) {
				getline(input_file, s);
				stringstream ss(s);
				while (getline(ss, s, ' ')) {
					this->dc_set.push_back(atoi(s.c_str()));
					on_dc_set.insert(atoi(s.c_str()));
				}
		    }
        }
	}
	for (set<unsigned int>::iterator itr = on_dc_set.begin(); itr != on_dc_set.end(); ++itr) {
		imp_tmp.data = *itr;
		this->implicants.push_back(imp_tmp);
	}
    table = vector<vector<implicant_struct> >(N+1);
}

void Quine_McCluskey::create_table() {
	int num1;
	for (vector<implicant_struct>::iterator itr=this->implicants.begin(); itr != this->implicants.end(); ++itr) {
		num1 = get_num_of_1(*itr);
		table[num1].push_back(*itr);
	}
}

void Quine_McCluskey::gen_prime_implicants() {
	vector<vector<bool> > prime (this->table.size());
	for (int i = 0; i < this->table.size(); i++) {
		for (int j = 0; j < this->table[i].size(); j++) {
			prime[i].push_back(false);
		}
	}
	implicant_struct tmp;
	vector<vector<implicant_struct> > new_table (this->table.size()-1);
	for (int i = 0; i < this->table.size()-1; i++) {
		for (int j = 0; j < this->table[i].size(); j++) {
			for (int k = 0; k < this->table[i+1].size(); k++) {
				if (compare_implicants(this->table[i][j], this->table[i+1][k])) {
					tmp = combine_implicants(this->table[i][j], this->table[i+1][k]);
					if (!check_duplicate(new_table[i], tmp)) {
						new_table[i].push_back(combine_implicants(this->table[i][j], this->table[i+1][k]));
					}
					prime[i][j] = true;
					prime[i+1][k] = true;
				}
			}
		}
	}

	for (int i = 0; i < this->table.size(); i++) {
		for (int j = 0; j < this->table[i].size(); j++) {
			if (!prime[i][j]) {
				this->prime_implicants.push_back(this->table[i][j]);
			}
		}
	}
	this->table = new_table;
}

void Quine_McCluskey::solve() {
	this->create_table();
	while (table_empty()) {
		gen_prime_implicants();
	}
}
