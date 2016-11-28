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

		Node currNode(grid->GetCurrentLocation());
		currNode.CreateNeighbors();
		Node endNode(grid->GetGoalLocation());
		endNode.CreateNeighbors();
		closedSet.push_back(currNode);
		int currIndex = 0;

		while(currNode.mX != endNode.mX && currNode.mY != endNode.mY)
		{
			printf("CurrNode X: %d Y: %d\n", currNode.mX, currNode.mY);
			for(int i = 0; i < currNode.neighbors.size(); i++)
			{
				Node p = currNode.neighbors[i];
				bool inClosedSet = InSet(closedSet, p);
				if(inClosedSet)
				{
					printf("-In Closed Set-\n");
					continue;
				}
				else
				{
					printf("-Not in Closed Set-\n");
					printf("Openset size: %d\n", openSet.size());
					
					bool inOpenSet = InSet(openSet, p);
					if(inOpenSet)
					{
						printf("-In Open Set-\n");
						float new_g = currNode.mG + 1;
						if(new_g > p.mG)
						{
							p.mParent = &currNode;
							p.neighbors[i].mG = new_g;
							p.mF = p.mG + p.mH;
						}
					}
					else
					{
						printf("-Not in open set-\n");
						p.mParent = &currNode;
						p.mH = abs(p.mX - endNode.mX) + abs(p.mY - endNode.mY);
						p.mG = currNode.mG + 1;
						p.mF = p.mH + p.mG;
						if(!grid->IsBlocked(p.loc))
						{
							printf("PUSHED BACK\n");
							openSet.push_back(p);
						}
					}	
				}
			}

			// set new Curr (node with lowest f in openSet)
			Node new_curr = *openSet.begin();
			for(int i = 0; i < openSet.size(); i++)
			{
				if(openSet[i].mF < new_curr.mF)
				{
					new_curr = openSet[i];
				}
			}
			printf("openSetSize: %d\n", openSet.size());
			for(int i = 0; i < openSet.size(); i++)
			{
				printf("INDEX: %d\n", i);
				if(NodesEqual(currNode, openSet[i]))
				{
					printf("found current node\n");
					currIndex = i;
				}
			}
			printf("Line 120\n\n");
			openSet.erase(openSet.begin() + currIndex);
			closedSet.push_back(currNode);
			currNode = new_curr;
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
