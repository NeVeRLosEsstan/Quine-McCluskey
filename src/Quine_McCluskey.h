#ifndef QMK
#define QMK

#include "imp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

class Quine_McCluskey{
    private:
        int N;
        vector<unsigned int> on_set;
        vector<unsigned int> dc_set;
        vector<implicant_struct> implicants;
        vector<implicant_struct> prime_implicants;
        vector<vector<implicant_struct> > table;
        int get_num_of_1(implicant_struct);
        void create_table();
        bool table_empty();
        bool check_duplicate(vector<implicant_struct>, implicant_struct);
        void gen_prime_implicants();
        bool compare_implicants(implicant_struct, implicant_struct);
        implicant_struct combine_implicants(implicant_struct, implicant_struct);

    public:
        void Init(ifstream &);
        vector<implicant_struct> get_prime();
        vector<unsigned int> get_on_set();
        int get_N();
        void solve();

};

#endif