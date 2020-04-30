//============================================================================
// Name        : WordLadders.cpp
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
#include <sstream>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

bool compareWord(const string& s, const string& t) {
	string a = s;
	string b = t;
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	int nbrDiff = 0;
	for (size_t i = 0; i < a.size(); ++i) {
		if (a[i] != b[i + nbrDiff]) {
			++nbrDiff;
			--i;
		}
		if (nbrDiff > 1) {
			return false;
		}
	}
	return true;
}

void insertInGraph(map<string, vector<string> >& graph, const string& w1, const string& w2) {
	//add thisWord to thatWord
	if (graph.count(w1) == 0) {
		//wasn't added before
		vector<string> v;
		v.push_back(w2);
		graph.insert(pair<string, vector<string> >(w1, v));
	} else {
		//was added before
		graph[w1].push_back(w2);
	}
}

pair<int, vector<string> > BFS(map<string, vector<string> >& graph, const string& from, const string& to) {
	vector<string> visited;
	visited.push_back(from);
	vector<vector<string> > paths;
	vector<string> temp;
	temp.push_back(from);
	paths.push_back(temp);
	int nLayers = 0;
	vector<vector<string> > neighbours;
	while (!paths.empty()) {
		vector<string> path = paths.back();
		paths.pop_back();
		string s = path.back();
		visited.push_back(s);
		if (s == to) {
			return make_pair(nLayers, path);
		}
		vector<string> n = graph[s];
		for (const string& w : n) {
			if (find(visited.begin(), visited.end(), w) == visited.end()) {
				//not visited, add to neighbours
				vector<string> newPath = path;
				newPath.push_back(w);
				neighbours.push_back(newPath);
			}
		}
		if (paths.empty()) {
			paths = neighbours;
			neighbours.clear();
			++nLayers;
		}
	}
	vector<string> v;
	return make_pair(-1, v);
}

int main() {

	//string fileName = "words-10.dat";
	//string testFileName = "words-10-test.in";
	string fileName = "words-250.dat";
	string testFileName = "words-250-test.in";
	//open file
	ifstream infile(fileName.c_str());

	//initialize variables
	string line;
	vector<string> words;

	//read each line
	while (getline(infile, line)) {
		words.push_back(line);
	}

	map<string, vector<string> > graph;
	for (size_t i = 0; i < words.size(); ++i) {
		string thisWord = words[i];
		string thisWordEnd = thisWord.substr(1);
		for (size_t j = i + 1; j < words.size(); ++j) {
			string thatWord = words[j];
			string thatWordEnd = thatWord.substr(1);
			if (compareWord(thisWordEnd, thatWord)) {
				insertInGraph(graph, thisWord, thatWord);
			}
			if (compareWord(thatWordEnd, thisWord)) {
				insertInGraph(graph, thatWord, thisWord);
			}
		}
	}
	infile.close();

	infile.open(testFileName.c_str());
	while (getline(infile, line)) {
		istringstream iss(line);
		string from;
		string to;
		iss >> from >> to;

		//BFS
		pair<int, vector<string> > p = BFS(graph, from, to);
		cout << p.first << " ";
		for (const string& s : p.second) {
			cout << s;
			if (s != p.second.back()) {
				 cout << "->";
			}
		}
		cout << endl;
	}


	return 0;
}
