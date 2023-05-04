#include "Cell.h"
//#include "C:\Users\jumpy\source\repos\ese326-finalProj\Cell.h"
#include "Nets.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;
// header file contain all the functions of boost
//#include <boost/algorithm/string.hpp>
//Congestion- how many nets are within in a region.
//

int main() {
	vector<vector<Cell>> PlacementGrid;
    vector<Cell>Initializer;
    map<string, Cell>CellCache;
    map<string, Net>netCache;
    
    ifstream Cellfile("file.txt");
    ifstream Netfile("netFile.txt");

    string line;
    while (getline(Cellfile, line))
    {
        istringstream iss(line);
        string CellName;
        int area;
        if (!(iss >> CellName>> area)) { break; } // error
        iss >> CellName >> area;
        cout << CellName << " " << area << "\n";
        Cell newCell =  Cell(CellName, area);
        Initializer.push_back(newCell);
        CellCache.emplace(CellName, newCell);
    }

    string netline,currentStartNode;
    while (getline(Netfile, netline)) {
        size_t pos = netline.find('s');
        if (pos != string::npos) {
            string starterCellN = netline.substr(0, pos);
            starterCellN.erase(remove_if(starterCellN.begin(), starterCellN.end(), ::isspace));

            Cell startingCell = CellCache[starterCellN];
            Net newNet = Net(startingCell, {});
            netCache.emplace(starterCellN, newNet);

            currentStartNode = starterCellN;
        }
        else {
            size_t newPos = netline.find(' ');
            string neighborCell = netline.substr(0, newPos);
            neighborCell.erase(remove_if(neighborCell.begin(), neighborCell.end(), ::isspace));
            Cell neighbor = CellCache[neighborCell];
            netCache[currentStartNode].addNeighbor(neighbor);
        }
        
    }
    



}

//int Cost (vector<int>old_place,vector<int>new_place){
//
//}
//int cost1() {
//
//}
//int cost2() {
//
//}
//int cost3() {
//
//}
//
//int changeTemp(int& temp) {
//
//}
//void Perturb(vector<vector<Cell>>&grid) {
//
//}
