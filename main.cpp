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
#include <memory>
using namespace std;

//Congestion- how many nets are within in a region.
random_device rd;
mt19937 gen(rd());

constexpr int maxNum = INT_MAX;
constexpr double Init_temp = 4000000;
constexpr double finalTemp = .01;

/// <summary>
/// 
/// </summary>
/// <param name="low"></param>
/// <param name="high"></param>
/// <returns></returns>
long random(const long low, const long high) {
    uniform_int_distribution<>dist(low, high);
    return dist(gen);
}

/// <summary>
/// 
/// </summary>
/// <param name="nets"></param>
/// <returns></returns>
long cost1(map<string,Net>& nets) {
    
    long totalCost = 0;
    long totalNetCost = 0;
    for (auto& net : nets) {
        Net start = net.second;
        vector<Cell>neighbors = start.getNeighbors();
        neighbors.push_back(start.getStartingCell());
        long max_x(0), max_y(0), min_x(maxNum), min_y(maxNum),yweight(0);
        for (auto& neighbor : neighbors) {
            max_x = max(max_x, neighbor.getX());
            max_y = max(max_y, neighbor.getY());
            min_x = min(min_x, neighbor.getX());
            min_y = min(min_y, neighbor.getY());
            yweight += neighbor.getArea();
            
        }
        totalNetCost = abs(((max_x - min_x))) + abs(((max_y - min_y)));
        
        totalCost += totalNetCost;
    }
    cout << totalCost << endl;
    
    return totalCost;
}
/// <summary>
/// 
/// </summary>
/// <param name="a1"></param>
/// <param name="a2"></param>
/// <returns></returns>
long overlap(Cell& a1, Cell& a2) {
    long totalOverlap(0);
    if (a1.getX() != a2.getX()) {
        return 0;
    }
    else {
        const int firstCell = a1.getY() - a1.getArea() / 2;
        const int secondCell = a2.getY() + a2.getArea() / 2;

        if (firstCell <= secondCell) {
            totalOverlap = secondCell - firstCell;
        }
    }
    return totalOverlap;
}
long cost2(vector<vector<Cell>>& PlacementGrid) {
    long totalCost(0), pairCost(0);
    const long n = PlacementGrid.size();

    for (long i = 0; i < n; i++) {
        const long m = PlacementGrid[i].size();
        for (long j = i + 1; j < m; j++) {
            for (long k = 0; k < n; k++) {
                const long m = PlacementGrid[k].size();
                for (long l = k + 1; l < m; l++) {
                    if (i == k && j == l) continue;
                    auto& cell1 = PlacementGrid[i][j];
                    auto& cell2 = PlacementGrid[k][l];
                    totalCost += overlap(cell1, cell2);

                }
            }
        }
    }
    cout << totalCost << endl;
    return totalCost;
}

///// <summary>
///// 
///// </summary>
///// <param name="PlacementGrid"></param>
///// <returns></returns>
long cost3(vector<vector<Cell>> PlacementGrid) {
    long totalCost3(0), rowCost(0), maxRowCost(0), minRowCost(maxNum);

    for (auto& row : PlacementGrid) {
        rowCost = 0;
        for (auto& cell : row) {

            rowCost += cell.getArea();
        }
        maxRowCost = max(maxRowCost, rowCost);
        minRowCost = min(minRowCost, rowCost);
        totalCost3 += rowCost;
    }
    cout << (maxRowCost - minRowCost)  << endl;
    return (maxRowCost - minRowCost) / 2;
}

///// <summary>
///// 
///// </summary>
///// <param name="nets"></param>
///// <param name="PlacementGrid"></param>
///// <returns></returns>
long Cost(map<string,Net>& nets, vector<vector<Cell>>& PlacementGrid) {
    long totalCost = 0;

    totalCost += cost1(nets);
    totalCost += cost2(PlacementGrid);
    totalCost += cost3(PlacementGrid);
    cout << totalCost;
    return totalCost;
}
///// <summary>
///// 
///// </summary>
///// <param name="Placement"></param>
///// <param name="numRAndC"></param>
///// <returns></returns>
vector<vector<Cell>> Perturb(vector<vector<Cell>>& Placement, const int numRAndC, map<string,Cell>&CellCache,map<string,Net>&netCache) {
      vector<vector<Cell>>new_place(Placement);
      int x(random(0, numRAndC - 1)), y(random(0, numRAndC - 1)), i(random(0, numRAndC - 1)), j(random(0, numRAndC - 1));
     
      swap(new_place[i][j], new_place[x][y]);
      Cell& A = Placement[i][j];
      Cell& B = Placement[x][y];
      CellCache[A.getName()].setX(x);
      CellCache[A.getName()].setY(y);
      CellCache[B.getName()].setX(i);
      CellCache[B.getName()].setY(j);

      for (auto& pairs : netCache) {
          Net &start = pairs.second;
          vector<Cell>Cells=start.getNeighbors();
          Cells.push_back(start.getStartingCell());
          auto posA = find(Cells.begin(), Cells.end(), A);
          auto posB = find(Cells.begin(), Cells.end(), B);
          if (posA != Cells.end()) {
               *posA=CellCache[A.getName()];

               start.setNeighbors(Cells);
          }
          if (posB != Cells.end()) {
              *posB = CellCache[B.getName()];
              start.setNeighbors(Cells);
          }
      }


    return new_place;
}

/// <summary>
/// 
/// </summary>
/// <param name="temp"></param>
void Schedule(double &temp) {
    if (temp >= Init_temp / 2)
        temp = .8 * temp;
    else if (temp > Init_temp / 4 && temp < Init_temp / 2)
        temp = .95 * temp;
    else
        temp = .8 * temp;
}
bool inner_loop_crit(int &size) {
    size -= 1;
    if (size == 0) {
        return true;
    }
    return false;
}
/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main() {
	map<string, Cell>CellCache;
    map<string, Net>netCache;
    map<string, Cell>CellCacheCopy;
    map<string, Net>netCacheCopy;
    vector<long>counts;
   
    

    
    ifstream Cellfile("ibm01.txt");
    ifstream Netfile("ibm01nets.txt");

    string line;
    //Parsing of all the files and creating cells
    int i = 0;
    while (i < 5) {
        string num;
        getline(Netfile,num);
        counts.push_back(stol(num));
        num.replace(num.find(num), num.length(), "");
        i++;
    }

    const int numRAndC = int(ceil(sqrt(counts[3])));
    vector<vector<Cell>> PlacementGrid(numRAndC, vector<Cell>(numRAndC));

    //Initial Placements and File parsing
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
        PlacementGrid[x][y]= newCell;
        CellCache[CellName] = newCell;
    }

    //Nets Parsing
    string netline,currentStartNode;
    while (getline(Netfile, netline)) {
        if (netline == "")continue;
        size_t pos = netline.find('s');
        if (pos != string::npos) {
            string starterCellN = netline.substr(0, pos);
            starterCellN.erase(remove_if(starterCellN.begin(), starterCellN.end(), ::isspace));

            Cell startingCell = CellCache[starterCellN];
            Net newNet = Net(startingCell, {});
            netCache[starterCellN] = newNet;

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
    CellCacheCopy = CellCache;
    netCacheCopy = netCache;
    //Start of actual Algorithm and Peicing it all together
    /*cost1(netCacheCopy);
    cost3(PlacementGrid);
    int t = 100;*/
    //cost2(PlacementGrid);
    //cost3(PlacementGrid);
    //while (t > 0) {
    //    
    //    //swap(PlacementGrid[random(0, numRAndC - 1)][random(0, numRAndC - 1)], PlacementGrid[random(0, numRAndC - 1)][random(0, numRAndC - 1)]);
    //    vector<vector<Cell>> new_place = Perturb(PlacementGrid, numRAndC,CellCacheCopy,netCacheCopy);
    //    //cost2(new_place);
    //    cost3(new_place);
    //    cost1(netCacheCopy);
    //    cout << "END OF ITERATION ----------------- END OF ITERATION" << endl;
    //    t--;
    //}
    ;
   // when swapping 2 places, also change in the map the values of x and y.
    //ok so get initial cost 1-4
    //then its(curcost1 / initcost1) + (curcost2 / initcost2)...
    double temp = Init_temp;
    while (temp > finalTemp) {
        int size = CellCache.size()/3;
        while (inner_loop_crit(size) == false) {
            vector<vector<Cell>> new_place = Perturb(PlacementGrid,numRAndC,CellCacheCopy,netCacheCopy);
           //long InitialCost = Cost();
            long initCost1(cost1(netCache)), initCost2(cost2(PlacementGrid)), initCost3(cost3(PlacementGrid)), initCost(Cost(netCache, PlacementGrid));
            long Cost1(cost1(netCacheCopy)), Cost2(cost2(new_place)), Cost3(cost3(new_place)), Costt(Cost(netCacheCopy, new_place));
            long NetCost = (initCost1 / Cost1) + (initCost2 / Cost2) + (initCost3 / Cost3) ;
            if (NetCost < 0) {
                PlacementGrid = new_place;
                netCache = netCacheCopy;
                CellCache = CellCacheCopy;
            }
            else if (random(0, 1) > exp(NetCost / temp)) {
                PlacementGrid = new_place;
                netCache = netCacheCopy;
                CellCache = CellCacheCopy;
            }
        }
        Schedule(temp);
    }
    
    
    

    return 0;
}



