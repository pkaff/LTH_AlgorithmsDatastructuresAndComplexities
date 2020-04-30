//============================================================================
// Name        : RailroadPlanning.cpp
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
#include "Edge.h"
#include "Node.h"
#include <climits>

using namespace std;

ostream& operator<<(ostream& os, const Node& obj) {
	os << "#" << obj.index << " named: " << obj.name << " connected to: " << endl;
	for (size_t i = 0; i < obj.edges.size(); ++i) {
		Edge* e = obj.edges[i];
		os << "#" << e->getEndNode()->getIndex() << " named: " << e->getEndNode()->getName() << ", capacity: " << e->getCapacity() << endl;
	}
	return os;
}

vector<int> DFS(vector<Node>& graph, vector<int>& visited, const int end) {
	int root = visited.back();

	vector<int> adjNode = graph[root].getAdjacentNodeID();

	for (int node : adjNode) {
		if (node == end && graph[root].getEdge(node)->getCapacity() != 0) {
			visited.push_back(node);
			return visited;
		}
	}

	vector<int> ret;
	size_t i;
	for (i = 0; i < adjNode.size(); ++i) {
		int node = adjNode[i];

		if (find(visited.begin(), visited.end(), node) != visited.end() || graph[root].getEdge(node)->getCapacity() == 0) {
			continue;
		}

		visited.push_back(node);

		vector<int> v = DFS(graph, visited, end);
		if (v.empty()) {
			visited.erase(visited.begin() + visited.size() - 1);
			ret = visited;
		} else {
			return v;
		}
	}
	if (i == adjNode.size()) {
		ret.clear();
	}
	return ret;
}

void minCut(vector<Node>& graph, vector<int>& visited, vector<pair<int, int> >& minCut) {
	vector<int> stack;
	stack.push_back(visited.back());
	while (!stack.empty()) {
		int node = stack.back();
		stack.pop_back();

		vector<int> adjNode = graph[node].getAdjacentNodeID();

		for (int n : adjNode) {
			if (find(visited.begin(), visited.end(), n) != visited.end() || graph[node].getEdge(n)->getCapacity() == 0) {
				continue;
			}
			visited.push_back(n);
			stack.push_back(n);
		}
	}
	for (int node : visited) {
		vector<int> adjNode = graph[node].getAdjacentNodeID();

		for (int n : adjNode) {
			if (find(visited.begin(), visited.end(), n) == visited.end()) {
				minCut.push_back(make_pair(node, n));
			}
		}
	}

}
int main() {
	string fileName = "rail.txt";

	//open file
	ifstream infile(fileName.c_str());

	//initialize variables
	string line;
	vector<Node> graph;

	int n;
	getline(infile, line);
	n = atoi(line.c_str());
	//read n nodes
	for (int i = 0; i < n; ++i) {
		if (getline(infile, line)) {
			Node node(line, i);
			graph.push_back(node);
		} else {
			cout << "Error in parsing nodes" << endl;
			return 1;
		}
	}

	int m;
	getline(infile, line);
	m = atoi(line.c_str());
	//read m edges
	for (int i = 0; i < m; ++i) {
		if (getline(infile, line)) {
			istringstream iss(line);
			int a, b, c;

			iss >> a >> b >> c;
			Edge* e1 = new Edge(&graph[a], &graph[b], c);
			Edge* e2 = new Edge(&graph[b], &graph[a], c);
			graph[a].addEdge(e1);
			graph[b].addEdge(e2);

		} else {
			cout << "Error in parsing edges" << endl;
			return 1;
		}
	}

	int maxFlow = 0;

	while (true) {
		vector<int> visited;
		visited.push_back(0); //start at node 0
		int end = graph[graph.size() - 1].getIndex(); //end at last node
		//Find simple path
		vector<int> path = DFS(graph, visited, end);

		if (path.back() != end) { //no more paths
			break;
		}

		int minCap = INT_MAX;
		//find bottleneck capacity
		for (size_t i = 0; i < path.size() - 1; ++i) {
			int node = path[i];
			int nextNode = path[i + 1];
			Edge* edge = graph[node].getEdge(nextNode);
			int cap = edge->getCapacity();
			if (cap != -1) {
				minCap = cap < minCap ? cap : minCap;
			}
		}

		//update capacities
		for (size_t i = 0; i < path.size() - 1; ++i) {
			int node = path[i];
			int nextNode = path[i + 1];
			Edge* edge = graph[node].getEdge(nextNode);
			Edge* edgeRev = graph[nextNode].getEdge(node);
			int cap = edge->getCapacity();
			if (cap != -1) {
				edge->pushFlow(minCap);
				edgeRev->addCapacity(minCap);

			}
		}


		maxFlow += minCap;
	}
	cout << "Maximum flow: " << maxFlow << endl;


	vector<int> visited;
	visited.push_back(0);
	vector<pair<int, int> > mc;
	minCut(graph, visited, mc);
	cout << "Min cut edges:" << endl;
	for (size_t i = 0; i < mc.size(); ++i) {
		int node = mc[i].first;
		int nextNode = mc[i].second;
		Edge* edge = graph[node].getEdge(nextNode);
		int cap = edge->getStartCapacity();
		cout << node << " " << nextNode << " " << cap << endl;
	}

	return 0;
}
