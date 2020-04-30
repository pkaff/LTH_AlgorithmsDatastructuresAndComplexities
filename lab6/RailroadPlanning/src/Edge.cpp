/*
 * Edge.cpp
 *
 *  Created on: 24 mar 2016
 *      Author: Kines
 */
#include "Edge.h"

Edge::Edge() : startNode(nullptr), endNode(nullptr), capacity(0), startCapacity(0) {};
Edge::Edge(Node* s, Node* e, int c) : startNode(s), endNode(e), capacity(c), startCapacity(c) {};
Edge::Edge(const Edge& other) {
	startNode = other.startNode;
	endNode = other.endNode;
	capacity = other.capacity;
	startCapacity = other.startCapacity;
};

void Edge::pushFlow(int f) {
	capacity -= f;
}

Node* Edge::getEndNode() {
	return endNode;
}

Node* Edge::getStartNode() {
	return startNode;
}

void Edge::addCapacity(int c) {
	if (capacity != -1) {
		capacity += c;
	}
}

int Edge::getCapacity() {
	return capacity;
}

int Edge::getStartCapacity() {
	return startCapacity;
}

