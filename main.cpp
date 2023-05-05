#include "Cell.h"
#include "Nets.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
#include <random>
#include <numeric>
using namespace std;

//Congestion- how many nets are within in a region.
//
random_device rd;
mt19937 gen(rd());

constexpr int maxNum = INT_MAX;

long random(const long low, const long high) {
    uniform_int_distribution<>dist(low, high);
    return dist(gen);
}

int cost1(map<string, Net>& nets) {
    
    long totalCost = 0;
    long totalNetCost = 0;
    for (auto& net : nets) {
        Net start = net.second;
        vector<Cell>neighbors = start.getNeighbors();
        neighbors.push_back(start.getStartingCell());
        long max_x(0), max_y(0), min_x(maxNum), min_y(maxNum),yweight(0);
        for (auto& neighbor : neighbors) {
           // cout << neighbor.getArea() << " ";
            max_x = max(max_x, neighbor.getX());
            max_y = max(max_y, neighbor.getY());
            min_x = min(min_x, neighbor.getX());
            min_y = min(min_y, neighbor.getY());
            yweight += neighbor.getArea();
           
            
        }
       // cout << endl;
        totalNetCost = ((max_x - min_x)*(1*.15)) + ((max_y - min_y)*(yweight*.85));
        
        totalCost += totalNetCost;
    }
    cout << totalCost;
    
    return totalCost;
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
    vector<vector<Cell>> PlacementGrid(numRAndC, vector<Cell>(numRAndC));

    while (getline(Cellfile, line))
    {
        istringstream iss(line);
        string CellName;
        int area;
        if (!(iss >> CellName>> area)) { break; } // error
        iss >> CellName >> area;
        
        long x = random(0, numRAndC - 1), y(random(0, numRAndC - 1));
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
    cost1(netCache);
    
    

    return 0;
}



//int Cost (vector<int>old_place,vector<int>new_place){
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


