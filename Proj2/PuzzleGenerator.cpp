#include "PuzzleGenerator.h"
using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{

	T = 100;
	T_min = .00001;
	alpha = .99;

	timer.StartTimer();
	maxtime = 59.9;	// To make sure we don't exceed a minute
	
	// We could also do as many random walks as we can within the given time limit.
	Puzzle basePuzzle(nRows, nColumns, minVal, maxVal);
	Puzzle bestPuzzle = SimulatedAnnealing(basePuzzle);
	while (timer.GetElapsedTime() + 5 < maxtime)
	{
		 basePuzzle = Puzzle(nRows, nColumns, minVal, maxVal);
		 Puzzle p = SimulatedAnnealing(basePuzzle);
		// Check if p is better than the best puzzle we have found so far.
		if(p.GetValue() > bestPuzzle.GetValue())
		{
			bestPuzzle = p;
		}
	}
	return bestPuzzle;
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that starts at a random configuration and keeps randomly modifying it
	// until the time limit is reached. Returns the best solution found so far.

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.
	
	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;			
	int bestValue = p.GetValue();
	
	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();
	 
	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell 
		// (since we are doing a random walk, we just replace p with its successor).
		//p = p.GetRandomSuccessor();
		printf("Best value: %d P: %d\n", bestValue, p.GetValue());

		vector<Puzzle> successors;
		bestPuzzle.GetAllSuccessors(successors);
		for(int i = 0; i < successors.size(); i++)
		{
			if(successors[i].GetValue() > bestValue)
			{
				bestValue = successors[i].GetValue();
				printf("new val: %d\n", bestValue);
				bestPuzzle = successors[i];
			}
		}
		
		// Update the current best solution.
		// if (p.GetValue() > bestValue)	// Calling GetValue() for the first time is costly
		// 								// since the puzzle has to be evaluated first.
		// {
		// 	bestValue = p.GetValue();	// Calling it a second time simply returns the value that was computed before.
		// 	bestPuzzle = p;


		// }
	}
	
	return bestPuzzle;
	
	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
	// vector<Puzzle> successors;
	// bestPuzzle.GetAllSuccessors(successors);
}

Puzzle PuzzleGenerator::HillClimb(double timeLimit)
{
	// Generate first puzzle randomly
	Puzzle p(nRows, nColumns, minVal, maxVal); 

	Puzzle bestPuzzle = p;

	Timer t;
	t.StartTimer();

	while(t.GetElapsedTime() < timeLimit-0.1)
	{
		vector<Puzzle> successors;
		bestPuzzle.GetAllSuccessors(successors);
		Puzzle topSuccessor = successors[0];
		for(int i = 0; i < successors.size(); i++)
		{
			if(successors[i].GetValue() > topSuccessor.GetValue())
			{
				printf("New value: %d ", successors[i].GetValue());
				printf("Old value: %d\n", topSuccessor.GetValue());
				topSuccessor = successors[i];
			}
		}

		if(topSuccessor.GetValue() > bestPuzzle.GetValue())
		{
			bestPuzzle = topSuccessor;
		}
		else
		{
			return bestPuzzle;
		}
	}

	// out of time
	return bestPuzzle;
}

Puzzle PuzzleGenerator::SimulatedAnnealing(Puzzle p)
{
	// 	Reference site: katrinaeg.com/simulated-annealing.html
	// Gen random solution
	Puzzle original = p;
	T = 100;

	while(T > T_min)
	{
		Puzzle neighbor = original.GetRandomSuccessor();
		if(neighbor.GetValue() > original.GetValue())
		{
			original = neighbor;
		}
		else
		{
			// figure out acceptance probability (a = e^((old-new)/T))
			int dif = neighbor.GetValue() - original.GetValue();
			double newVal = pow(2.718, dif/T);
			double randomVal = (double) (rand() % 100) / 100.0;
			if(newVal > randomVal)
			{
				original = neighbor;
			}
		}

		T *= alpha;
	}
	return original;
}
