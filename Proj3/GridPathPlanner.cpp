#include "GridPathPlanner.h"

GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid, bool use_adaptive_a_star) {
	// TODO
	printf("Constructor\n");
	adaptive_a_star = use_adaptive_a_star;
	gridWidth = grid->GetWidth();
	gridHeight = grid->GetHeight();

	printf("Width: %d Height: %d\n", gridWidth, gridHeight);
}

GridPathPlanner::~GridPathPlanner(){
	// TODO
}

xyLoc GridPathPlanner::GetNextMove(PartiallyKnownGrid* grid) {
	// TODO
	// This is just a dummy implementation that returns a random neighbor.
	if(!adaptive_a_star)
	{
		// Use Forward A*
		printf("Forward A* \n");
		std::vector<xyLoc> open;
		std::vector<xyLoc> closed;
		xyLoc curr = grid->GetCurrentLocation();
		xyLoc goal = grid->GetGoalLocation();

		// Add current location to closed list
		closed.push_back(curr);

		// Add neighbors to open list
		open.push_back(xyLoc(curr.x+1, curr.y));
		open.push_back(xyLoc(curr.x-1, curr.y));
		open.push_back(xyLoc(curr.x, curr.y+1));
		open.push_back(xyLoc(curr.x, curr.y-1));

		// Create vector to track neighbors
		std::vector<xyLoc> neighbors;
		neighbors.push_back(xyLoc(curr.x+1, curr.y));
		neighbors.push_back(xyLoc(curr.x-1, curr.y));
		neighbors.push_back(xyLoc(curr.x, curr.y+1));
		neighbors.push_back(xyLoc(curr.x, curr.y-1));

		// while(curr != goal)
		// {

		// }
		for(int i = 0; i < neighbors.size(); i++)
		{
			xyLoc checkIfContains;
			for(int j = 0; j < open.size(); j++)
			{
				if(neighbors[i] == open[i])
				{
					checkIfContains = open[i];
					break;
				}

			}

			if(checkIfContains.x == -1 && checkIfContains.y == -1)
				printf("NOTOPEN");
			else
				printf(" INOPEN ");
		}
		
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
