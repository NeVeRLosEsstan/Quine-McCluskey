#ifndef IMP_H
#define IMP_H

#include <vector>

using namespace std;

typedef struct implicant {
    unsigned int data;
    unsigned int dashes;
} implicant_struct;

typedef struct sort_element {
	int literal;
	vector<implicant_struct> min_cov;
} sort_element_struct;

#endif