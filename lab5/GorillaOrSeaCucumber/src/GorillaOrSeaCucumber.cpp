//============================================================================
// Name        : GorillaOrSeaCucumber.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

static const int DIAG = 0;
static const int LEFT = 1;
static const int UP = 2;

vector<vector<pair<int, int> > > NWScoreMatrix(const string& dna1, const string& dna2, map<char, map<char, int> >& scores) {
	int n = dna1.size();
	int m = dna2.size();

	vector<vector<pair<int, int> > > M(n + 1, vector<pair<int, int> >(m + 1));

	int scoreSpace = scores['*']['A'];

	for (int i = 0; i <= n; ++i) {
		M[i][0] = make_pair(i * scoreSpace, UP);
	}

	for (int j = 0; j <= m; ++j) {
		M[0][j] = make_pair(j * scoreSpace, LEFT);
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			char x_i = dna1[i - 1];
			char y_j = dna2[j - 1];
			int match = scores[x_i][y_j] + M[i - 1][j - 1].first;
			int space2 = scoreSpace + M[i - 1][j].first;
			int space1 = scoreSpace + M[i][j - 1].first;
			if (match >= space2 && match >= space1) {
				M[i][j] = make_pair(match, DIAG);
			} else if (space2 >= space1 && space2 >= match) {
				M[i][j] = make_pair(space2, UP);
			} else {
				M[i][j] = make_pair(space1, LEFT);
			}
		}
	}

	return M;
}

//i and j starts at dna1.size() and dna2.size(), so access in dna is done at i - 1 and j - 1
pair<string, string> sequenceAlignmentHelp(int i, int j, const string& dna1, const string& dna2, map<char, map<char, int> >& scores, const vector<vector<pair<int, int> > >& NWScores) {
	string Z;
	string W;
	pair<string, string> recursiveSequence;
	if (i == 0 && j == 0) {
		Z = "";
		W = "";
	} else if (i == 0) {
		Z.push_back('-');
		W.push_back(dna2[j - 1]);
		recursiveSequence = sequenceAlignmentHelp(i, j - 1, dna1, dna2, scores, NWScores);
	} else if (j == 0) {
		Z.push_back(dna1[i - 1]);
		W.push_back('-');
		recursiveSequence = sequenceAlignmentHelp(i - 1, j, dna1, dna2, scores, NWScores);
	} else {
		switch (NWScores[i][j].second) {
		case DIAG:
			Z.push_back(dna1[i - 1]);
			W.push_back(dna2[j - 1]);
			recursiveSequence = sequenceAlignmentHelp(i - 1, j - 1, dna1, dna2, scores, NWScores);
			break;
		case UP:
			Z.push_back(dna1[i - 1]);
			W.push_back('-');
			recursiveSequence = sequenceAlignmentHelp(i - 1, j, dna1, dna2, scores, NWScores);
			break;
		case LEFT:
			Z.push_back('-');
			W.push_back(dna2[j - 1]);
			recursiveSequence = sequenceAlignmentHelp(i, j - 1, dna1, dna2, scores, NWScores);
			break;
		}
	}
	Z.insert(0, recursiveSequence.first);
	W.insert(0, recursiveSequence.second);
	return pair<string, string>(Z, W);
}

pair<pair<string, string>, int> sequenceAlignment(const string& dna1, const string& dna2, map<char, map<char, int> >& scores) {
	vector<vector<pair<int, int> > > NWScores = NWScoreMatrix(dna1, dna2, scores);

	return make_pair(sequenceAlignmentHelp(dna1.size(), dna2.size(), dna1, dna2, scores, NWScores), NWScores[dna1.size()][dna2.size()].first);
}


int main() {

	//score file
	string fileName = "BLOSUM62.txt";

	//open file
	ifstream infile(fileName.c_str());

	//initialize variables
	string line;

	map<char, map<char, int> > scores;
	vector<char> letters;
	//read each line
	while (getline(infile, line)) {
		if (line.find("#") == string::npos) {
			istringstream iss(line);
			if (line.substr(line.size() - 1) == "*") {
				char c;
				while (iss >> c) {
					letters.push_back(c);
				}
			} else {
				char c;
				iss >> c;
				int s;
				int letter = 0;
				while (iss >> s) {
					scores[c][letters[letter]] = s;
					++letter;
				}
			}
		}
	}

	//Read DNA
	//fileName = "Toy_FASTAs.in";
	fileName = "HbB_FASTAs.in";

	infile.close();
	infile.open(fileName.c_str());

	string specimen;
	string dna;
	vector<pair<string, string> > species;

	while (getline(infile, line)) {
		if (line.find('>') != string::npos) {
			if (!specimen.empty()) {
				species.push_back(pair<string, string>(specimen, dna));
				dna = "";
			}
			specimen = line.substr(1, line.size() - 2);
		} else {
			dna += line;
		}
	}
	//last species
	species.push_back(pair<string, string>(specimen, dna));

	for (size_t i = 0; i < species.size() - 1; ++i) {
		for (size_t j = 1; j < species.size() - i; ++j) {
			string s1 = species[i].first;
			string s2 = species[i + j].first;
			string dna1 = species[i].second;
			string dna2 = species[i + j].second;
			pair<pair<string, string>, int> alig = sequenceAlignment(dna1, dna2, scores);
			cout << s1 << "--" << s2 << ": " << alig.second << endl;
			cout << alig.first.first << endl;
			cout << alig.first.second << endl;
			cout << endl;
		}
	}

	return 0;
}
