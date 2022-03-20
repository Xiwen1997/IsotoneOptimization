#pragma once
# include <iostream>
# include <cmath>
# include <set>
# include <algorithm>
# include <iterator>
using namespace std;

class node {
public:
	node(int i) {
		index = i;
	}
	int index = 0;
	node* next = NULL;
};

class Adjacency_list {
public:
	Adjacency_list(int size) {
		n = size;
		node_ad_array = new node *[size];
		for (int i = 0;i < size;i++) {
			node_ad_array[i] = new node(i);
		}
	}
	~Adjacency_list() {
		for (int i = 0; i < n;i++) {
			node* cur_node = node_ad_array[i];
			node* temp_node = cur_node;
			while (cur_node->next != NULL) {
				temp_node = cur_node->next;
				delete cur_node;
				cur_node = temp_node;
			}
			delete cur_node;
		}
		delete[]node_ad_array;
	}

	Adjacency_list(const Adjacency_list& src_obj) {
		n = src_obj.n;
		node_ad_array = new node *[n];
		for (int i = 0;i < n;i++) {
			node_ad_array[i] = new node(i);
		}
		int ii = 0;
		for (int ii = 0;ii < n;ii++) {
			node* cur_node_obj = src_obj.node_ad_array[ii];
			node* cur_node = node_ad_array[ii];

			cur_node_obj = cur_node_obj->next;
			while (cur_node_obj != NULL) {
				cur_node->next = new node(cur_node_obj->index);
				cur_node_obj = cur_node_obj->next;
				cur_node = cur_node->next;
			}
		}

	}

	bool isEmpty(int i) {
		// check whether node i is connected with any other node
		return (node_ad_array[i]->next == NULL) ? true : false;
	}

	void connect(int i, int j) {
		// Connecting node i and node j
		if (node_ad_array[i]->next == NULL) {
			node_ad_array[i]->next = new node(j);
		}
		else {
			node* cur_node = node_ad_array[i]->next;
			while (cur_node->next != NULL && cur_node->index < j) {
				cur_node = cur_node->next;
			}
			node* temp_node = cur_node->next;
			cur_node->next = new node(j);
			cur_node->next->next = temp_node;
		}

		if (node_ad_array[j]->next == NULL) {
			node_ad_array[j]->next = new node(i);
		}
		else {
			node* cur_node = node_ad_array[j]->next;
			while (cur_node->next != NULL && cur_node->index < i) {
				cur_node = cur_node->next;
			}
			node* temp_node = cur_node->next;
			cur_node->next = new node(i);
			cur_node->next->next = temp_node;
		}
	}
	void disconnect(int i, int j) {
		// Disconnecting node i and node j
		bool not_connected = true;
		node* cur_node = node_ad_array[i]->next;
		node* pre_node = node_ad_array[i];
		while (cur_node->index != j) {
			pre_node = cur_node;
			cur_node = cur_node->next;
			if (cur_node == NULL) {
				not_connected = false;
				break;
			}
		}
		if (not_connected) {
			pre_node->next = cur_node->next;
			delete cur_node;
			cur_node = node_ad_array[j]->next;
			pre_node = node_ad_array[j];
			while (cur_node->index != i) {
				pre_node = cur_node;
				cur_node = cur_node->next;
			}
			pre_node->next = cur_node->next;
			delete cur_node;
		}

	}
	set<int> compute_subgraph(int i) {
		// Find all the subgraph where the node i is in. 
		set<int> searched_set;
		set<int> subgraph_set;
		// initialize searched set and subgraph set
		searched_set.insert(i);
		subgraph_set.insert(i);
		node* cur_node = node_ad_array[i]->next;
		while (cur_node != NULL) {
			subgraph_set.insert(cur_node->index);
			cur_node = cur_node->next;
		}

		while (searched_set.size() != subgraph_set.size()) {
			set<int> to_search_set(subgraph_set);
			set_difference(subgraph_set.begin(), subgraph_set.end(), searched_set.begin(), searched_set.end(), inserter(to_search_set, to_search_set.begin()));
			searched_set = subgraph_set;
			for (set<int>::iterator it = to_search_set.begin();it != to_search_set.end();it++) {
				cur_node = node_ad_array[*it]->next;
				while (cur_node != NULL) {
					subgraph_set.insert(cur_node->index);
					cur_node = cur_node->next;
				}
			}
		}
		return subgraph_set;
	}
	void print() {
		for (int i = 0;i < n;i++) {
			cout << i << " -> ";
			node* cur_node = node_ad_array[i]->next;
			if (cur_node == NULL) {
				cout << "()" << endl;
			}
			else {
				cout << "( ";
				while (cur_node != NULL) {
					cout << cur_node->index << " ";
					cur_node = cur_node->next;
				}
				cout << ")" << endl;
			}
		}
	}
	bool is_directly_connected(int i, int j) {
		bool connected = false;
		node* cur_node = node_ad_array[i]->next;
		while (cur_node != NULL) {
			if (cur_node->index == j) {
				connected = true;
				break;
			}
			cur_node = cur_node->next;
		}
		return connected;
	}
	bool is_connected(int i, int j) {
		set<int> searched_set;
		set<int> subgraph_set;
		// initialize searched set and subgraph set
		searched_set.insert(i);
		subgraph_set.insert(i);
		node* cur_node = node_ad_array[i]->next;
		while (cur_node != NULL) {
			subgraph_set.insert(cur_node->index);
			if (j == cur_node->index) {
				return true;
			}
			cur_node = cur_node->next;
		}

		while (searched_set.size() != subgraph_set.size()) {
			set<int> to_search_set(subgraph_set);
			set_difference(subgraph_set.begin(), subgraph_set.end(), searched_set.begin(), searched_set.end(), inserter(to_search_set, to_search_set.begin()));
			searched_set = subgraph_set;
			for (set<int>::iterator it = to_search_set.begin();it != to_search_set.end();it++) {
				cur_node = node_ad_array[*it]->next;
				while (cur_node != NULL) {
					subgraph_set.insert(cur_node->index);
					if (j == cur_node->index) {
						return true;
					}
					cur_node = cur_node->next;
				}
			}
		}
		return false;
	}
	void experiment() {
		connect(5, 11);
		connect(5, 12);
		connect(2, 5);
		connect(2, 6);
		connect(6, 13);
		set<int> inactive_set = compute_subgraph(5);
		for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
			cout << *it << " ";
		}
		cout << endl;
		disconnect(2, 6);
		inactive_set = compute_subgraph(5);
		for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
			cout << *it << " ";
		}
		cout << endl;
	}

	int n; // number of nodes
	node** node_ad_array; // the address of each node
};

class DAG_node {
public:
	DAG_node(int i) {
		index = i;
	}
	int index = 0;
	DAG_node* next = NULL;
	DAG_node* prev = NULL;
};

class DAG {
public:
	~DAG() {
		for (int i = 0; i < n;i++) {
			DAG_node* cur_node = node_array[i];
			DAG_node* temp_node = cur_node;
			while (cur_node->next != NULL) {
				temp_node = cur_node->next;
				delete cur_node;
				cur_node = temp_node;
			}
			temp_node = cur_node;
			while (cur_node->prev != NULL) {
				temp_node = cur_node->prev;
				delete cur_node;
				cur_node = temp_node;
			}
			delete cur_node;
		}
		delete[]node_array;
	}

	void initialize(int size) {
		n = size;
		node_array = new DAG_node *[size];
		for (int i = 0;i < size;i++) {
			node_array[i] = new DAG_node(i);
		}
	}

	void connect(int i, int j) {
		if (node_array[i]->next == NULL) {
			node_array[i]->next = new DAG_node(j);
		}
		else {
			DAG_node* cur_node = node_array[i]->next;
			while (cur_node->next != NULL && cur_node->index < j) {
				cur_node = cur_node->next;
			}
			DAG_node* temp_node = cur_node->next;
			cur_node->next = new DAG_node(j);
			cur_node->next->next = temp_node;
		}

		if (node_array[j]->prev == NULL) {
			node_array[j]->prev = new DAG_node(i);
		}
		else {
			DAG_node* cur_node = node_array[j]->prev;
			while (cur_node->prev != NULL && cur_node->index < i) {
				cur_node = cur_node->prev;
			}
			DAG_node* temp_node = cur_node->prev;
			cur_node->prev = new DAG_node(i);
			cur_node->prev->prev = temp_node;
		}
	}

	bool is_connected(int i, int j) {
		bool connected = false;
		DAG_node* cur_node = node_array[i]->next;
		while (cur_node != NULL) {
			if (cur_node->index == j) {
				connected = true;
				break;
			}
			cur_node = cur_node->next;
		}
		return connected;
	}

	set<int> get_children_set(int i) {
		set<int> children_set;
		DAG_node* cur_node = node_array[i]->next;
		while (cur_node != NULL) {
			children_set.insert(cur_node->index);
			cur_node = cur_node->next;
		}
		return children_set;
	}

	set<int> get_parents_set(int i) {
		set<int> parents_set;
		DAG_node* cur_node = node_array[i]->prev;
		while (cur_node != NULL) {
			parents_set.insert(cur_node->index);
			cur_node = cur_node->prev;
		}
		return parents_set;
	}

private:
	int n;
	DAG_node** node_array;
};


class ListNode {
public:
	int row_index;
	int col_index;
	ListNode* next;
	ListNode() {
		next = NULL;
	}
};

class List {
private:
	ListNode* first;
	int size;
public:
	List() {
		size = 0;
		first = NULL;
	}
	~List() {
		while (first != NULL) {
			ListNode* to_delete_node = first;
			first = to_delete_node->next;
			delete to_delete_node;
		}
	}
	void push(int col_index, int row_index) {
		ListNode* newNode = new ListNode();
		newNode->col_index = col_index;
		newNode->row_index = row_index;
		newNode->next = first;
		first = newNode;
		size++;
	}

	void pop(int& col_index, int& row_index) {
		col_index = first->col_index;
		row_index = first->row_index;
		ListNode* to_delete_node = first;
		first = to_delete_node->next;
		delete to_delete_node;
		size--;
	}
	/*
	bool find_col_index(int to_find_col_index, int& row_index_to_put) {
	bool found = false;
	ListNode* cur_node = first;
	while (!found && cur_node != NULL) {
	if (cur_node->col_index == to_find_col_index) {
	found = true;
	row_index_to_put = cur_node->row_index;
	}
	else {
	cur_node = cur_node->next;
	}
	}
	return found;
	}
	*/
	int get_size() {
		return size;
	}

};



class orderedBlock {
public:
	orderedBlock(int size, double* y = NULL, double* w = NULL) {
		n = size;
		xl = new double[n];
		calibrated_xl = new double[n];
		weights = new double[n];
		if (w != NULL) {
			weighted = true;
		}
		else {
			weighted = false;
		}
		if (y != NULL) {
			for (int i = 0;i < n;i++) {
				xl[i] = y[i];
				calibrated_xl[i] = y[i];
			}
			if (weighted) {
				for (int i = 0;i < n;i++) {
					weights[i] = w[i];
				}
			}
		}

		str_DAG = DAG();
		str_DAG.initialize(n);
	}

	~orderedBlock() {
		delete[]xl;
		delete[]calibrated_xl;
	}

	// You can define any isotonic constraints here
	void load_chain_isotonic_str() {
		m = n - 1;
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < m;i++) {
			str_DAG.connect(i, i + 1);
			E[E_count][0] = i;
			E[E_count][1] = i + 1;
			E_count++;
		}
	}
	 
	void load_2D_isotonic_str() {
		int nl = floor(sqrt(n));
		m = nl * nl - 2 * nl + n;
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < nl - 1;i++) {
			for (int j = 0;j < nl - 1;j++) {
				str_DAG.connect(j + nl * i, (j + 1) + nl * i);
				E[E_count][0] = j + nl * i;
				E[E_count][1] = (j + 1) + nl * i;
				E_count++;
				str_DAG.connect(j + nl * i, j + nl * (i + 1));
				E[E_count][0] = j + nl * i;
				E[E_count][1] = j + nl * (i + 1);
				E_count++;
			}
			str_DAG.connect(nl - 1 + nl * i, nl - 1 + nl * (i + 1));
			E[E_count][0] = nl - 1 + nl * i;
			E[E_count][1] = nl - 1 + nl * (i + 1);
			E_count++;
		}
		for (int j = nl * (nl - 1);j < n - 1;j++) {
			str_DAG.connect(j, j + 1);
			E[E_count][0] = j;
			E[E_count][1] = j + 1;
			E_count++;
		}
	}

	void load_2D_rectangle_isotonic_str(int p1, int p2) {
		m = 2 * p1 * p2 - p1 - p2;
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < p1 - 1;i++) {
			for (int j = 0;j < p2 - 1;j++) {
				str_DAG.connect(j + p2 * i, (j + 1) + p2 * i);
				E[E_count][0] = j + p2 * i;
				E[E_count][1] = (j + 1) + p2 * i;
				E_count++;
				str_DAG.connect(j + p2 * i, j + p2 * (i + 1));
				E[E_count][0] = j + p2 * i;
				E[E_count][1] = j + p2 * (i + 1);
				E_count++;
			}
			str_DAG.connect(p2 - 1 + p2 * i, p2 - 1 + p2 * (i + 1));
			E[E_count][0] = p2 - 1 + p2 * i;
			E[E_count][1] = p2 - 1 + p2 * (i + 1);
			E_count++;
		}
		for (int j = p2 * (p1 - 1);j < n - 1;j++) {
			str_DAG.connect(j, j + 1);
			E[E_count][0] = j;
			E[E_count][1] = j + 1;
			E_count++;
		}
	}

	void load_3D_rectangle_isotonic_str(int p1, int p2, int p3) {
		m = 3 * (2 * p1 * p2 - p1 - p2);
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < p1;i++) {
			for (int j = 0;j < p2;j++) {
				for (int k = 0;k < p3;k++) {
					if (i < p1 - 1) {
						E[E_count][0] = i * p2 * p3 + j * p3 + k;
						E[E_count][1] = (i + 1) * p2 * p3 + j * p3 + k;
						str_DAG.connect(E[E_count][0], E[E_count][1]);
						E_count++;
					}
					if (j < p2 - 1) {
						E[E_count][0] = i * p2 * p3 + j * p3 + k;
						E[E_count][1] = i * p2 * p3 + (j + 1) * p3 + k;
						str_DAG.connect(E[E_count][0], E[E_count][1]);
						E_count++;
					}
				}
			}
		}
	}

	void load_4D_rectangle_isotonic_str(int p1, int p2, int p3, int p4) {
		m = p3 * p4 * (2 * p1 * p2 - p1 - p2);
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < p1;i++) {
			for (int j = 0;j < p2;j++) {
				for (int k = 0;k < p3;k++) {
					for (int u = 0;u < p4;u++) {
						if (i < p1 - 1) {
							E[E_count][0] = i * p2 * p3 * p4 + j * p3 * p4 + k * p4 + u;
							E[E_count][1] = (i + 1) * p2 * p3 * p4 + j * p3 * p4 + k * p4 + u;
							str_DAG.connect(E[E_count][0], E[E_count][1]);
							E_count++;
						}
						if (j < p2 - 1) {
							E[E_count][0] = i * p2 * p3 * p4 + j * p3 * p4 + k * p4 + u;
							E[E_count][1] = i * p2 * p3 * p4 + (j + 1) * p3 * p4 + k * p4 + u;
							str_DAG.connect(E[E_count][0], E[E_count][1]);
							E_count++;
						}
					}

				}
			}
		}
	}

	void load_5D_rectangle_isotonic_str(int p1, int p2, int p3, int p4, int p5) {
		m = p3 * p4 * p5 * (2 * p1 * p2 - p1 - p2);
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < p1;i++) {
			for (int j = 0;j < p2;j++) {
				for (int k = 0;k < p3;k++) {
					for (int u = 0;u < p4;u++) {
						for (int o = 0;o < p5;o++) {
							if (i < p1 - 1) {
								E[E_count][0] = i * p2 * p3 * p4 * p5 + j * p3 * p4 * p5 + k * p4 * p5 + u * p5 + o;
								E[E_count][1] = (i + 1) * p2 * p3 * p4 * p5 + j * p3 * p4 * p5 + k * p4 * p5 + u * p5 + o;
								str_DAG.connect(E[E_count][0], E[E_count][1]);
								E_count++;
							}
							if (j < p2 - 1) {
								E[E_count][0] = i * p2 * p3 * p4 * p5 + j * p3 * p4 * p5 + k * p4 * p5 + u * p5 + o;
								E[E_count][1] = i * p2 * p3 * p4 * p5 + (j + 1) * p3 * p4 * p5 + k * p4 * p5 + u * p5 + o;
								str_DAG.connect(E[E_count][0], E[E_count][1]);
								E_count++;
							}
						}

					}

				}
			}
		}
	}

	void load_6D_rectangle_isotonic_str(int p1, int p2, int p3, int p4, int p5, int p6) {
		m = p3 * p4 * p5 * p6 * (2 * p1 * p2 - p1 - p2);
		E = new int*[m];
		for (int i = 0;i < m;i++) {
			E[i] = new int[2];
		}
		int E_count = 0;

		for (int i = 0;i < p1;i++) {
			for (int j = 0;j < p2;j++) {
				for (int k = 0;k < p3;k++) {
					for (int u = 0;u < p4;u++) {
						for (int o = 0;o < p5;o++) {
							for (int l = 0;l < p6;l++) {
								if (i < p1 - 1) {
									E[E_count][0] = i * p2 * p3 * p4 * p5 * p6 + j * p3 * p4 * p5 * p6 + k * p4 * p5 * p6 + u * p5 * p6 + o * p6 + l;
									E[E_count][1] = (i + 1) * p2 * p3 * p4 * p5 * p6 + j * p3 * p4 * p5 * p6 + k * p4 * p5 * p6 + u * p5 * p6 + o * p6 + l;
									str_DAG.connect(E[E_count][0], E[E_count][1]);
									E_count++;
								}
								if (j < p2 - 1) {
									E[E_count][0] = i * p2 * p3 * p4 * p5 * p6 + j * p3 * p4 * p5 * p6 + k * p4 * p5 * p6 + u * p5 * p6 + o * p6 + l;
									E[E_count][1] = i * p2 * p3 * p4 * p5 * p6 + (j + 1) * p3 * p4 * p5 * p6 + k * p4 * p5 * p6 + u * p5 * p6 + o * p6 + l;
									str_DAG.connect(E[E_count][0], E[E_count][1]);
									E_count++;
								}
							}

						}

					}

				}
			}
		}
	}
	 
	  
	void solve_by_SBM(bool remember_adj_gra = false) {
		outer_loop = 0;
		inner_loop = 0;
		for (int i = 0;i < n;i++) {
			calibrated_xl[i] = xl[i];
		}
		Adjacency_list* Adjacency_obj = new Adjacency_list(n);
		double* Ax = compute_Ax();
		int min_pos = whichMin(Ax, m);
		double min_value = Ax[min_pos];
		int i, j;
		double x_bar;
		double* local_lambda;
		set<int> block_set;
		while (min_value < -1e-9) {
			outer_loop++;
			i = E[min_pos][0];
			j = E[min_pos][1];
			if (Adjacency_obj->isEmpty(i) && Adjacency_obj->isEmpty(j)) {
				Adjacency_obj->connect(i, j);
				//cout << "Connecting " << i << "-" << j << endl;
				if (weighted) {
					x_bar = (xl[i] * weights[i] + xl[j] * weights[j]) / (weights[i] + weights[j]);
				}
				else {
					x_bar = 0.5 * (xl[i] + xl[j]);
				}
				calibrated_xl[i] = x_bar;
				calibrated_xl[j] = x_bar;
			}
			else {
				Adjacency_obj->connect(i, j);
				//cout << "Connecting " << i << "-" << j << endl;
				block_set = Adjacency_obj->compute_subgraph(i); // get all the index inside this block
				x_bar = get_x_bar(block_set);
				local_lambda = solve_local_KKT(block_set, x_bar, El, Adjacency_obj);
				for (set<int>::iterator it = block_set.begin();it != block_set.end();it++) {
					calibrated_xl[*it] = x_bar;
					// cout << "x[" << *it << "] = " << x_bar << endl;
				}
				if (get_min_value(local_lambda, block_set.size() - 1) < -1e-12) {
					// go to inner loop
					set<int> G_bar_index_set;
					for (int i = 0;i < block_set.size() - 1;i++) {
						G_bar_index_set.insert(i);
					}
					current_nbo = block_set.size();
					solve_inner_loop(G_bar_index_set, local_lambda, Adjacency_obj);
				}
				for (int i = 0;i < block_set.size() - 1;i++) {
					delete[]El[i];
				}
				delete[]El;
				delete[]local_lambda;
			}
			Compute_min_diff(min_value, min_pos, Ax);
		}
		if (remember_adj_gra) {
			//Adjacency_obj.print();
			if (warm_start_obj != NULL) {
				delete warm_start_obj;
			}
			warm_start_obj = Adjacency_obj;
		}
		else {
			delete Adjacency_obj;
		}
	}

	void solve_by_SBM_with_warm_start() {
		// for every index, re-compute the primal values
		bool* checked_array = new bool[n];
		for (int i = 0;i < n;i++) {
			checked_array[i] = false;
		}
		set<int> block_set;
		// now begin computing
		for (int it = 0;it < n;it++) {
			if (!checked_array[it]) {
				// otherwise this node has been recomputed
				block_set = warm_start_obj->compute_subgraph(it);
				double x_bar = get_x_bar(block_set);
				for (set<int>::iterator it = block_set.begin();it != block_set.end();it++) {
					calibrated_xl[*it] = x_bar;
					checked_array[*it] = true;
					// cout << "x[" << *it << "] = " << x_bar << endl;
				}
				if (block_set.size() > 2) {
					double* local_lambda = solve_local_KKT(block_set, x_bar, El, warm_start_obj);
					if (get_min_value(local_lambda, block_set.size() - 1) < -1e-12) {
						// go to inner loop
						set<int> G_bar_index_set;
						for (int i = 0;i < block_set.size() - 1;i++) {
							G_bar_index_set.insert(i);
						}
						current_nbo = block_set.size();
						solve_inner_loop(G_bar_index_set, local_lambda, warm_start_obj);
					}
					for (int i = 0;i < block_set.size() - 1;i++) {
						delete[]El[i];
					}
					delete[]El;
					delete[]local_lambda;
				}
			}
		}

		double* Ax = compute_Ax();
		int min_pos = whichMin(Ax, m);
		double min_value = Ax[min_pos];
		int i, j;
		double x_bar;
		double* local_lambda;
		while (min_value < -1e-9) {
			outer_loop++;
			i = E[min_pos][0];
			j = E[min_pos][1];
			if (warm_start_obj->isEmpty(i) && warm_start_obj->isEmpty(j)) {
				warm_start_obj->connect(i, j);
				//cout << "Connecting " << i << "-" << j << endl;
				if (weighted) {
					x_bar = (xl[i] * weights[i] + xl[j] * weights[j]) / (weights[i] + weights[j]);
				}
				else {
					x_bar = 0.5 * (xl[i] + xl[j]);
				}
				calibrated_xl[i] = x_bar;
				calibrated_xl[j] = x_bar;
			}
			else {
				warm_start_obj->connect(i, j);
				//cout << "Connecting " << i << "-" << j << endl;
				block_set = warm_start_obj->compute_subgraph(i); // get all the index inside this block
				x_bar = get_x_bar(block_set);
				local_lambda = solve_local_KKT(block_set, x_bar, El, warm_start_obj);
				for (set<int>::iterator it = block_set.begin();it != block_set.end();it++) {
					calibrated_xl[*it] = x_bar;
					// cout << "x[" << *it << "] = " << x_bar << endl;
				}
				if (get_min_value(local_lambda, block_set.size() - 1) < -1e-12) {
					// go to inner loop
					set<int> G_bar_index_set;
					for (int i = 0;i < block_set.size() - 1;i++) {
						G_bar_index_set.insert(i);
					}
					current_nbo = block_set.size();
					solve_inner_loop(G_bar_index_set, local_lambda, warm_start_obj);
				}
				for (int i = 0;i < block_set.size() - 1;i++) {
					delete[]El[i];
				}
				delete[]El;
				delete[]local_lambda;
			}
			Compute_min_diff(min_value, min_pos, Ax);
		}

	}

	void load_y_triangle(double**& Zt, int p1, int p2) {
		int ii = 0;
		int jj = 0;
		for (int i = 0;i < n;i++) {
			xl[i] = Zt[ii][jj];
			ii++;
			if (ii == p1) {
				ii = 0;
				jj++;
			}
			calibrated_xl[i] = xl[i];
		}
	}

	void write_triangle(double**& Xt, int p1, int p2) {
		int iii = 0;
		int jjj = 0;
		for (int i = 0;i < n;i++) {
			Xt[iii][jjj] = calibrated_xl[i];
			iii++;
			if (iii == p1) {
				iii = 0;
				jjj++;
			}
		}
	}



	void print_warm_obj() {
		warm_start_obj->print();
	}

	void solve_inner_loop(set<int> &inequality_indexset, double *lambda, Adjacency_list* Adjacency_obj) {
		set<int> current_equality_indexset;
		set<int> current_inequality_indexset;
		// initialization
		for (set<int>::iterator it = inequality_indexset.begin(); it != inequality_indexset.end();it++) {
			if (calibrated_xl[El[*it][0]] != calibrated_xl[El[*it][1]]) {
				current_inequality_indexset.insert(*it);
			}
			else {
				current_equality_indexset.insert(*it);
			}
		}
		bool local_kkt_solved = true;
		double min_lambda = 0;
		int min_lambda_pos = 0;
		int** El_i; //local_str_info 
		int** El_j; //local_str_info 
		for (set<int>::iterator it = inequality_indexset.begin(); it != inequality_indexset.end();it++) {
			if (lambda[*it] < min_lambda) {
				local_kkt_solved = false;
				min_lambda = lambda[*it];
				min_lambda_pos = *it;
			}
		}
		while (!local_kkt_solved) {
			inner_loop++;
			int to_delete_i = El[min_lambda_pos][0];
			int to_delete_j = El[min_lambda_pos][1];
			Adjacency_obj->disconnect(to_delete_i, to_delete_j);
			current_inequality_indexset.insert(min_lambda_pos);
			current_equality_indexset.erase(min_lambda_pos);

			// cout << "Disconnecting " << to_delete_i << "-" << to_delete_j << endl;
			lambda[min_lambda_pos] = 0;
			set<int> block_set_i = Adjacency_obj->compute_subgraph(to_delete_i);
			set<int> block_set_j = Adjacency_obj->compute_subgraph(to_delete_j);

			// re-optimize individual blocks, update x and lambda
			double x_bar_i = get_x_bar(block_set_i);
			double x_bar_j = get_x_bar(block_set_j);
			for (set<int>::iterator it = block_set_i.begin();it != block_set_i.end();it++) {
				calibrated_xl[*it] = x_bar_i;
			}
			for (set<int>::iterator it = block_set_j.begin();it != block_set_j.end();it++) {
				calibrated_xl[*it] = x_bar_j;
			}
			if (block_set_i.size() != 1) {
				double* local_lambda_i = solve_local_KKT(block_set_i, x_bar_i, El_i, Adjacency_obj);
				updata_local_lamdba(lambda, El, local_lambda_i, El_i, block_set_i.size() - 1);
			}
			if (block_set_j.size() != 1) {
				double* local_lambda_j = solve_local_KKT(block_set_j, x_bar_j, El_j, Adjacency_obj);
				updata_local_lamdba(lambda, El, local_lambda_j, El_j, block_set_j.size() - 1);
			}

			// repair primal feasibility if necessary
			int min_i = 0;
			int min_j = 0;
			bool is_still_primal_feasible = is_primal_feasible(El, current_nbo - 1, min_i, min_j);

			while (!is_still_primal_feasible) {
				Adjacency_obj->connect(min_i, min_j);
				//cout << "re-connect " << min_i << "-" << min_j << endl;
				set<int> subcurrent_set = Adjacency_obj->compute_subgraph(min_i);
				int** l_El = new int*[subcurrent_set.size() - 1];
				double l_x_bar = get_x_bar(subcurrent_set);
				double* l_local_lambda = solve_local_KKT(subcurrent_set, l_x_bar, l_El, Adjacency_obj);
				updata_local_lamdba(lambda, El, l_local_lambda, l_El, subcurrent_set.size() - 1);
				for (set<int>::iterator it = subcurrent_set.begin();it != subcurrent_set.end();it++) {
					calibrated_xl[*it] = l_x_bar;
				}
				for (int i = 0;i < subcurrent_set.size() - 1;i++) {
					delete[]l_El[i];
				}
				delete[]l_El;
				delete[]l_local_lambda;
				is_still_primal_feasible = is_primal_feasible(El, current_nbo - 1, min_i, min_j);
			}

			// Whether go to recursion
			min_lambda = 0;
			for (set<int>::iterator it = current_inequality_indexset.begin(); it != current_inequality_indexset.end();it++) {
				if (lambda[*it] < min_lambda) {
					min_lambda = lambda[*it];
					min_lambda_pos = *it;
				}
			}
			if (min_lambda < 0) {
				solve_inner_loop(current_inequality_indexset, lambda, Adjacency_obj);
			}

			local_kkt_solved = true;
			min_lambda = 0;
			for (set<int>::iterator it = inequality_indexset.begin(); it != inequality_indexset.end();it++) {
				if (lambda[*it] < min_lambda) {
					local_kkt_solved = false;
					min_lambda = lambda[*it];
					min_lambda_pos = *it;
				}
			}
		}
	}

	 
	int get_inner_loop_num() {
		return(inner_loop);
	}
	int get_outer_loop_num() {
		return(outer_loop);
	}
	 
	void print_xl() {
		for (int j = 0;j < n;j++) {
			cout << calibrated_xl[j] << " ";
		}
		cout << endl;
	}
	 
	void get_x_calibrated(double* x_to_write) {
		for (int i = 0;i < n;i++) {
			x_to_write[i] = calibrated_xl[i];
		}
	}


private:
	int n; // number of node
	int m; // number of constraints or edges
	double* xl; // data of every nodes
	double* calibrated_xl; // calibrated xl
	double* weights; // weights
	bool weighted;
	DAG str_DAG;
	Adjacency_list* warm_start_obj;

	int outer_loop;
	int inner_loop;
	int current_nbo = 0;

	int** E; // constraints input matrix
	int** El; //local_str_info
	double* compute_Ax() {
		double* Ax = new double[m];
		for (int i = 0;i < m;i++) {
			Ax[i] = -calibrated_xl[E[i][0]] + calibrated_xl[E[i][1]];
		}
		return Ax;
	}

	int whichMin(double* x, int size) {
		int min_pos;
		min_pos = 0;
		for (int i = 1;i < size;i++) {
			if (x[i] < x[min_pos]) {
				min_pos = i;
			}
		}
		return min_pos;
	}
	double get_min_value(double* x, int size) {
		double min_value = x[0];
		for (int i = 1;i < size;i++) {
			if (x[i] < min_value) {
				min_value = x[i];
			}
		}
		return min_value;
	}
	double get_x_bar(set<int> inactive_set) {
		double x_bar = 0.0;
		if (weighted) {
			double w_sum = 0.0;
			for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
				x_bar = x_bar + weights[*it] * xl[*it];
				w_sum = w_sum + weights[*it];
			}
			x_bar = x_bar / w_sum;
		}
		else {
			for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
				x_bar = x_bar + xl[*it];
			}
			x_bar = x_bar / inactive_set.size();
		}
		return x_bar;
	}
	  
	void Compute_min_diff(double& min_value, int& min_pos, double*& Ax) {
		min_value = 0.0;
		for (int i = 0;i < m;i++) {
			Ax[i] = -calibrated_xl[E[i][0]] + calibrated_xl[E[i][1]];
			if (Ax[i] < min_value) {
				min_value = Ax[i];
				min_pos = i;
			}
		}
	}

	set<int> get_violator_set(double* x) {
		set<int> violator_set;
		for (int i = 0;i < m;i++) {
			if (-x[E[i][0]] + x[E[i][1]] < -1e-9) {
				violator_set.insert(i);
			}
		}
		return violator_set;
	}

	double partial_fi(int i, double x_i) {
		if (weighted) {
			return(weights[i] * (x_i - xl[i]));
		}
		else {
			return(x_i - xl[i]);
		}
	}

	void updata_local_lamdba(double*& big_local_lambda, int** El_big, double*& small_local_lambda, int** El_small, int small_n) {
		int countEl;
		for (int ii = 0; ii < small_n;ii++) {
			countEl = 0;
			while (!(El_big[countEl][0] == El_small[ii][0] && El_big[countEl][1] == El_small[ii][1]))
			{
				countEl++;
			}
			big_local_lambda[countEl] = small_local_lambda[ii];
		}
	}

	double* solve_local_KKT(set<int>& inactive_set, double& x_bar, int**& El, Adjacency_list* Adjacency_obj) {
		int nl = inactive_set.size() - 1;
		double** Al = new double*[nl];
		double* bl = new double[nl];
		El = new int*[nl];
		for (int i = 0;i < nl;i++) {
			El[i] = new int[2];
			Al[i] = new double[nl];
			for (int j = 0;j < nl;j++) {
				Al[i][j] = 0;
			}
			bl[i] = 0;
			El[i][0] = 0;
			El[i][1] = 0;
		}
		int count_El = 0;
		int* temp_array = new int[nl + 1];
		for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
			temp_array[count_El] = int(*it);
			count_El++;
		}
		int* row_abs_sum = new int[nl];
		int* unique_col_index = new int[nl];
		for (int i = 0;i < nl;i++) {
			row_abs_sum[i] = 0;
		}
		count_El = 0;
		int ii, jj;
		for (int i = 0;i < inactive_set.size();i++) {
			for (int j = 0; j < inactive_set.size();j++) {
				if (i != j) {
					ii = temp_array[i];
					jj = temp_array[j];
					if (str_DAG.is_connected(ii, jj) && Adjacency_obj->is_directly_connected(ii, jj)) {
						El[count_El][0] = temp_array[i];
						El[count_El][1] = temp_array[j];
						if (i < nl) {
							Al[i][count_El] = 1;
							row_abs_sum[i] = row_abs_sum[i] + 1;
							unique_col_index[i] = count_El;
						}
						if (j < nl) {
							Al[j][count_El] = -1;
							row_abs_sum[j] = row_abs_sum[j] + 1;
							unique_col_index[j] = count_El;
						}
						//cout << El[count_El][0] << " " << El[count_El][1] << endl;
						count_El++;
					}
				}
			}
		}
		delete[]temp_array;
		int count_eq = 0;
		for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
			if (count_eq < nl) {
				bl[count_eq] = -partial_fi(*it, x_bar);
				count_eq++;
			}
		}
		double* local_lambda = solve_linear_system(Al, bl, nl);
		 
		for (int i = 0;i < nl;i++) {
			delete[]Al[i];
		}
		delete[]Al;
		delete[]bl;
		delete[]unique_col_index;

		return local_lambda;
	}

	double* solve_linear_system(double**& Al, double*& bl, int& nl) {
		double** Aug = new double*[nl];
		for (int i = 0;i < nl;i++) {
			Aug[i] = new double[nl + 1];
			for (int j = 0;j < nl;j++) {
				Aug[i][j] = Al[i][j];
			}
			Aug[i][nl] = bl[i];
		}
		int k;
		double max, temp, l, s;
		for (int i1 = 0; i1 < nl - 1; i1++)
		{
			max = fabs(Aug[i1][i1]);
			k = i1;
			for (int i = i1; i < nl; i++)
			{
				if (max < fabs(Aug[i][i1]))
				{
					max = fabs(Aug[i][i1]);
					k = i;
				}
			}

			for (int j = i1; j < nl + 1; j++)
			{
				temp = Aug[i1][j];
				Aug[i1][j] = Aug[k][j];
				Aug[k][j] = temp;
			}

			for (k = i1 + 1; k < nl; k++)
			{
				l = -Aug[k][i1] / Aug[i1][i1];
				for (int j = i1; j < nl + 1; j++)
					Aug[k][j] = Aug[k][j] + l * Aug[i1][j];
			}
		}

		double* x_star = new double[nl];
		x_star[nl - 1] = Aug[nl - 1][nl] / Aug[nl - 1][nl - 1];

		for (int i = nl - 2; i >= 0; i = i - 1)
		{
			s = 0;
			for (int j = i + 1; j < nl; j++)
				s = s + Aug[i][j] * x_star[j];
			x_star[i] = (Aug[i][nl] - s) / Aug[i][i];
		}

		for (int i = 0;i < nl;i++) {
			delete[]Aug[i];
		}
		delete[]Aug;

		return x_star;
	}

	bool is_primal_feasible(set<int> inactive_set) {
		bool is_feasible = true;
		for (set<int>::iterator it = inactive_set.begin();it != inactive_set.end();it++) {
			for (set<int>::iterator it2 = inactive_set.begin();it2 != inactive_set.end();it2++) {
				if (str_DAG.is_connected(*it, *it2) && calibrated_xl[*it] - calibrated_xl[*it2] > 10e-9) {
					is_feasible = false;
				}
			}
		}
		return is_feasible;
	}

	bool is_primal_feasible(int**& El, int nl, int& min_i, int& min_j) {
		double min_value = 0.0;
		for (int i = 0; i < nl;i++) {
			if (-calibrated_xl[El[i][0]] + calibrated_xl[El[i][1]] < min_value) {
				min_value = -calibrated_xl[El[i][0]] + calibrated_xl[El[i][1]];
				min_i = El[i][0];
				min_j = El[i][1];
			}
		}
		return (min_value >= 0) ? true : false;
	}
};



// A class for isotonic multivariate linear regression 
class IMLR {
public:
	IMLR(int mi, int ni, int pi, double** Ai, double** Yi) {
		A = Ai; // hard copy
		Y = Yi; // hard copy
		m = mi;
		n = ni;
		p = pi;
		calibrated_X = new double*[p];
		for (int i = 0;i < p;i++) {
			calibrated_X[i] = new double[m];
		}
		ATY = new double*[p];
		ATA = new double*[p];
		for (int i = 0;i < p;i++) {
			ATY[i] = new double[m];
			ATA[i] = new double[p];
		}
		BT = new orderedBlock(p * m);
		BT->load_2D_rectangle_isotonic_str(p, m);
		// compute ATY and ATA
		for (int i = 0;i < p;i++) {
			for (int j = 0;j < p;j++) {
				ATA[i][j] = 0;
				for (int k = 0;k < n;k++) {
					ATA[i][j] = ATA[i][j] + A[k][i] * A[k][j];
				}
			}
			for (int j = 0;j < m;j++) {
				ATY[i][j] = 0;
				for (int k = 0;k < n;k++) {
					ATY[i][j] = ATY[i][j] + A[k][i] * Y[k][j];
				}
			}
		}
	}
	~IMLR() {
		for (int i = 0;i < n;i++) {
			delete[]A[i];
		}
		delete[]A;
		for (int i = 0;i < m;i++) {
			delete[]Y[i];
		}
		delete[]Y;
	}
	void solve(double initial_eta = 0.001, double beta1 = 0.5, double beta2 = 0.9, bool warm_ups = false) {
		double** Zt = new double*[p];
		double** Xt = new double*[p];
		double** gt = new double*[p]; // gradient of Xt
		double** Xt_next = new double*[p];
		for (int i = 0;i < p;i++) {
			Xt[i] = new double[m];
			gt[i] = new double[m];
			Zt[i] = new double[m];
			Xt_next[i] = new double[m];
			for (int j = 0;j < m;j++) {
				Xt[i][j] = 0;
				Xt_next[i][j] = 0;
				Zt[i][j] = 0;
				gt[i][j] = 0;
			}
		}

		double cur_obj = get_obj(Xt);
		//cout << cur_obj << endl;
		double next_obj;
		double eta = initial_eta;

		compute_gradient(gt, Xt);
		//for (int i = 0;i < p;i++) {
		//	for (int j = 0;j < m;j++) {
		//		cout << gt[i][j] << " ";
		//	}
		//	cout << endl;
		//}

		do_gradient_descent(eta, Zt, Xt, gt);

		//for (int i = 0;i < p;i++) {
		//	for (int j = 0;j < m;j++) {
		//		cout << Zt[i][j] << " ";
		//	}
		//	cout << endl;
		//}
		BT->load_y_triangle(Zt, p, m);
		BT->solve_by_SBM(true);
		BT->write_triangle(Xt_next, p, m);
		//for (int i = 0;i < p;i++) {
		//	for (int j = 0;j < m;j++) {
		//		cout << Xt_next[i][j] << " ";
		//	}
		//	cout << endl;
		//}

		next_obj = get_obj(Xt_next);

		double twiceExpands = twice_expands(cur_obj, gt, Xt, Xt_next, eta);
		while (next_obj > twiceExpands) {
			eta = eta * beta1;
			do_gradient_descent(eta, Zt, Xt, gt);
			BT->load_y_triangle(Zt, p, m);
			if (warm_ups) {
				BT->solve_by_SBM_with_warm_start();
			}
			else {
				BT->solve_by_SBM();
			}
			BT->write_triangle(Xt_next, p, m);
			next_obj = get_obj(Xt_next);
			twiceExpands = twice_expands(cur_obj, gt, Xt, Xt_next, eta);
			//cout << next_obj << endl;
		}

		cur_obj = next_obj;

		double Xt_diff = 1;
		int beta_not_change_times = 0;

		while (Xt_diff > 1e-4) {
			double** Xt_temp = Xt;
			Xt = Xt_next;
			Xt_next = Xt_temp;

			cur_obj = next_obj;
			//cout << "Current obj value: " << cur_obj << endl;
			compute_gradient(gt, Xt);
			do_gradient_descent(eta, Zt, Xt, gt);
			BT->load_y_triangle(Zt, p, m);
			if (warm_ups) {
				BT->solve_by_SBM_with_warm_start();
			}
			else {
				BT->solve_by_SBM();
			}
			BT->write_triangle(Xt_next, p, m);
			/*
			for (int i = 0;i < p;i++) {
			for (int j = 0;j < m;j++) {
			cout << Xt_next[i][j] << " ";
			}
			cout << endl;
			}
			*/
			next_obj = get_obj(Xt_next);
			twiceExpands = twice_expands(cur_obj, gt, Xt, Xt_next, eta);
			if (next_obj < twiceExpands) {
				beta_not_change_times++;
				if (beta_not_change_times >= 3) {
					beta_not_change_times = 2;
					eta = eta / beta2; // increase learning rate
				}
			}
			else {
				// decrease learning rate
				while (next_obj >= twiceExpands) {
					beta_not_change_times = 0;
					eta = eta * beta2;
					do_gradient_descent(eta, Zt, Xt, gt);
					BT->load_y_triangle(Zt, p, m);
					if (warm_ups) {
						BT->solve_by_SBM_with_warm_start();
					}
					else {
						BT->solve_by_SBM();
					}
					BT->write_triangle(Xt_next, p, m);
					next_obj = get_obj(Xt_next);
					//cout << next_obj << endl;
					twiceExpands = twice_expands(cur_obj, gt, Xt, Xt_next, eta);
				}
			}
			//cout << get_obj(Xt_next) << endl;
			Xt_diff = sqrt(diff_Fnorm(Xt, Xt_next) / Fnorm(Xt));
		}

		for (int i = 0;i < p;i++) {
			for (int j = 0;j < m;j++) {
				calibrated_X[i][j] = Xt[i][j];
			}
		}



	}




private:
	int m;
	int n;
	int p;
	double** A; // n*p
	double** Y; // n*m
	double** ATY; // p*m
	double** ATA; // p*p
	double** calibrated_X;

	orderedBlock* BT;


	double get_obj(double**& Xt) {
		// \| y-Ax \|_2^2
		double sum = 0;
		double ri = 0;
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < m;j++) {
				ri = Y[i][j];
				for (int k = 0;k < p;k++) {
					ri = ri - A[i][k] * Xt[k][j];
				}
				sum = sum + ri * ri;
			}
		}
		return 0.5 * sum;
	}

	void compute_gradient(double**& g, double** Xl) {
		if (g == NULL) {
			g = new double*[p];
			for (int i = 0;i < p;i++) {
				g[i] = new double[m];
			}
		}
		// g =  t(A) * A * X - A * y

		for (int i = 0;i < p;i++) {
			for (int j = 0;j < m;j++) {
				g[i][j] = -ATY[i][j];
				for (int k = 0;k < p;k++) {
					g[i][j] = g[i][j] + ATA[i][k] * Xl[k][j];
				}
			}
		}
	}

	void do_gradient_descent(double eta, double** Zt, double** Xt, double** gt) {
		for (int i = 0;i < p;i++) {
			for (int j = 0;j < m;j++) {
				Zt[i][j] = Xt[i][j] - eta * gt[i][j];
			}
		}
	}


	double diff_Fnorm(double**& X1, double**& X2) {
		double sum = 0;
		double dxi;
		for (int i = 0; i < p;i++) {
			for (int j = 0;j < m;j++) {
				dxi = X1[i][j] - X2[i][j];
				sum = sum + dxi * dxi;
			}
		}
		return sum;
	}
	double Fnorm(double**& X) {
		double sum = 0;
		double dxi;
		for (int i = 0; i < p;i++) {
			for (int j = 0;j < m;j++) {
				sum = sum + X[i][j] * X[i][j];
			}
		}
		return sum;
	}

	double twice_expands(double fxt, double**& gt, double**& Xt, double**& Xt_next, double eta) {
		double sum = fxt;
		double dxi;
		for (int i = 0;i < p;i++) {
			for (int j = 0;j < m;j++) {
				dxi = Xt_next[i][j] - Xt[i][j];
				sum = sum + (gt[i][j] + dxi / (2 * eta)) * dxi;
			}
		}
		return sum;
	}
};


