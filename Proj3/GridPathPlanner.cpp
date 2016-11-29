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
			Node *cur = &mNodes[i][j];
			// printf("(%d, %d)\n", i, j);

			// Create Neighbors List
			if(i < 39) cur->neighbors.push_back(&mNodes[i+1][j]);
			if(i > 0) cur->neighbors.push_back(&mNodes[i-1][j]);
			if(j < 9) cur->neighbors.push_back(&mNodes[i][j+1]);
			if(j > 0) cur->neighbors.push_back(&mNodes[i][j-1]);

			// Remove known blocked nodes from neighbor list
			for (int x = 0; x < cur->neighbors.size(); x++) {
				xyLoc n = cur->neighbors[x]->loc;
				if (!grid->IsValidLocation(n) || grid->IsBlocked(n)) {
					cur->neighbors[x] = cur->neighbors.back();
					cur->neighbors.pop_back();
					x--;
				}
			}
			printf("Node (%d, %d)\n", i, j);
			for(int n = 0; n < cur->neighbors.size(); n++)
			{
				printf("---Neighbor (%d, %d)\n", cur->neighbors[n]->mX, cur->neighbors[n]->mY);
			}
		}
	}
}

void GridPathPlanner::PrintInfo(Node *n)
{
	printf("Node (%d, %d) with xyLoc(%d, %d)\n", n->mX, n->mY, n->loc.x, n->loc.y);
	for(int i = 0; i < n->neighbors.size(); i++)
	{
		printf("-- Neighbor (%d, %d)\n", n->neighbors[i]->mX, n->neighbors[i]->mY);
	}
}

bool GridPathPlanner::InSet(std::vector<Node*> set, Node *n) {
	for(int i = 0; i < set.size(); i++)
	{
		if(set[i] == n)
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
		std::vector<Node*> openSet;
		std::vector<Node*> closedSet;

		xyLoc currentLoc = grid->GetCurrentLocation();
		Node *currentNode = &mNodes[currentLoc.x][currentLoc.y];
		Node *startNode = currentNode;

		xyLoc endLoc = grid->GetGoalLocation();
		Node *endNode = &mNodes[endLoc.x][endLoc.y];
	
		closedSet.push_back(currentNode);
		// PrintInfo(currentNode);
		while(currentNode != endNode)
		{
			for(int n = 0; n < currentNode->neighbors.size(); n++)
			{
				Node *p = currentNode->neighbors[n];
				// printf("Searching through neighbors\n");
				bool inClosedSet = InSet(closedSet, p);
				if(inClosedSet)
				{
					// printf("In closed set: ");
					// PrintInfo(p);
					continue;
				}
				else
				{
					// printf("Not in closed set: ");
					// PrintInfo(p);
					bool inOpenSet = InSet(openSet, p);
					if(inOpenSet)
					{
						// printf("In open set: ");
						// PrintInfo(p);
						int new_g = currentNode->mG + 1;
						if(new_g > p->mG)
						{
							p->mParent = currentNode;
							p->mG = new_g;
							p->mF = p->mG + p->mH;
						}
					}
					else
					{
						p->mParent = currentNode;
						p->mH = abs(p->mX - endNode->mX) + abs(p->mY - endNode->mY);
						p->mG = currentNode->mG + 1;
						p->mF = p->mH + p->mG;
						if(!grid->IsBlocked(p->loc))
						{
							// printf("PUSHED BACK\n");
							openSet.push_back(p);
						}
					}
				}
			}

			// Find node with lowest f
			Node *smallest = *openSet.begin();
			// printf("Starting with ");
			// PrintInfo(smallest);
			int removeIndex = 0;
			for(int s = 0; s < openSet.size(); s++)
			{
				if(openSet[s]->mF < smallest->mF)
				{
					removeIndex = s;
					// printf("Found smallest!: ");
					// PrintInfo(openSet[s]);
					smallest = openSet[s];
				}
			}

			openSet.erase(openSet.begin() + removeIndex);
			closedSet.push_back(currentNode);
			currentNode = smallest;
		}

		Node * secondToLast;
		while(currentNode != startNode)
		{
			secondToLast = currentNode;
			currentNode = currentNode->mParent;
		}

		return secondToLast->loc;
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
