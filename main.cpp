#include "Cell.h"
#include "Nets.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
using namespace std;
// header file contain all the functions of boost
//#include <boost/algorithm/string.hpp>
//Congestion- how many nets are within in a region.
//

int main() {
	vector<vector<Cell>> PlacementGrid;
    vector<Cell>Initializer;
    
    ifstream Cellfile("file.txt");
    ifstream NetFile("netFile.txt");

    string line;
    while (getline(Cellfile, line))
    {
        istringstream iss(line);
        string CellName;
        int area;
        if (!(iss >> CellName>> area)) { break; } // error
        iss >> CellName >> area;
        Initializer.push_back(Cell(CellName, area));

        // process pair (a,b)
    }

    /*int start(0), end(0);
    char dl = ' ';

    int a;
    string b;
    while (Cellfile >> a >> b)
    {
        
    }*/




}

int Cost (vector<int>old_place,vector<int>new_place){

}
int cost1() {

}
int cost2() {

}
int cost3() {

}

int changeTemp(int& temp) {

}
void Perturb(vector<vector<Cell>>&grid) {

}
