/*
 * Algorithm.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#include "Algorithm.h"
#include "Logger.h"

#include <algorithm>

extern int visitedNodes;

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
		LOG << "Trying with k = " << k;
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
	visitedNodes++;
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

int Algorithm::selectUnassignedNode(Solution* solution, Graph& graph)
{
	//Variable Selection Heuristic: minimum remaining values (MRV)
	int node = -1;
	int remainingValues = -1;
	int unassignedNeighbors = -1;
	for (int i = 0; i < solution->getNum_Nodes(); i++)
	{
		if (solution->getColor(i) == -1)
		{
			if (node == -1)
			{
				node = i;
				remainingValues = solution->getDomainValues(i).size();
			} else
			{
				int tmp = solution->getDomainValues(i).size();
				if (remainingValues > tmp)
				{
					node = i;
					remainingValues = tmp;
				} else if (remainingValues == tmp)	//Tie-breaking: degree heuristic. Choose variable which is involved in more constraints (has more unassigned neighbours)
				{
					if(unassignedNeighbors == -1)
						unassignedNeighbors = getUnassignedNeighbours(solution,graph,node);
					int tmp = getUnassignedNeighbours(solution, graph, i);
					if(tmp > unassignedNeighbors)
					{
						unassignedNeighbors = tmp;
						node = i;
					}
				}
			}
		}
	}

//	for (int i = 0; i < solution->getNum_Nodes(); i++)
//	{
//		if(solution->getColor(i) == -1)
//			return i;
//	}
	return node;
}

int Algorithm::getUnassignedNeighbours(Solution* solution, const Graph& graph, int node)
{
	vector<int> neighbours = graph.getNeighbours(node);
	int count = 0;
	for (vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
	{
		if (solution->getColor(*it) == -1)
			count++;
	}
	return count;
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

vector<int> Algorithm::orderColors(int nodeId, Solution* solution, Graph& graph)
{
	//Value Selection Heuristic: least-constraining-value (value that rules out the fewest choices for the neighbours)
	std::vector<int> orderedValues;
	ValueOrder valOrder(solution, graph, nodeId);
	orderedValues = solution->getDomainValues(nodeId);
	std::sort(orderedValues.begin(), orderedValues.end(), valOrder);
	return orderedValues;

//	return solution->getDomainValues(nodeId);
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


bool Algorithm::ValueOrder::operator()(int col1, int col2)
{
	int count_col1 = 0, count_col2 = 0;
	vector<int> neighbours = graph.getNeighbours(nodeId);
	for (vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
	{
		std::vector<int> domain = solution->getDomainValues(*it);
		if(std::find(domain.begin(), domain.end(), col1) != domain.end())
			count_col1++;
		if(std::find(domain.begin(), domain.end(), col2) != domain.end())
			count_col2++;
	}
	return count_col1 < count_col2;
}

} /* namespace graphcoloring */
