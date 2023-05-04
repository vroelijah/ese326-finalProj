#pragma once
#include <string>
#include <vector>
using namespace std;

class Cell {

private:
	 string name;
	vector<int> currentPos = {};
	int area;
	int x = 0;
	int y = 0;


public:

	Cell() :name(""),area(0) {}
	Cell(const Cell& other) : name(other.name), currentPos(other.currentPos), area(other.area), x(other.x), y(other.y){}
	Cell(const string& name,const int& area) :name(name), area(area), x(0), y(0) {}
	Cell(const string& name, int& x, int& y,const int& area) :name(name), x(x), y(y), area(area) {}
	//Cell(Cell&) = default;

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

	void setX(const int& x) {
		this->x = x;
	}
	void setY(const int& y) {
		this->y = y;

	}
	

	//mutate functions
	void setCurrentPos(int x, int y) {
		if (currentPos.empty()) {
			currentPos.push_back(x);
			currentPos.push_back(y);
		}
	}

	Cell& operator=(const Cell& a) {
		name = a.name;
		currentPos = a.currentPos;
		area = a.area;
		x = a.x;
		y = a.y;
		return *this;
	}


	~Cell() {}

};

