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




};
