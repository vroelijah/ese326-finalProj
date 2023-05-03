#pragma once
#include <string>
#include <vector>
#include "Cell.h"
using namespace std;


class Net {

	 Cell startingCell;
	 vector<Cell>neighborCells;

public:

	Net( Cell &start,vector<Cell>neighbors) :startingCell(start), neighborCells(neighbors) {}

	const string getNetStartCellName() {
		return this->startingCell.getName();
	}

};
