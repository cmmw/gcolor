/*
 * Algorithm.h
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#ifndef SRC_ALGORITHM_H_
#define SRC_ALGORITHM_H_

#include "Solution.h"
#include "Graph.h"

namespace graphcoloring {

class Algorithm {
public:
	Algorithm();
	virtual ~Algorithm();

	Solution* findOptimalSolution(Graph& graph);

	Solution* backtrack(Solution* solution, Graph& graph);

	int selectUnassignedNode(Solution* solution, Graph& graph);

	int getUnassignedNeighbours(Solution* solution, const Graph& graph, int node);

	bool inferences(Solution* solution, Graph& graph, int lastSetNodeId);

	vector<int> orderColors(int nodeId, Solution* solution, Graph& graph);

	bool assignmentIsConsistent(int nodeId, int color, Solution* solution, Graph& graph);

	bool simpleForwardChecking(Solution* solution, Graph& graph, int lastSetNodeId);

	bool maintainingArcConsistency(Solution* solution, Graph& graph, int lastSetNodeId);

	bool revise(Solution* solution, Graph& graph, int nodeId1, int nodeId2);
};

} /* namespace graphcoloring */

#endif /* SRC_ALGORITHM_H_ */
