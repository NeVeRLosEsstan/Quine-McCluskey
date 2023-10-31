#include "Petrick.h"
#include "imp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;

bool sort_by_SOP(const sort_element_struct a, const sort_element_struct b) {
	if (a.min_cov.size() < b.min_cov.size())
        return true;
	else if (a.min_cov.size() > b.min_cov.size())
        return false;
	else 
		return (a.literal < b.literal);
}

bool Petrick::check_duplicate(vector<implicant_struct> x, implicant_struct y) {
	for (int i = 0; i < x.size(); i++) {	
		if (((x[i].data) == (y.data))&&((x[i].dashes) == (y.dashes))) {
			return true;
		}
	}
	return false;
}

bool Petrick::compare_prime_on(implicant_struct imp, unsigned int x){
	return ((imp.data ^ x) & (~(imp.dashes)))==0;
}

void Petrick::Init(vector<implicant_struct> prime_imp, vector<unsigned int> on_set, int x) {
	this->prime_implicants = prime_imp;
	this->on_set = on_set;
	this->N = x;
	for (int i = 0; i < this->prime_implicants.size(); i++) {
		map_element tmp;
		tmp.axis = i;

		for (int j = 0; j < this->on_set.size(); j++) {
			if (compare_prime_on(this->prime_implicants[i], this->on_set[j])) {
				tmp.x_loc.insert(j);
			}
		}

		if (!tmp.x_loc.empty()) {
			this->map_row.push_back(tmp);
		}
	}

	set<int>::iterator itr;
	for (int i = 0; i < this->on_set.size(); i++) {
		map_element tmp;
		tmp.axis = i;

		for (int j = 0; j < this->map_row.size(); j++) {
			itr = this->map_row[j].x_loc.find(i);
			if (itr != this->map_row[j].x_loc.end()) {
				tmp.x_loc.insert(this->map_row[j].axis);
			}
		}
		this->map_col.push_back(tmp);
	}
}

bool Petrick::EPI() {
	bool changed = false;
	set<int> col_2_b_del;
	for (int i = 0; i < this->map_col.size(); i++) {
		if (this->map_col[i].x_loc.size() == 1) {
			changed = true;
			int row = *this->map_col[i].x_loc.begin();
			if (!check_duplicate(this->essential_implicants, this->prime_implicants[row])) {
				this->essential_implicants.push_back(this->prime_implicants[row]);
			}
			for (int j = 0; j < this->map_row.size(); j++) {
				if (this->map_row[j].axis == row) {
					for (set<int>::iterator itr = this->map_row[j].x_loc.begin(); itr!= this->map_row[j].x_loc.end(); itr++) {
						col_2_b_del.insert(*itr);
					}
				}
			}
		}
	}
	for (set<int>::iterator itr = col_2_b_del.begin(); itr!= col_2_b_del.end(); itr++) {
		for (int k = 0; k < this->map_row.size(); k++) {
			if (this->map_row[k].x_loc.find(*itr) != this->map_row[k].x_loc.end()) {
				this->map_row[k].x_loc.erase(*itr);
			}
		}
		for (int k = 0; k < this->map_row.size(); k++) {
			if (this->map_row[k].x_loc.empty()) {
                this->map_row.erase(this->map_row.begin() + k);
				k--;
            }
		}
		for (int k = 0; k < this->map_col.size(); k++) {
			if (this->map_col[k].axis == *itr) {
				this->map_col.erase(this->map_col.begin() + k);
				k--;
			}
		}
	}
	return changed;
}

bool Petrick::RD() {
	bool changed = false;
	set<int> row_2_b_del;
	for (int i = 0; i < this->map_row.size(); i++) {
		for (int j = i+1; j < this->map_row.size(); j++) {
			if (i != j) {
				set<int> tmp;
				set_union(this->map_row[i].x_loc.begin(), this->map_row[i].x_loc.end(), this->map_row[j].x_loc.begin(), 
							this->map_row[j].x_loc.end(), inserter(tmp, tmp.begin()));
				if (tmp.size() == this->map_row[i].x_loc.size()) {
					if (this->map_row[i].x_loc.size() > this->map_row[j].x_loc.size()) {
						row_2_b_del.insert(this->map_row[j].axis);
						changed = true;
					}
					else if (this->map_row[i].x_loc.size() == this->map_row[j].x_loc.size()) {
						if (__builtin_popcount(this->prime_implicants[this->map_row[i].axis].dashes) <= 
							__builtin_popcount(this->prime_implicants[this->map_row[j].axis].dashes)) {
							row_2_b_del.insert(this->map_row[i].axis);
							changed = true;
						}
						else if (__builtin_popcount(this->prime_implicants[this->map_row[i].axis].dashes) > 
							__builtin_popcount(this->prime_implicants[this->map_row[j].axis].dashes)) {
							row_2_b_del.insert(this->map_row[j].axis);
							changed = true;
						}
					}
				}
				else if (tmp.size() == this->map_row[j].x_loc.size()) {
					if (this->map_row[i].x_loc.size() < this->map_row[j].x_loc.size()) {
						row_2_b_del.insert(this->map_row[i].axis);
						changed = true;
					}
					else if (this->map_row[i].x_loc.size() == this->map_row[j].x_loc.size()) {
						if (__builtin_popcount(this->prime_implicants[this->map_row[i].axis].dashes) <= 
							__builtin_popcount(this->prime_implicants[this->map_row[j].axis].dashes)) {
							row_2_b_del.insert(this->map_row[i].axis);
							changed = true;
						}
						else if (__builtin_popcount(this->prime_implicants[this->map_row[i].axis].dashes) > 
							__builtin_popcount(this->prime_implicants[this->map_row[j].axis].dashes)) {
							row_2_b_del.insert(this->map_row[j].axis);
							changed = true;
						}
					}
				}
			}
		}
		
	}
	for (set<int>::iterator itr = row_2_b_del.begin(); itr!= row_2_b_del.end(); itr++) {
		for (int k = 0; k < this->map_col.size(); k++) {
			if (this->map_col[k].x_loc.find(*itr) != this->map_col[k].x_loc.end()) {
				this->map_col[k].x_loc.erase(*itr);
			}
		}
		for (int k = 0; k < this->map_col.size(); k++) {
			if (this->map_col[k].x_loc.empty()) {
                this->map_col.erase(this->map_col.begin() + k);
				k--;
            }
		}
		for (int k = 0; k < this->map_row.size(); k++) {
			if (this->map_row[k].axis == *itr) {
				this->map_row.erase(this->map_row.begin() + k);
				k--;
			}
		}
	}
	return changed;
}

bool Petrick::CD() {
	bool changed = false;
	set<int> col_2_b_del;
	for (int i = 0; i < this->map_col.size()-1; i++) {
		for (int j = i+1; j < this->map_col.size(); j++) {
			set<int> tmp;
			set_union(this->map_col[i].x_loc.begin(), this->map_col[i].x_loc.end(), this->map_col[j].x_loc.begin(), 
						this->map_col[j].x_loc.end(), inserter(tmp, tmp.begin()));
			if (tmp.size() == this->map_col[i].x_loc.size()) {
				if (this->map_col[i].x_loc.size() > this->map_col[j].x_loc.size()) {
					col_2_b_del.insert(this->map_col[i].axis);
					changed = true;
				}
				else if (this->map_col[i].x_loc.size() == this->map_col[j].x_loc.size()) {
					col_2_b_del.insert(this->map_col[i].axis);
					changed = true;
				}
			}
		}
		
	}
	for (set<int>::iterator itr = col_2_b_del.begin(); itr!= col_2_b_del.end(); itr++) {
		for (int k = 0; k < this->map_row.size(); k++) {
			if (this->map_row[k].x_loc.find(*itr) != this->map_row[k].x_loc.end()) {
				this->map_row[k].x_loc.erase(*itr);
			}
		}
		for (int k = 0; k < this->map_row.size(); k++) {
			if (this->map_row[k].x_loc.empty()) {
                this->map_row.erase(this->map_row.begin() + k);
				k--;
            }
		}
		for (int k = 0; k < this->map_col.size(); k++) {
			if (this->map_col[k].axis == *itr) {
				this->map_col.erase(this->map_col.begin() + k);
				k--;
			}
		}
	}
	return changed;
}

sort_element_struct Petrick::solve() {
	bool epi_change = true;
	bool rd_change = true;
	bool cd_change = true;

	vector<sort_element_struct> sop_sort;

	while ((epi_change || rd_change || cd_change) && !this->map_row.empty()) {
		if (!this->map_row.empty() || !map_col.empty()) {
			epi_change = EPI();
		}
		if (!this->map_row.empty()) {
			rd_change = RD();
		}
		if (!this->map_col.empty()) {
			cd_change = CD();
		}
	}
	
	if (!map_row.empty()) {
		vector<int> axis;
		vector<vector<unsigned int> > map_bit_row (this->map_row.size(),  vector<unsigned int>((this->map_col.size()-1)/32+1));
		for (int i = 0; i < this->map_row.size(); i++) {
			axis.push_back(map_row[i].axis);
			for (int j = 0; j < this->map_col.size(); j++) {
                int k = j/32;
				map_bit_row[i][k]<<= 1;
				if (this->map_row[i].x_loc.find(map_col[j].axis) != this->map_row[i].x_loc.end()) {
					map_bit_row[i][k]++;
				}
            }
		}

		Node *root = new Node;
		root->mcov = vector<unsigned int>((this->map_col.size()-1)/32+1);
		root->map = map_bit_row;
		root->axis = axis;
		int min = 2147483647;
		queue<Node *> q;
		for (int i = map_bit_row.size()-1; i >= 0; i--) {
			int count = 0;
			for (int j = 0 ; j < map_bit_row[i].size(); j++) {
				count += __builtin_popcount(map_bit_row[i][j]);
			} 
			if (count != 0) {
				Node *tmp = new Node;
				tmp->imp.insert(axis[i]);
				tmp->axis = root->axis;
				tmp->map = root->map;

				for (int j = 0; j < root->mcov.size(); j++) {
					tmp->mcov.push_back(root->mcov[j] | map_bit_row[i][j]);
				}

				for (int j = 0; j < tmp->map.size(); j++) {
					int counter = 0;
					for (int k = 0; k < tmp->mcov.size(); k++) {
						tmp->map[j][k] = tmp->map[j][k] & (~(tmp->mcov[k]));
						counter += __builtin_popcount(tmp->map[j][k]);
					}
					if (counter == 0) {
						for (int k = 0; k < axis.size(); k++) {
							if (axis[k] == tmp->axis[j]) {
								map_bit_row[k] = vector<unsigned int>((this->map_col.size()-1)/32+1);
							}
						}
					}
				}
				tmp->start = i;
				root->children.push_back(tmp);
				q.push(tmp);
			}
		}
		delete root;
		while (!q.empty()) {
			Node *p = q.front();
			vector<vector<unsigned int> >ref_map = p->map;
			vector<int> ref_axis = p->axis;
			q.pop();
			for (int i = p->start-1; i >= 0; i--) {
				
				int count = 0;
				for (int j = 0 ; j < ref_map[i].size(); j++) {
					count += __builtin_popcount(ref_map[i][j]);
				} 
				if (count != 0) {
					Node *tmp = new Node;
					set<int> tmp2;
					int count = 0;
					tmp->imp = p->imp;
					if (tmp->imp.insert(ref_axis[i]).second) {
						tmp->axis = p->axis;
						tmp->map = p->map;
						for (int j = 0; j < p->mcov.size(); j++) {
							tmp->mcov.push_back(p->mcov[j] | ref_map[i][j]);
							count += __builtin_popcount(tmp->mcov[j]);
						}
						for (int j = 0; j < tmp->map.size(); j++) {
							int counter = 0;
							for (int k = 0; k < tmp->mcov.size(); k++) {
								tmp->map[j][k] = tmp->map[j][k] & (~(tmp->mcov[k]));
								counter += __builtin_popcount(tmp->map[j][k]);
							}
							if (counter == 0) {
								for (int k = 0; k < ref_axis.size(); k++) {
									if (ref_axis[k] == tmp->axis[j]) {
										ref_map[k] = vector<unsigned int>((this->map_col.size()-1)/32+1);
									}
								}
							}
						}
						tmp->start = i;
						if (count == map_col.size()) {
							if (tmp->imp.size() < min) {
								min = tmp->imp.size();
								all_min.clear();
							}
							if (tmp->imp.size() == min) {
								all_min.push_back(tmp->imp);
							}
						}
						if (tmp->imp.size() <= min) {
							p->children.push_back(tmp);
							q.push(tmp);
						}
						else {
							delete tmp;
						}
					}
					else {
                        delete tmp;
                    }
				}
			}
			delete p;
		}


		for (int i = 0 ; i < all_min.size(); i++) {
			sort_element_struct tmp;
			tmp.literal = 0;
			for (int j = 0; j < this->essential_implicants.size(); j++) {
				tmp.min_cov.push_back(this->essential_implicants[j]);
				tmp.literal += __builtin_popcount(this->essential_implicants[j].dashes);
			}
			for (set<int>::iterator it = all_min[i].begin(); it!= all_min[i].end(); it++) {
				tmp.min_cov.push_back(this->prime_implicants[*it]);
				tmp.literal += __builtin_popcount(this->prime_implicants[*it].dashes);
			}
            sop_sort.push_back(tmp);
		}
		
	}
	else {
		sort_element_struct tmp;
		tmp.literal = 0;
		for (int j = 0; j < this->essential_implicants.size(); j++) {
			tmp.min_cov.push_back(this->essential_implicants[j]);
			tmp.literal += __builtin_popcount(this->essential_implicants[j].dashes);
		}
		sop_sort.push_back(tmp);
	}

	sort(sop_sort.begin(), sop_sort.end(), sort_by_SOP);
		
	return sop_sort.back();
}
