#include <string>
#include <vector>
using namespace std;


class Cell {

	
	const string name;
	vector<int> currentPos = {};


public:

	Cell(string& name) :name(name) {}
	Cell(string& name, int &x, int& y) :name(name), currentPos({ x,y }) {}




};
