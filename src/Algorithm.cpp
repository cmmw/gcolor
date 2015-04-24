/*
 * Algorithm.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#include "Algorithm.h"

#include <algorithm>

namespace graphcoloring {

Algorithm::Algorithm() {
}

Algorithm::~Algorithm() {
}

// Repeatedly launch backtracking to find solution with minimum k
Solution* Algorithm::findOptimalSolution(Graph& graph) {
	int k = 1;
	bool solutionFound = false;
	Solution* solution;
	while (!solutionFound) {
		solution = backtrack(new Solution(graph.getNum_Nodes(), k), graph);

		if (solution != NULL) {
			solutionFound = true;
			return solution;
		}
		else {
			k++;
		}
	}
	// should never be reached
	return NULL;
}


// Backtracking algorithm as described in the Book:
// "Artificial Intelligence: A Modern Approach" by Stuart Russel and Peter Norvig
Solution* Algorithm::backtrack(Solution* solution, Graph& graph) {
	int nodeId = selectUnassignedNode(solution, graph);

	if (nodeId == -1) {
		// all nodes are colored, return solution
		return solution;
	}

	vector<int> domainValues = orderColors(nodeId, solution, graph);

	for (vector<int>::iterator it = domainValues.begin(); it != domainValues.end();
			it++) {
		int color = *it;
		if (assignmentIsConsistent(nodeId, color, solution, graph)) {
			Solution* newSolution = new Solution(*solution);
			newSolution->setColor(nodeId, color);

			if (inferences(newSolution, graph, nodeId)) {
				Solution* backtrackSolution;
				backtrackSolution = backtrack(newSolution, graph);

				if (backtrackSolution != NULL) {
					delete solution;
					return backtrackSolution;
				}
			}

			delete newSolution;
		}
	}
	return NULL;

}

int Algorithm::selectUnassignedNode(Solution* solution, Graph& graph) {
	// TODO: implement Variable Selection Heuristic here


	// just return the next node which is not set yet
	for (int i=0; i<solution->getNum_Nodes(); i++) {
		if (solution->getColor(i) == -1) {
			return i;
		}
	}

	// return -1 if all nodes are set already
	return -1;
}

bool Algorithm::inferences(Solution* solution, Graph& graph, int lastSetNodeId) {
	return simpleForwardChecking(solution, graph, lastSetNodeId);
}

bool Algorithm::simpleForwardChecking(Solution* solution, Graph& graph, int lastSetNodeId) {
	int color = solution->getColor(lastSetNodeId);
	vector<int> neighbours = graph.getNeighbours(lastSetNodeId);
	for (vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
		if (solution->getColor(*it) == -1) {
			vector<int> domainValues = solution->getDomainValues(*it);
			domainValues.erase( std::remove( domainValues.begin(), domainValues.end(), color ), domainValues.end() );

			if (domainValues.size() == 0) {
				return false;
			}
			else {
				solution->setDomainValues(*it, domainValues);
			}
		}
	}

	return true;
}

vector<int> Algorithm::orderColors(int nodeId, Solution* solution, Graph& graph) {
	// TODO: implement Value Selection Heuristic here

	// just return domainValues in standard order
	return solution->getDomainValues(nodeId);
}

bool Algorithm::assignmentIsConsistent(int nodeId, int color, Solution* solution, Graph& graph) {
	// just check if the newly assigned variable causes any inconsistencies to previously set colors
	vector<int> neighbours = graph.getNeighbours(nodeId);
	for (vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
		if (color == solution->getColor(*it)) {
			return false;
		}
	}

	return true;
}


} /* namespace graphcoloring */
