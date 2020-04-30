//============================================================================
// Name        : SpanningUSA.cpp
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
#include <map>
#include <algorithm>
#include <utility>
#include <queue>
#include <climits>

using namespace std;

class CompareDist {
public:
	bool operator()(pair<string, pair<string, int> > c1, pair<string, pair<string, int> > c2) {
		return c1.second.second > c2.second.second;
	}
};

void update_queue(vector<pair<string, pair<string, int> > >& queue, const vector<pair<string, int> >& edges) {
	for (pair<string, int> e : edges) {
		for (pair<string, pair<string, int> >& q : queue) {
			if (q.first == e.first && q.second.second > e.second) {
				q.second = e;
				break;
			}
		}
	}
	sort(queue.begin(), queue.end(),
			[] (const pair<string, pair<string, int> >& a, const pair<string, pair<string, int> >& b) -> bool
			{
			return a.second.second < b.second.second;
			}
	);
}

int main() {

	string fileName = "USA-highway-miles.in";

	//open file
	ifstream infile(fileName.c_str());

	//initialize variables
	string line;

	map<string, vector<pair<string, int> > > cities;
	int nCities = 0;
	//read each line
	while (getline(infile, line)) {
		if (line.find("--") == string::npos) {
			line.erase(remove(line.begin(), line.end(), '\"'), line.end());
			line.erase(remove(line.end() - 1, line.end(), ' '), line.end());
			vector<pair<string, int> > connections;
			cities.insert(pair<string, vector<pair<string, int> > >(line, connections));
			nCities++;
		} else {
			string cityPart;
			string intPart;
			int dist;
			cityPart = line.substr(0, line.find("[") - 1);
			intPart = line.substr(line.find("["));
			string c1;
			string c2;
			c1 = cityPart.substr(0, cityPart.find("--"));
			c2 = cityPart.substr(cityPart.find("--") + 2);
			c1.erase(remove(c1.begin(), c1.end(), '\"'), c1.end());
			c2.erase(remove(c2.begin(), c2.end(), '\"'), c2.end());
			intPart = intPart.substr(1, intPart.size() - 2);
			dist = atoi(intPart.c_str());
			cities[c1].push_back(pair<string, int>(c2, dist));
			cities[c2].push_back(pair<string, int>(c1, dist));
		}
	}

	//prim
	vector<pair<string, pair<string, int> > > queue;
	map<string, vector<pair<string, int> > > mst;

	//insert arbitrary node with value 0 as "start node"
	pair<string, pair<string, int> > city;
	city = make_pair("San Diego", pair<string, int>(" ", 0));
	queue.push_back(city);

	//insert rest of nodes with INT_MAX value
	for (const auto& ct : cities) {
		if (ct.first != city.first) {
			pair<string, int> p = make_pair(" ", INT_MAX);
			queue.push_back(pair<string, pair<string, int> >(ct.first, p));
		}
	}

	while (!queue.empty()) {
		pair<string, pair<string, int> > v = queue.front();
		queue.erase(queue.begin());
		vector<pair<string, int> > edges = cities[v.first];
		vector<pair<string, int> > vec;
		if (mst.count(v.first) == 1) {
			mst[v.first].push_back(v.second);
		} else {
			vec.push_back(v.second);
			mst.insert(pair<string, vector<pair<string, int> > >(v.first, vec));
		}
		update_queue(queue, edges);
	}

	int treeSize = 0;
	for (const auto& m : mst) {
		for (const auto& p : m.second) {
			treeSize += p.second;
		}
	}
	cout << "MST size: " << treeSize << endl;

	return 0;
}
