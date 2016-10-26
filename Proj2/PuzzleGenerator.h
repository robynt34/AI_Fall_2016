#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <math.h>
#include "Puzzle.h"
#include "Timer.h"

class PuzzleGenerator{
public:
	PuzzleGenerator(int nRows, int nColumns, int minVal, int maxVal);
	Puzzle GeneratePuzzle(); 
	Puzzle RandomWalk(double timelimit);	// Time limit given in seconds.
	Puzzle HillClimb(double timeLimit);
	Puzzle SimulatedAnnealing(Puzzle p);
	
private:
	int nRows, nColumns, minVal, maxVal;
	double alpha, T, T_min;
	Timer timer;
	double maxtime;
};
