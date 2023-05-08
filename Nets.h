#pragma once
#include <string>
#include <vector>
#include "Cell.h"
#include <iostream>
using namespace std;




class Net {

	 Cell startingCell;
	 vector<Cell>neighborCells;
	 /*string null = " ";
	  int area = 0;*/

public:
	Net() : startingCell(Cell()) {}
	Net( Cell start,vector<Cell>neighbors) :startingCell(start), neighborCells(neighbors) {}

	const string getNetStartCellName() {
		return startingCell.getName();
	}
	void addNeighbor(Cell &neighbor) {
		neighborCells.push_back(neighbor);
	}
	void setNeighbors(vector<Cell>&neighbors) {
		this->neighborCells = neighbors;
	}

	vector<Cell> getNeighbors() {
		return this->neighborCells;
	}
	Cell getStartingCell() {
		return this->startingCell;
	}

	~Net() {}

};
