/*
 * Algorithm2.h
 *
 *  Created on: 09.05.2015
 *      Author: Christian
 */

#ifndef SRC_ALGORITHM2_H_
#define SRC_ALGORITHM2_H_

#include "Solution.h"
#include "Graph.h"
#include <deque>

namespace graphcoloring
{

class Algorithm2
{
public:
	Algorithm2(const Graph& graph);
	virtual ~Algorithm2();
	Solution findOptimalSolution();

private:
	Graph graph;
	int k;
	Solution bestSolution;
	int bestCosts;
	Solution currentSol;
	unsigned int tabuListSize;
	unsigned int maxTries;

	std::vector<std::vector<int> > colorClasses;
	std::deque<std::pair<int, int> > tabuList;

	struct Move
	{
		Move(int node, int color, int deltaCosts) :
				node(node), color(color), deltaCosts(deltaCosts)
		{
		}
		int node;
		int color;
		int deltaCosts;
	};

	void greedyConstHeu();
	int evaluate();
	int getMinConflictColor(int node);
	void printClasses();
	bool moveAllowed(int node, int color);

	std::vector<Move> genMoves(bool ignoreTabulist = false);
	Move getMinConflictMove(const std::vector<Move>& moves);
	Move getRandomMove(const std::vector<Move>& moves);
	Move getBestMove(const std::vector<Move>& moves);
	void move(const Move& move);


};

} /* namespace tcbvrp */

#endif /* SRC_ALGORITHM2_H_ */
