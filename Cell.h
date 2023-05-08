#pragma once
#include <string>
#include <vector>
using namespace std;

class Cell {

private:
	 string name;
	vector<int> currentPos = {};
	int area;
	long x = 0;
	long y = 0;


public:

	Cell() :name(""),area(0) {}
	Cell(const Cell& other) : name(other.name), currentPos(other.currentPos), area(other.area), x(other.x), y(other.y){}
	Cell(const string& name,const int& area) :name(name), area(area), x(0), y(0) {}
	Cell(const string& name, long& x, long& y,const int& area) :name(name), x(x), y(y), area(area) {}
	//Cell(Cell&) = default;

	//Functions
	const string getName() {
		return this->name;
	}

	long getX() {
		return this->x;
	}
	long getY() {
		return this->y;
	}

	const int getArea() {
		return this->area;
	}

	void setX(const long& x) {
		this->x = x;
	}
	void setY(const long& y) {
		this->y = y;

	}
	

	//mutate functions
	void setCurrentPos(int x, int y) {
		if (currentPos.empty()) {
			currentPos.push_back(x);
			currentPos.push_back(y);
		}
	}

	/*Cell& operator=(const Cell& a) {
		name = a.name;
		currentPos = a.currentPos;
		area = a.area;
		x = a.x;
		y = a.y;
		return *this;
	}*/
	Cell& operator=(const Cell& a) {
		name = a.name;
		currentPos = a.currentPos;
		area = a.area;
		x = a.x;
		y = a.y;
		return *this;
	}
	bool operator== (const Cell& other) const {
		return name == other.name && currentPos == other.currentPos && area == other.area && x == other.x && y == other.y;
	}



	~Cell() {}

};

