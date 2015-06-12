/*
 * Algorithm.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#include "Algorithm.h"
#include "Logger.h"

#include <algorithm>
#include "time.h"

#define TIME_PASSED double(clock() - begin) / CLOCKS_PER_SEC


extern int visitedNodes;
extern int triedColors;

extern int mac;
extern int mrv;
extern int lcv;
extern clock_t begin;
extern double time_limit;

extern int alg;

bool firstRun = true;


namespace graphcoloring {

Algorithm::Algorithm() {
}

Algorithm::~Algorithm() {
}

// Repeatedly launch backtracking to find solution with minimum k
Solution* Algorithm::findOptimalSolution(Graph& graph) {
	int startK = 0;
	int endK = graph.getNum_Nodes();
	int k = (endK - startK) / 2;
	Solution* solution;
	Solution* bestSolution = NULL;
	bool firstRun = true;
	while (time_limit == -1 || (alg == 1 && firstRun) || TIME_PASSED < time_limit) {

		LOG << "Trying with k = " << k;
		LOG << "searching in [" << startK << ", " << endK << "]";
		solution = backtrack(new Solution(graph.getNum_Nodes(), k), graph);

		if (solution != NULL) {
			int step = (endK - k) / 2;
			if(step == 0)
				step = 1;
			LOG << "found solution (current time " << TIME_PASSED << "): ";
			solution->printSolution();
			endK = k;
			k -= step;
			delete bestSolution;
			bestSolution = solution;
		}
		else {
			int step = (k - startK) / 2;
			if(step == 0)
				step = 1;
			LOG << "not found";
			startK = k;
			k += step;
		}

		if ((endK - startK) == 1)
			break;
		firstRun = false;

	}
	return bestSolution;
}


// Backtracking algorithm as described in the Book:
// "Artificial Intelligence: A Modern Approach" by Stuart Russel and Peter Norvig
Solution* Algorithm::backtrack(Solution* solution, Graph& graph) {
	if ((alg == 0 || !firstRun) && time_limit != -1 && TIME_PASSED > time_limit) {
		return NULL;
	}

	int nodeId = selectUnassignedNode(solution, graph);
	visitedNodes++;
	if (nodeId == -1) {
		// all nodes are colored, return solution
		return solution;
	}

	vector<int> domainValues = orderColors(nodeId, solution, graph);

	//cout << "Colorlist size: " << domainValues.size() << endl;
	for (vector<int>::iterator it = domainValues.begin(); it != domainValues.end();
			it++) {

		triedColors++;
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
	if (mrv > 0) {
		return MRV(solution, graph);
	}
	else {
		return simpleSelectionHeuristic(solution, graph);
	}
}

int Algorithm::simpleSelectionHeuristic(Solution* solution, const Graph& graph)
{
	for (int i = 0; i < solution->getNum_Nodes(); i++)
	{
		if (solution->getColor(i) == -1)
			return i;
	}
	return -1;
}

int Algorithm::MRV(Solution* solution, const Graph& graph)
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
					if (unassignedNeighbors == -1)
						unassignedNeighbors = getUnassignedNeighbours(solution, graph, node);
					int tmp = getUnassignedNeighbours(solution, graph, i);
					if (tmp > unassignedNeighbors)
					{
						unassignedNeighbors = tmp;
						node = i;
					}
				}
			}
		}
	}
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
	if (mac > 0) {
		return maintainingArcConsistency(solution, graph, lastSetNodeId);
	}
	else {
		return simpleForwardChecking(solution, graph, lastSetNodeId);
	}
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

bool Algorithm::maintainingArcConsistency(Solution* solution, Graph& graph, int lastSetNodeId) {
	list<Edge> edges;
	vector<Edge> incidentEdges = graph.getIncidentEdges(lastSetNodeId);

	for (vector<Edge>::iterator it = incidentEdges.begin(); it != incidentEdges.end(); it++) {
		if (solution->getColor(it->getV1()) != -1 || solution->getColor(it->getV2()) != -1) {
			if (it->getV1() == lastSetNodeId) {
				edges.push_back(Edge(it->getV2(), it->getV1()));
			}
			else {
				edges.push_back(Edge(it->getV1(), it->getV2()));
			}
		}
	}

	for (list<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
		if (revise(solution, graph, it->getV1(), it->getV2())) {
			if (solution->getDomainValues(it->getV1()).size() == 0) {
				return false;
			}

			incidentEdges = graph.getIncidentEdges(it->getV1());

			for (vector<Edge>::iterator it2 = incidentEdges.begin(); it2 != incidentEdges.end(); it2++) {
				if (it2->getV1() != it->getV2() && it2->getV2() != it->getV2()
						&& (solution->getColor(it2->getV1()) == -1 || solution->getColor(it2->getV2()) == -1)) {
					if (it2->getV1() == it->getV1()) {
						edges.push_back(Edge(it2->getV2(), it2->getV1()));
					}
					else {
						edges.push_back(Edge(it2->getV1(), it2->getV2()));
					}
				}
			}
		}
	}

	return true;
}

bool Algorithm::revise(Solution* solution, Graph& graph, int nodeId1, int nodeId2) {
	bool revised = false;

	vector<int> oldDomainValues1 = solution->getDomainValues(nodeId1);

	list<int> domainValues1(oldDomainValues1.begin(), oldDomainValues1.end() );

	vector<int> domainValues2;
	if (solution->getColor(nodeId2) != -1) {
		domainValues2.push_back(solution->getColor(nodeId2));
	}
	else {
		domainValues2 = solution->getDomainValues(nodeId2);
	}

	for (list<int>::iterator it = domainValues1.begin(); it != domainValues1.end();) {
		if (domainValues2.size() == 1 && *it == domainValues2.front()) {
			it = domainValues1.erase(it);
			revised = true;
		}
		else {
			it++;
		}
	}

	vector<int> newDomainValues1( domainValues1.begin(), domainValues1.end() );

	solution->setDomainValues(nodeId1, newDomainValues1);

	return revised;
}


vector<int> Algorithm::orderColors(int nodeId, Solution* solution, Graph& graph)
{
	if (lcv > 0) {
		return leastConstrainingValue(nodeId, solution, graph);
	}
	else {
		return simpleValueOrdering(nodeId, solution, graph);
	}
}

vector<int> Algorithm::leastConstrainingValue(int nodeId, Solution* solution, Graph& graph)
{
	//Value Selection Heuristic: least-constraining-value (value that rules out the fewest choices for the neighbors)
	std::vector<int> orderedValues;

	std::list<int> colors;
	std::list<int> values;

	vector<int> domainValues = solution->getDomainValues(nodeId);
	for (vector<int>::iterator it = domainValues.begin(); it != domainValues.end(); it++)
	{
		int value = countOccurrence(*it, graph, solution, nodeId);

		if (value != -1) {
			int pos = 0;
			for (std::list<int>::iterator it2 = values.begin(); it2 != values.end(); it2++)
			{
				if (value < *it2)
					break;
				pos++;
			}
			std::list<int>::iterator valIt = values.begin();
			std::list<int>::iterator colIt = colors.begin();
			std::advance(valIt, pos);
			std::advance(colIt, pos);
			values.insert(valIt, value);
			colors.insert(colIt, *it);

		}
	}

	orderedValues = std::vector<int>(colors.begin(), colors.end());
	return orderedValues;
}

vector<int> Algorithm::simpleValueOrdering(int nodeId, Solution* solution, Graph& graph)
{
	return solution->getDomainValues(nodeId);
}

int Algorithm::countOccurrence(int col, const Graph& graph, Solution* solution, int nodeId)
{
	vector<int> neighbours = graph.getNeighbours(nodeId);
	int count = 0;
	for (vector<int>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
	{
		if (solution->getColor(*it) == -1) {

			std::vector<int> domain = solution->getDomainValues(*it);
			if (std::find(domain.begin(), domain.end(), col) != domain.end())
				count++;
		}
		else if (solution->getColor(*it) == col) {
			return -1;
		}
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
