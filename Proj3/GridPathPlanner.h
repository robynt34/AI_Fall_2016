#ifndef GRID_PATH_PLANNER_H
#define GRID_PATH_PLANNER_H

#include "PartiallyKnownGrid.h"

class GridPathPlanner{
public:
	GridPathPlanner(PartiallyKnownGrid* grid, bool use_adaptive_a_star = false);
	~GridPathPlanner();

	xyLoc GetNextMove(PartiallyKnownGrid* grid);
	int GetNumExpansions();

private:
	bool adaptive_a_star;
	int gridWidth, gridHeight;
};

#endif
