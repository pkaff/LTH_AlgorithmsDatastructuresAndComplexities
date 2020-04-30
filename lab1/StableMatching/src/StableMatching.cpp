//============================================================================
// Name        : StableMatching.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main() {

	//open file
	//string fileName = "sm-friends.in";
	string fileName = "sm-illiad.in";
	//string fileName = "sm-bbt.in.txt";
	/*cout << "Please enter filename: ";
	cin >> fileName;
	cout << endl;*/

	ifstream infile(fileName.c_str());

	//initialize variables
	string line;
	int n = 0;
	vector<string> names;
	vector< vector<int> > preferences;

	//read each line
	while (getline(infile, line)) {
		//skip empty line
		if (line.empty()) {
			continue;
		}

		//skip commented
		string first;
		first = line.substr(0, 1);
		if (first != "#") {
			//read n
			if (first == "n") {
				n = atoi(line.substr(2).c_str());
				names.push_back(line); //dummy to get correct index
				vector<int> v;
				preferences.push_back(v); //dummy
			} else if (line.find(":") == string::npos) {
				//read names
				istringstream iss(line);
				string s;
				iss >> s >> s;
				names.push_back(s);
			} else {
				//read preferences
				istringstream iss(line);
				vector<int> pref(2*n + 1); //account for dummy for index

				int p;
				string s;
				int index = 1;
				pref[0] = 1; //current position in preference list, start at 1
				iss >> s; //ignore first indicator

				while (iss >> p) {
					pref[index++] = p;
				}
				preferences.push_back(pref);
			}
		}
	}

	for (size_t i = 2; i < preferences.size(); i += 2) {
		//invert women list
		vector<int> v = preferences[i];
		for (size_t j = 1; j < v.size(); ++j) {
			int n = v[j];
			preferences[i][n] = j;
		}
	}

	vector<int> wife;
	vector<int> husband;
	vector<int> freeMen;

	for (size_t i = 0; i < preferences.size(); ++i) {
		if (i % 2 != 0) {
			//add all men to free list
			freeMen.push_back(i);
		}
		wife.push_back(0);
		husband.push_back(0);
	}

	while (!freeMen.empty()) {

		int man = freeMen.back();
		freeMen.pop_back();
		vector<int> &manPref = preferences[man];
		int &currentProposal = manPref[0];
		int woman = manPref[currentProposal];
		vector<int> &womanPref = preferences[woman];

		//cout << names[man] << " proposed to " << names[woman] << endl;
		if (husband[woman] == 0) {
			//woman unmatched
			husband[woman] = man;
			wife[man] = woman;
		} else if (womanPref[man] < womanPref[husband[woman]]) {
			//woman prefers m to current partner
			int oldHusband = husband[woman];
			freeMen.push_back(oldHusband);
			wife[oldHusband] = 0;
			husband[woman] = man;
			wife[man] = woman;
		} else {
			//rejection
			freeMen.push_back(man);
		}
		//next proposal in preference, if proposed to all then
		if (currentProposal++ > n) {
			cout << "No stable matching. " << names[man] << " proposed to all women" << endl;
			return 0;
		}
	}

	cout << "Stable matching achieved:" << endl;
	for (size_t m = 1; m < names.size(); m += 2) {
		cout << names[m] << " -- " << names[wife[m]] << endl;
	}

	return 0;
}
