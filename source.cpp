#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include "SBM.h"
using namespace std;

// Two examples

int main() {
	int p1 = 16;
	int p2 = 99;
	double* y = new double[p1 * p2];
	double* w = new double[p1 * p2];


	ifstream infile;
	infile.open("y2.txt");
	int count = 0;
	double tmp;
	while (!infile.eof())
	{
		infile >> tmp;
		y[count] = tmp; 
		count++;

	}
	infile.close(); 
	count = 0;
	ifstream infile2;
	infile2.open("w2.txt");
	while (!infile2.eof())
	{
		infile2 >> tmp;
		w[count] = tmp;
		count++;
	}
	infile2.close();

	orderedBlock* BT = new orderedBlock(p1 * p2, y, w);
	BT->load_2D_rectangle_isotonic_str(p1, p2);
	auto start11 = std::chrono::high_resolution_clock::now();
	auto finish11 = std::chrono::high_resolution_clock::now();
	for (int i = 0;i < 1;i++) {
		cout << i << ": ";
		BT->solve_by_SBM();
		finish11 = std::chrono::high_resolution_clock::now();
		cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish11 - start11).count();
		cout << " milliseconds" << endl;
	}
	finish11 = std::chrono::high_resolution_clock::now();
	cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish11 - start11).count();
}