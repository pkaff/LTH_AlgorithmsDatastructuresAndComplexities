/*
 * Node.h

 *
 *  Created on: 24 mar 2016
 *      Author: Kines
 */

#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <vector>
#include <memory>
#include "Edge.h"

class Edge;

class Node {
public:

	Node();
	Node(std::string n, int i);
	Node(const Node& other);
	~Node();

	void addEdge(Edge* e);

	std::vector<Edge* > getEdges();

	Node& operator=(const Node& rhs);

	bool operator==(const Node& rhs);

	bool operator!=(const Node& rhs);

	Edge* getEdge(int other);

	std::vector<int> getAdjacentNodeID();

	std::string getName();

	int getIndex();

	friend std::ostream& operator<<(std::ostream& os, const Node& obj);

private:
	std::string name;
	int index;
	std::vector<Edge* > edges;
};

#endif /* NODE_H_ */
