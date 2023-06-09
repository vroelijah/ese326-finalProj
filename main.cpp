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
/// These Functions give a random number between the given parameters
/// 
/// </summary>
/// <param name="low"></param>
/// <param name="high"></param>
/// <returns></returns>
long random(const long low, const long high) {
    uniform_int_distribution<>dist(low, high);
    return dist(gen);
}

double randomF(const double low, const double high) {
    uniform_real_distribution<double>dist(low, high);
    return dist(gen);
}

/// <summary>
/// This is our cost one function. It takes in our Nets map, which has every net within it.
/// We go through every Net, get the starting node and all of its neighbors, and we 
/// find the maximum x coordiate of all the neighbors, max y, min x, and min y,
/// and then we calculate the maximum bounding rectangle of that net.
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
    //cout << totalCost << endl;
    
    return totalCost;
}
/// <summary>
/// This is our cost two function, as well as our overlap function. We find overlap by first looking at two cells
/// that are in the same row, since we are assuming that all heights are the same.If they are not in the same row, they shouldnt overlap.
/// If they are in the same row, we divide the width by two, to account for the two directions the width can spread, and if
/// the first cell, assuming its to the left of the second cell, had a total area to the right of its coordinates that overlaped
/// with the total area of the second cell to the left, we found how much they overlapped by, and added it together.
/// We divided by two so the number wasnt too large.
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
    //cout << totalCost/2 << endl;
    return totalCost/2;
}

///// <summary>
///// For Cost three, to keep things simple, We just Subtracted the Min row cost from the max row cost, and divided by two so the number wasnt so big.
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
   // cout << (maxRowCost - minRowCost)  << endl;
    return (maxRowCost - minRowCost) / 2;
}

///// <summary>
///// The Cost Function just took in costs 1, 2, and 3 and added them together. We did not end up using it as we had to normalize the costs.
///// </summary>
///// <param name="nets"></param>
///// <param name="PlacementGrid"></param>
///// <returns></returns>
long Cost(map<string,Net>& nets, vector<vector<Cell>>& PlacementGrid) {
    long totalCost = 0;

    totalCost += cost1(nets);
    totalCost += cost2(PlacementGrid);
    totalCost += cost3(PlacementGrid);
   // cout << totalCost;
    return totalCost;
}

/// <summary>
/// This Function just placed all of the cells in our final placement grid in into a File, which we parsed through to be able to visualize what the placement
/// Would look like.
/// </summary>
/// <param name="PlacementGrid"></param>
void VisualPlacement(vector<vector<Cell>>& PlacementGrid) {

    ofstream OutFile("ibm01Output.txt");

    for (auto& list : PlacementGrid) {
        for (auto& cell : list) {
            OutFile << cell.getX() << " " << cell.getY() << " " << cell.getArea() << endl;
        }
        OutFile << endl;
    }

    OutFile.close();
}
///// <summary>
///// This is our Random placement Function. We swap two places on a copy of our map
//    We then update the values of the cells that we swapped, update them in our Cell Cache
//    and go through our nets to update the neighbors of the cells.
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
/// This is our schduleing Temo function. We pass a value in by ref to be able to be dcreased based on what value it is currently at.
/// </summary>
/// <param name="temp"></param>
void Schedule(double &temp) {
    if (temp >= Init_temp / 4)
        temp = .8 * temp;
    else if (temp > 100000 && temp < 250000)
        temp = .95 * temp;
    else
        temp = .8 * temp;

    //cout << temp << endl;
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
    //we do our initial placement with a 2d vector that takes the square root of the number of total cells, and create a grid with width and length equal to the square root
    // that way we always have an equal number of rows and columns
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

   // Start of the actual algorithm. Most of it from the textboox. size is divided by a number to speed up the algoritm. We also normalized
   //the costs by dividing by eachoher as mentioned in class.
    double temp = Init_temp;
    while (temp > finalTemp) {
        int size = CellCache.size()/2000;
        double NetCost;
        double deltaCost;
        while (inner_loop_crit(size) == false) {
            //cout << size << endl;
            vector<vector<Cell>> new_place = Perturb(PlacementGrid,numRAndC,CellCacheCopy,netCacheCopy);
           //long InitialCost = Cost();
            double initCost1(cost1(netCache)), initCost2(cost2(PlacementGrid)), initCost3(cost3(PlacementGrid)); //
            double Cost1(cost1(netCacheCopy)), Cost2(cost2(new_place)), Cost3(cost3(new_place)), Costt(Cost(netCacheCopy, new_place));

            double initCost((initCost1 / initCost1) + (initCost2 / initCost2) + (initCost3 / initCost3));
            NetCost = (initCost1 / Cost1) + (initCost2 / Cost2) + (initCost3 / Cost3) ;
            deltaCost = NetCost/ initCost;
            if (deltaCost < 0) {
                PlacementGrid = new_place;
                netCache = netCacheCopy;
                CellCache = CellCacheCopy;
            }
            else if (randomF(0, 2) > exp((NetCost-initCost) / temp) && temp>150) {
                PlacementGrid = new_place;
                netCache = netCacheCopy;
                CellCache = CellCacheCopy;
                cout << "random increase given" << endl;
            }
        }
        Schedule(temp);
        
    }
    VisualPlacement(PlacementGrid);
    
    
    Cellfile.close();
    Netfile.close();
    return 0;
}



