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

	void greedyConstHeu();
	void solve();
	int evaluate();
	int getMinConflictColor(int node);
	void printClasses();
	bool moveAllowed(int node, int color);

};

} /* namespace tcbvrp */

#endif /* SRC_ALGORITHM2_H_ */
