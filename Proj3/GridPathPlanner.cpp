#include "GridPathPlanner.h"

GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid, bool use_adaptive_a_star) {
	// TODO
	printf("Constructor\n");
	adaptive_a_star = use_adaptive_a_star;
	gridWidth = grid->GetWidth();
	gridHeight = grid->GetHeight();

	CreateNodes(gridWidth, gridHeight, grid);

	printf("Width: %d Height: %d\n", gridWidth, gridHeight);
}

GridPathPlanner::~GridPathPlanner(){
	// TODO
}

void GridPathPlanner::CreateNodes(int width, int height, PartiallyKnownGrid *grid)
{
	mNodes = new Node*[width];
	for(int i = 0; i < width; i++)
	{
		mNodes[i] = new Node[height];
	}

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			xyLoc location(i,j);
			mNodes[i][j].loc = location;
			mNodes[i][j].mX = i;
			mNodes[i][j].mY = j;
		}
	}

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			Node cur = mNodes[i][j];
			// printf("(%d, %d)\n", i, j);

			// Create Neighbors List
			if(i < 39) cur.neighbors.push_back(mNodes[i+1][j]);
			if(i > 0) cur.neighbors.push_back(mNodes[i-1][j]);
			if(j < 9) cur.neighbors.push_back(mNodes[i][j+1]);
			if(j > 0) cur.neighbors.push_back(mNodes[i][j-1]);

			// Remove known blocked nodes from neighbor list
			for (int x = 0; x < cur.neighbors.size(); x++) {
				xyLoc n = cur.neighbors[x].loc;
				if (!grid->IsValidLocation(n) || grid->IsBlocked(n)) {
					cur.neighbors[x] = cur.neighbors.back();
					cur.neighbors.pop_back();
					x--;
				}
			}
			printf("Node (%d, %d)\n", i, j);
			for(int n = 0; n < cur.neighbors.size(); n++)
			{
				printf("---Neighbor (%d, %d)\n", cur.neighbors[n].mX, cur.neighbors[n].mY);
			}
		}
	}
}

bool GridPathPlanner::NodesEqual(Node x, Node y)
{
	if(x.mX == y.mX && x.mY == y.mY)
	{
		printf("return true from nodes equal\n");
		return true;
	}
	printf("return false from nodes equal\n");
	return false;
}

bool GridPathPlanner::InSet(std::vector<Node> v, Node n)
{
	for(int i = 0; i < v.size(); i++)
	{
		if(NodesEqual(n, v[i]))
			return true;
	}	
	return false;
}


xyLoc GridPathPlanner::GetNextMove(PartiallyKnownGrid* grid) {
	// TODO
	// This is just a dummy implementation that returns a random neighbor.
	if(!adaptive_a_star)
	{
		// Use Forward A*
		printf("Forward A* \n");
		std::vector<Node> openSet;
		std::vector<Node> closedSet;

		// Node currNode(grid->GetCurrentLocation());
		// currNode.CreateNeighbors();
		// Node endNode(grid->GetGoalLocation());
		// endNode.CreateNeighbors();
		// closedSet.push_back(currNode);
		// int currIndex = 0;
	}

	xyLoc curre = grid->GetCurrentLocation();
	std::vector<xyLoc> neighbors;
	neighbors.push_back(xyLoc(curre.x+1, curre.y));
	neighbors.push_back(xyLoc(curre.x-1, curre.y));
	neighbors.push_back(xyLoc(curre.x, curre.y+1));
	neighbors.push_back(xyLoc(curre.x, curre.y-1));

	for (int i = 0; i < neighbors.size(); i++) {
		xyLoc n = neighbors[i];
		if (!grid->IsValidLocation(n) || grid->IsBlocked(n)) {
			neighbors[i] = neighbors.back();
			neighbors.pop_back();
			i--;
		}
	}

	if (neighbors.size() == 0)
		return kInvalidXYLoc;
	else
		return neighbors[rand()%neighbors.size()];
}

int GetNumExpansions() {
	// TODO
	return 0;
}
