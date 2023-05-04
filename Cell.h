#pragma once
#include <string>
#include <vector>
using namespace std;

class Cell {

private:
	const string name;
	vector<int> currentPos = {};
	const int area;
	int x, y = 0;


public:

	Cell() :name(""), area(0), x(0), y(0) {};
	Cell(string& name, int& area) :name(name), area(area), x(0), y(0) {}
	Cell(string& name, int& x, int& y, const int& area) :name(name), x(x), y(y), area(area) {}

	//Functions
	const string getName() {
		return this->name;
	}

	int getX() {
		return this->x;
	}
	int getY() {
		return this->y;
	}

	const int getArea() {
		return this->area;
	}

	void setX(int& x) {
		this->x = x;
	}
	void setY(int& y) {
		this->y = y;

	}

	//mutate functions
	void setCurrentPos(int x, int y) {
		if (currentPos.empty()) {
			currentPos.push_back(x);
			currentPos.push_back(y);
		}
	}



};

