#ifndef GRID_PATH_PLANNER_H
#define GRID_PATH_PLANNER_H

#include "PartiallyKnownGrid.h"
#include <set>

struct Node{
	xyLoc loc;
	int mF, mH, mG;
	int mX,mY;
	struct Node *mParent;
	bool mBlocked;
	std::vector<Node*> neighbors;
	Node(){
		
	}

	Node(xyLoc location){
		loc = location;
		mX = loc.x;
		mY = loc.y;
		mF = mH = mG = 0;
		mBlocked = false;
		printf("Made node X: %d Y: %d\n", mX, mY);
	
	}
};

class GridPathPlanner{
public:
	GridPathPlanner(PartiallyKnownGrid* grid, bool use_adaptive_a_star = false);
	~GridPathPlanner();
	void PrintInfo(Node *n);
	xyLoc GetNextMove(PartiallyKnownGrid* grid);
	int GetNumExpansions();
	void CreateNodes(int w, int h, PartiallyKnownGrid *grid);

private:
	Node **mNodes;
	bool adaptive_a_star;
	int gridWidth, gridHeight;
};

#endif
