/*
 * Algorithm2.cpp
 *
 *  Created on: 09.05.2015
 *      Author: Christian
 */

#include "Algorithm2.h"
#include "Logger.h"
#include <algorithm>
#include <climits>

namespace graphcoloring
{

Algorithm2::Algorithm2(const Graph& graph) :
		graph(graph), k(30), bestSolution(graph.getNum_Nodes(), k), bestCosts(INT_MAX), currentSol(graph.getNum_Nodes(), k), tabuListSize(10), maxTries(300)
{

}

Algorithm2::~Algorithm2()
{
}

void Algorithm2::greedyConstHeu()
{
	for (int i = 0; i < graph.getNum_Nodes(); i++)
	{
		std::vector<int> neighbors = graph.getNeighbours(i);
		std::vector<int> domain = bestSolution.getDomainValues(i);
		for (std::vector<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++)
		{
			if (bestSolution.getColor(*it) != -1)
			{
				domain.erase(std::remove(domain.begin(), domain.end(), bestSolution.getColor(*it)), domain.end());
			}
			if (domain.empty())
			{
				LOG << "k too small";
				exit(-1);
			}
		}
		bestSolution.setColor(i, domain.front());
		unsigned int color = domain.front();
		if (colorClasses.size() < color)
		{
			colorClasses.resize(color);
		}
		colorClasses[color - 1].push_back(i);
	}
	bestCosts = evaluate();
	currentSol = bestSolution;
}

Solution Algorithm2::findOptimalSolution()
{
	greedyConstHeu();
	solve();
	return bestSolution;
}

void Algorithm2::solve()
{
	unsigned int count = 0;
	while (count < maxTries)
	{
		std::vector<int> colorClass = colorClasses.back();
		int bestDelta = INT_MAX;
		int bestColor = -1;
		int bestNode = -1;
		std::vector<int> bestNeighbors;
		bool foundMove = false;

		for (std::vector<int>::iterator it = colorClass.begin(); it != colorClass.end(); it++)
		{
//			int color = getMinConflictColor(*it);
//			if (color == -1)
//				continue;

			/***/
			int nodeColor = currentSol.getColor(*it);
			for (int color = 1; (unsigned int) color < colorClasses.size(); color++)		//skip last color
			{
				if (color == nodeColor)
					continue;

				int delta = 0;
				std::vector<int> neighbors = graph.getNeighbours(*it);
				for (std::vector<int>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
				{
					if (color == currentSol.getColor(*neighbor))
						delta += graph.getNeighbours(*neighbor).size();
				}
				delta -= neighbors.size();

				if (moveAllowed(*it, color))		//TODO: Aspiration criteria
				{
					foundMove = true;
					if (delta < bestDelta)
					{
						bestDelta = delta;
						bestColor = color;
						bestNode = *it;
						bestNeighbors = neighbors;
					}
				}
			}
			/***/
		}

		//Move to next solution
		if (foundMove)
		{
			LOG << "Assign node " << bestNode << " color " << bestColor;
			int oldColor = currentSol.getColor(bestNode);
			for (std::vector<int>::iterator it = bestNeighbors.begin(); it != bestNeighbors.end(); it++)
			{
				if (currentSol.getColor(*it) == bestColor)
				{
					currentSol.setColor(*it, oldColor);
					colorClasses[bestColor - 1].erase(std::remove(colorClasses[bestColor - 1].begin(), colorClasses[bestColor - 1].end(), *it), colorClasses[bestColor - 1].end());
					colorClasses[oldColor - 1].push_back(*it);

					//Add move to tabu-list
					if (tabuList.size() > tabuListSize)
						tabuList.pop_front();
					tabuList.push_back(make_pair(*it, bestColor));
				}
			}
			currentSol.setColor(bestNode, bestColor);
			colorClasses[oldColor - 1].erase(std::remove(colorClasses[oldColor - 1].begin(), colorClasses[oldColor - 1].end(), bestNode), colorClasses[oldColor - 1].end());
			colorClasses[bestColor - 1].push_back(bestNode);

			//Add move to tabu-list
			if (tabuList.size() > tabuListSize)
				tabuList.pop_front();
			tabuList.push_back(make_pair(bestNode, oldColor));

			//Removed a color TODO: reset tabu-list here?
			if (colorClasses.back().empty())
			{
				LOG << "************ Removed a color ************";
				colorClasses.pop_back();
				bestCosts = evaluate();
				bestSolution = currentSol;
				count = 0;
			}

		} else
		{
			LOG << "No solution found... reduce size of tabu-list!";
		}
		LOG << "evaluate: " << evaluate();
		count++;
	}
}

int Algorithm2::getMinConflictColor(int node)
{
	int conflicts = INT_MAX;
	int color = -1;
	std::vector<int> neighborColors(colorClasses.size());
	std::vector<int> neighbors = graph.getNeighbours(node);
	unsigned int currentColor = currentSol.getColor(node);

	for (std::vector<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++)
	{
		neighborColors[currentSol.getColor(*it) - 1]++;
	}

	for (unsigned int i = 0; i < colorClasses.size(); i++)
	{
		if (i + 1 == currentColor || !moveAllowed(node, i + 1))
			continue;

		if (neighborColors[i] < conflicts)
		{
			conflicts = neighborColors[i];
			color = i + 1;
			if (conflicts == 0)
				break;
		}
	}
	return color;
}

int Algorithm2::evaluate()
{
	int degree = 0;
	std::vector<int> colorClass = colorClasses.back();
	for (std::vector<int>::iterator it = colorClass.begin(); it != colorClass.end(); it++)
	{
		degree += graph.getIncidentEdges(*it).size();
	}
	return degree;
}

void Algorithm2::printClasses()
{
	int i = 1;
	for (std::vector<std::vector<int> >::iterator colorClass = colorClasses.begin(); colorClass != colorClasses.end(); colorClass++)
	{
		LOG << "Nodes with color " << i << ":";
		for (std::vector<int>::iterator it = colorClass->begin(); it != colorClass->end(); it++)
		{
			LOG << *it;
		}
		i++;
	}
}

bool Algorithm2::moveAllowed(int node, int color)
{
	for (unsigned int i = 0; i < tabuList.size(); i++)
	{
		if (tabuList[i].first == node && tabuList[i].second == color)
			return false;
	}
	return true;
}

} /* namespace tcbvrp */