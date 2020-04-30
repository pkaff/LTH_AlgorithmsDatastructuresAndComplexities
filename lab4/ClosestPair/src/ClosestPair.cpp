//============================================================================
// Name        : ClosestPair.cpp
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
#include <sstream>
#include <cmath>
#include <cstdlib>

using namespace std;

class Point {
public:
	int name;
	double x;
	double y;

	Point() : name(0), x(0), y(0) {};
	Point(int nbr, double xP, double yP) : name(nbr), x(xP), y(yP) {};
	double dist(const Point& other) const {
		double dx = other.x - this->x;
		double dy = other.y - this->y;
		return sqrt(pow(dx, 2) + pow(dy, 2));
	}
	Point& operator=(const Point& rhs) {
		if (this == &rhs) return *this;
		name = rhs.name;
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
};

//recursive algorithm returns a pair of closest points
pair<Point, Point> closestPair(int begin, int end, const vector<Point>& xSort, vector<Point> ySort) {
	if ((end - begin) == 1) {
		//base case with 2 points
		return pair<Point, Point>(xSort[begin], xSort[end]);
	} else if ((end - begin) == 2) {
		//base case with 3 points - manually compare the three
		if (xSort[begin].dist(xSort[begin + 1]) < xSort[begin + 1].dist(xSort[end])) {
			return pair<Point, Point>(xSort[begin], xSort[begin + 1]);
		} else {
			return pair<Point, Point>(xSort[begin + 1], xSort[end]);
		}
	} else {
		//calculate separation line, find smallest dist of two halves
		int separationLineX;
		separationLineX = (xSort[(begin + end)/2].x + xSort[(begin + end)/2 + 1].x)/2;
		pair<Point, Point> d1 = closestPair(begin, (begin + end)/2, xSort, ySort);
		pair<Point, Point> d2 = closestPair((begin + end)/2 + 1, end, xSort, ySort);
		pair<Point, Point> delta;
		if (d1.first.dist(d1.second) < d2.first.dist(d2.second)) {
			delta = d1;
		} else {
			delta = d2;
		}
		//remove all values > delta from separation line in the vector sorted by y values
		ySort.erase(remove_if(ySort.begin(), ySort.end(), [separationLineX, delta](const Point& p) {
			return abs(separationLineX - p.x) > delta.first.dist(delta.second) ? true : false;
		}), ySort.end());

		//check points near the line and only 11 of them in y coord order
		for (size_t i = 0; i < ySort.size(); ++i) {
			for (size_t j = 1; j <= 11; ++j) {
				if (i + j >= ySort.size()) {
					break;
				}
				if (ySort[i].dist(ySort[i + j]) < delta.first.dist(delta.second)) {
					delta = pair<Point, Point>(ySort[i], ySort[i + j]);
				}
			}
		}
		return delta;
	}
}

int main() {
	//string fileName = "ch150.tsp";
	string fileName = "ch130.tsp";

	//open file
	ifstream infile(fileName.c_str());

	//initialize variables
	string line;

	vector<Point> coords;

	while (getline(infile, line)) {
		if (line.find('.') != string::npos) {
			istringstream iss(line);

			double pNbr, xCoord, yCoord;
			iss >> pNbr >> xCoord >> yCoord;
			Point p(pNbr, xCoord, yCoord);
			coords.push_back(p);
		}
	}

	//sort by x coord
	sort(coords.begin(), coords.end(), [](const Point& a, const Point& b) {
		return a.x < b.x;
	});

	//also send a vector with points sorted by y coord
	vector<Point> coordsYSort = coords;
	sort(coordsYSort.begin(), coordsYSort.end(), [](const Point& a, const Point& b) {
		return a.y < b.y;
	});

	//call recursive algorithm
	pair<Point, Point> closest = closestPair(0, coords.size() - 1, coords, coordsYSort);
	cout << "Point " << closest.first.name << " & Point " << closest.second.name << " || dist: " << closest.first.dist(closest.second) << endl;
	return 0;
}
