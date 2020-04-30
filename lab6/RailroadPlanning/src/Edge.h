/*
 * Edge.h
 *
 *  Created on: 24 mar 2016
 *      Author: Kines
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "Node.h"

class Node;

class Edge {
public:
	Edge();
	Edge(Node* s, Node* e, int c);
	Edge(const Edge& other);

	void pushFlow(int f);

	Node* getEndNode();

	Node* getStartNode();

	void addCapacity(int c);

	int getCapacity();

	int getStartCapacity();

private:
	Node* startNode;
	Node* endNode;
	int capacity;
	int startCapacity;
};

#endif /* EDGE_H_ */
