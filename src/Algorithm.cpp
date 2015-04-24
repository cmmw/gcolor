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

	std::list<int> colors;
	std::list<int> values;

	for (int i = 1; i <= solution->getK(); i++)
	{
		int value = countOccurrence(i, graph, solution, nodeId);
		int pos = 0;
		for (std::list<int>::iterator it = values.begin(); it != values.end(); it++)
		{
			if (value < *it)
				break;
			pos++;
		}
		std::list<int>::iterator valIt = values.begin();
		std::list<int>::iterator colIt = colors.begin();
		std::advance(valIt, pos);
		std::advance(colIt, pos);
		values.insert(valIt, value);
		colors.insert(colIt, i);
	}

	orderedValues = std::vector<int>(colors.begin(), colors.end());
	return orderedValues;

//	return solution->getDomainValues(nodeId);
}


int Algorithm::countOccurrence(int col, const Graph& graph, Solution* solution, int nodeId)
{
	vector<int> neighbours = graph.getNeighbours(nodeId);
	int count = 0;
	for (vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
	{
		if (solution->getColor(*it) != -1)
			continue;

		std::vector<int> domain = solution->getDomainValues(*it);
		if (std::find(domain.begin(), domain.end(), col) != domain.end())
			count++;
	}
	return count;
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
