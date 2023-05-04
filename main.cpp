#include "Cell.h"
#include "Nets.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
#include <random>
using namespace std;

//Congestion- how many nets are within in a region.
//
random_device rd;
mt19937 gen(rd());

int random(const int low, const int high) {
    uniform_int_distribution<>dist(low, high);
    return dist(gen);
}

int main() {
	map<string, Cell>CellCache;
    map<string, Net>netCache;
    vector<long>counts;
    

    
    ifstream Cellfile("ibm01.txt");
    ifstream Netfile("ibm01nets.txt");

    string line;

    int i = 0;
    while (i < 5) {
        string num;
        getline(Netfile,num);
        counts.push_back(stol(num));
        num.replace(num.find(num), num.length(), "");
        i++;
    }

    int numRAndC = int(ceil(sqrt(counts[3])));
    //cout << numRAndC << "\n";
    vector<vector<Cell>> PlacementGrid(numRAndC, vector<Cell>(numRAndC));

    while (getline(Cellfile, line))
    {
        istringstream iss(line);
        string CellName;
        int area;
        if (!(iss >> CellName>> area)) { break; } // error
        iss >> CellName >> area;
       // cout << CellName << " " << area << endl;
        
        int x = random(0, numRAndC - 1), y(random(0, numRAndC - 1));
        int j = 0;
        while (PlacementGrid[x][y].getName() != ""&&j<=numRAndC*numRAndC) {
            x = random(0, numRAndC -1);
            y = random(0, numRAndC -1);
            j++;
        }
        Cell newCell = Cell(CellName,x,y,area);
        PlacementGrid[x][y]=newCell;
        CellCache.emplace(CellName,newCell);
    }
    /*for (auto& list : PlacementGrid) {
        for (auto& cell : list) {
            cout << cell.getName() << " " << cell.getArea() << " ";
        }
        cout << endl;
    }*/

    string netline,currentStartNode;
    while (getline(Netfile, netline)) {
        if (netline == "")continue;
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
   // netCache[currentStartNode].getNeighbors();



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


