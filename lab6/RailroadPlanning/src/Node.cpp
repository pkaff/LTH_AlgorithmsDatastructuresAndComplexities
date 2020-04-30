/*
 * Node.cpp
 *
 *  Created on: 24 mar 2016
 *      Author: Kines
 */

#include "Node.h"
#include <iostream>
Node::Node() : name(""), index(-1) {};
Node::Node(std::string n, int i) : name(n), index(i) {};
Node::Node(const Node& other) : name(other.name), index(other.index), edges(other.edges) {};

Node::~Node() {
	for (Edge* e : edges) {
		delete[] e;
	}
};

void Node::addEdge(Edge* e) {
	edges.push_back(e);
}

std::vector<Edge*> Node::getEdges() {
	return edges;
}

Node& Node::operator=(const Node& rhs) {
	if (this == &rhs) return *this;
	name = rhs.name;
	index = rhs.index;
	edges = rhs.edges;
	return *this;
}

bool Node::operator==(const Node& rhs) {
	return index == rhs.index;
}

bool Node::operator!=(const Node& rhs) {
	return !(*this == rhs);
}

Edge* Node::getEdge(int other) {
	for (Edge* e : edges) {
		if (e->getEndNode()->index == other) {
			return e;
		}
	}
	return nullptr;
}

std::vector<int> Node::getAdjacentNodeID() {
	std::vector<int> ids;
	for (Edge* e : edges) {
		ids.push_back(e->getEndNode()->index);
	}
	return ids;
}

std::string Node::getName() {
	return name;
}

int Node::getIndex() {
	return index;
}


