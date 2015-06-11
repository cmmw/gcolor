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
		graph(graph), k(30), bestSolution(graph.getNum_Nodes(), k), bestCosts(INT_MAX), currentSol(graph.getNum_Nodes(), k), tabuListSize(30), maxTries(5000)
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
	LOG << "Colors: " << colorClasses.size();
}

Solution Algorithm2::findOptimalSolution()
{
	/*Initial solution*/
	greedyConstHeu();

	unsigned int i = 0;
	while (i < maxTries)
	{
		std::vector<Move> moves = genMoves();
//		Move _move = getBestMove(moves);
//		Move _move = getRandomMove(moves);
		Move _move = getMinConflictMove();

		move(_move);
		i++;
	}

	return bestSolution;
}

void Algorithm2::move(const Move& move)
{
	std::vector<int> bestNeighbors = graph.getNeighbours(move.node);
	int newColor = move.color;
	int oldColor = currentSol.getColor(move.node);
	for (std::vector<int>::iterator it = bestNeighbors.begin(); it != bestNeighbors.end(); it++)
	{

		if (currentSol.getColor(*it) == newColor)
		{
			currentSol.setColor(*it, oldColor);
			colorClasses[newColor - 1].erase(std::remove(colorClasses[newColor - 1].begin(), colorClasses[newColor - 1].end(), *it), colorClasses[newColor - 1].end());
			colorClasses[oldColor - 1].push_back(*it);

			//Add move to tabu-list
			if (tabuList.size() > tabuListSize)
				tabuList.pop_front();
			tabuList.push_back(make_pair(*it, newColor));
		}
	}
	currentSol.setColor(move.node, newColor);
	colorClasses[oldColor - 1].erase(std::remove(colorClasses[oldColor - 1].begin(), colorClasses[oldColor - 1].end(), move.node), colorClasses[oldColor - 1].end());
	colorClasses[newColor - 1].push_back(move.node);

	//Add move to tabu-list
	if (tabuList.size() > tabuListSize)
		tabuList.pop_front();
	tabuList.push_back(make_pair(move.node, oldColor));

	//Removed a color TODO: reset tabu-list here?
	if (colorClasses.back().empty())
	{
		colorClasses.pop_back();
		bestCosts = evaluate();
		bestSolution = currentSol;
		LOG << "Colors: " << colorClasses.size();
	}
}

std::vector<Algorithm2::Move> Algorithm2::genMoves(bool ignoreTabulist)
{
	std::vector<Move> moves;
	std::vector<int> colorClass = colorClasses.back();
	for (std::vector<int>::iterator it = colorClass.begin(); it != colorClass.end(); it++)
	{
		int nodeColor = currentSol.getColor(*it);
		for (int color = 1; (unsigned int) color < colorClasses.size(); color++)		//skip last color
		{
			if (color == nodeColor || (!ignoreTabulist && !moveAllowed(*it, color)))
				continue;

			int delta = 0;
			std::vector<int> neighbors = graph.getNeighbours(*it);
			for (std::vector<int>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
			{

				if (color == currentSol.getColor(*neighbor))
					delta += graph.getNeighbours(*neighbor).size();
			}
			delta -= neighbors.size();
			moves.push_back(Move(*it, color, delta));
		}
	}
	return moves;
}

Algorithm2::Move Algorithm2::getMinConflictMove()
{
	Move m;
	do
	{
		m.node = rand() % graph.getNum_Nodes();
		m.color = getMinConflictColor(m.node);
	} while (m.color == -1);
	return m;
}

Algorithm2::Move Algorithm2::getRandomMove(const std::vector<Move>& moves)
{
	int idx = rand() % moves.size();
	return moves[idx];
}

Algorithm2::Move Algorithm2::getBestMove(const std::vector<Algorithm2::Move>& moves)
{
	Move bestMove = moves[0];
	for (std::vector<Move>::const_iterator it = moves.begin(); it != moves.end(); it++)
	{
		if (it->deltaCosts < bestMove.deltaCosts)
		{
			bestMove = *it;
		}
	}
	return bestMove;
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
