#include <string>
#include <vector>
using namespace std;


class Cell {

	
	const string name;
	vector<int> currentPos = {};
	const int area;


public:

	Cell(string& name, int &area) :name(name), area(area) {}
	Cell(string& name, int &x, int& y, const int &area) :name(name), currentPos({ x,y }), area(area) {}

	//Functions
	const string getName() {
		return this->name;
	}

	vector<int>getPosition() {
		return this->currentPos;
	}

	const int getArea() {
		return this->area;
	}

	//mutate functions
	void setCurrentPos(int x, int y) {
		if (currentPos.empty()) {
			currentPos.push_back(x);
			currentPos.push_back(y);
		}
	}



};
