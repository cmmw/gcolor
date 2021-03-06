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

private:
	int selectUnassignedNode(Solution* solution, Graph& graph);

	int simpleSelectionHeuristic(Solution* solution, const Graph& graph);

	int MRV(Solution* solution, const Graph& graph);

	int getUnassignedNeighbours(Solution* solution, const Graph& graph, int node);

	bool inferences(Solution* solution, Graph& graph, int lastSetNodeId);

	vector<int> orderColors(int nodeId, Solution* solution, Graph& graph);

	vector<int> leastConstrainingValue(int nodeId, Solution* solution, Graph& graph);

	vector<int> simpleValueOrdering(int nodeId, Solution* solution, Graph& graph);

	int countOccurrence(int col, const Graph& graph, Solution* solution, int nodeId);

	bool assignmentIsConsistent(int nodeId, int color, Solution* solution, Graph& graph);

	bool simpleForwardChecking(Solution* solution, Graph& graph, int lastSetNodeId);

	bool maintainingArcConsistency(Solution* solution, Graph& graph, int lastSetNodeId);

	bool revise(Solution* solution, Graph& graph, int nodeId1, int nodeId2);


	class ValueOrder
	{

	public:
		ValueOrder(Solution* solution, const Graph& graph, int nodeId) : solution(solution), graph(graph), nodeId(nodeId)
		{
		}

		bool operator()(int col1, int col2);

	private:
		Solution* solution;
		const Graph graph;
		int nodeId;

	};

};

} /* namespace graphcoloring */

#endif /* SRC_ALGORITHM_H_ */
