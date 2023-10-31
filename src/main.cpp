#include "Quine_McCluskey.h"
#include "Petrick.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

bool sort_by_output(const implicant_struct a, const implicant_struct b) {
	for (int i = 31; i >= 0; i--) {
		int bita = a.data>>i & 1;
		int dasha = a.dashes>>i & 1;
		int bitb = b.data>>i & 1;
		int dashb = b.dashes>>i & 1;
		if (bita > bitb) {
			if (dasha==0 && dashb==0) return true;
			else if (dasha==0 && dashb!=0) return true;
			else if (dasha!=0 && dashb==0) return false;
		}
		else if (bita < bitb) {
            if (dasha==0 && dashb==0) return false;
            else if (dasha==0 && dashb!=0) return true;
            else if (dasha!=0 && dashb==0) return false;
        }
		else {
            if (dasha==0 && dashb!=0) return true;
            else if (dasha!=0 && dashb==0) return false;
        }
	}
	return true;
}

void print_output(int N, implicant_struct x, ofstream& out) {
	for (int i = N-1; i >= 0; i--) {
		int bit = (x.data >> i) & 1;
		int bitd = (x.dashes >> i) & 1;
		if (bitd == 0) {
			out << char(N-i+64);
			if (bit == 0) out << '\'';
		}
    }
}

int main(int argc, char *argv[]) {

	if( argc!=3 ) {
		cerr<<"Command should be : ./<student_id>.o <input file name>.txt <output file name>.txt"<< endl;
		exit(-1);
	}

	ifstream input_file(argv[1]);
	ofstream output_file(argv[2]);

	Quine_McCluskey QQ;
	Petrick PP;
	vector<unsigned int> on_set;
	vector<implicant_struct> prime;
	sort_element_struct min_cov;
	int N;

	QQ.Init(input_file);
	QQ.solve();

	on_set = QQ.get_on_set();
	prime = QQ.get_prime();
	N = QQ.get_N();
	PP.Init(prime, on_set, N);
	min_cov = PP.solve();

	sort(prime.begin(), prime.end(), sort_by_output);
	sort(min_cov.min_cov.begin(), min_cov.min_cov.end(), sort_by_output);

	output_file << ".p " << prime.size() << endl;

	for (int i = 0; i < ((prime.size()>15)? 15 : prime.size()); i++) {
        print_output(N, prime[i], output_file);
		output_file << endl;
	}
	
	output_file << endl << ".mc " << min_cov.min_cov.size() << endl;

	for (int i = 0; i < min_cov.min_cov.size(); i++) {
        print_output(N, min_cov.min_cov[i], output_file);
		output_file << endl;
	}
	
	output_file << "literal=" << min_cov.min_cov.size()*N-min_cov.literal;
}