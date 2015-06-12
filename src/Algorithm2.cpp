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
#include <time.h>

#define TIME_PASSED double(clock() - begin) / CLOCKS_PER_SEC

extern clock_t begin;

namespace graphcoloring
{

Algorithm2::Algorithm2(const Graph& graph, double tl) :
		graph(graph), k(graph.getNum_Nodes()), bestSolution(graph.getNum_Nodes(), graph.getNum_Nodes()), bestCosts(INT_MAX), currentSol(graph.getNum_Nodes(), graph.getNum_Nodes()), tabuListSize(30), maxTries(25000), timeLimit(-1), p(5), tl(tl)
{

}

Algorithm2::Algorithm2(const Graph& graph, int p, int iterationLimit, double tl) :
		graph(graph), k(graph.getNum_Nodes()), bestSolution(graph.getNum_Nodes(), graph.getNum_Nodes()), bestCosts(INT_MAX), currentSol(graph.getNum_Nodes(), graph.getNum_Nodes()), tabuListSize(30), maxTries(iterationLimit), timeLimit(-1), p(p), tl(tl)
{

}
Algorithm2::Algorithm2(const Graph& graph, int p, double timeLimit, double tl) :
		graph(graph), k(graph.getNum_Nodes()), bestSolution(graph.getNum_Nodes(), graph.getNum_Nodes()), bestCosts(INT_MAX), currentSol(graph.getNum_Nodes(), graph.getNum_Nodes()), tabuListSize(30), maxTries(-1), timeLimit(timeLimit), p(p), tl(tl)
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
	}
	currentSol = bestSolution;
}

Solution Algorithm2::findOptimalSolution(const Solution* solution)
{
	/*Initial solution*/
	if (solution == NULL)
	{
		greedyConstHeu();
	} else
	{
		currentSol = *solution;
		bestSolution = *solution;
	}

	/*Init color classes*/
	for (int i = 0; i < graph.getNum_Nodes(); i++)
	{
		unsigned int color = currentSol.getColor(i);
		if (colorClasses.size() < color)
		{
			colorClasses.resize(color);
		}
		colorClasses[color - 1].push_back(i);
	}
	bestCosts = evaluate();
	LOG << "Colors: " << colorClasses.size();

	int i = 0;
	bool end = false;
	while (!end)
	{
		int r = rand() % 100;
		Move _move;

		if (r >= 100 - p)
		{
			_move = getRandomMove();
		}
		else
		{
			std::vector<Move> moves = genMoves();
			if (moves.empty())
			{
				tabuList.erase(tabuList.begin(), tabuList.begin() + tabuListSize / 2);
				tabuListSize = tabuList.size();
				std::cout << tabuListSize << std::endl;
				continue;
			}
			_move = getBestMove(moves);
		}

//		Move _move = getMinConflictMove();

		move(_move);
		i++;

		if (maxTries == -1)
		{
			end = TIME_PASSED > timeLimit;
		}
		else
		{
			end = i > maxTries;
		}
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
	//for (std::vector<int>::iterator it = colorClass.begin(); it != colorClass.end(); it++)
	//{
	//
	//  int nodeId = *it;

	// NOTE von Felix: ich wuerds gern mal so probieren, dass er immer einen
	// zufaelligen Knoten aus der letzten Klasse auswaehlt so wie es im Paper steht:
	// http://leeds-faculty.colorado.edu/glover/TS%20-%20vignettes%20-%20coloring%20-%20Malaguti%20&%20Toth%20Extended.pdf
	// Koennen wir aber evt. spaeter wieder aendern.

	int random_idx = 1 + (rand() % (int) (colorClass.size() - 1 + 1));
	int nodeId = colorClass[random_idx - 1];
	int currentCost = evaluate();

	int nodeColor = currentSol.getColor(nodeId);

	for (int color = 1; (unsigned int) color < colorClasses.size(); color++)		//skip last color
	{

		if (color == nodeColor /*|| (!ignoreTabulist)*/) // NOTE von Felix: Was macht das ignoreTabuList genau hier?
			continue;

		int delta = 0;
		std::vector<int> neighbors = graph.getNeighbours(nodeId);
		for (std::vector<int>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
		{

			if (color == currentSol.getColor(*neighbor))
				delta += graph.getNeighbours(*neighbor).size();
		}
		delta -= neighbors.size();

		bool aspiration_criteria = false;

		if (currentCost + delta < bestCosts)
		{
			bestCosts = currentCost + delta;
			aspiration_criteria = true;
		}

		if (aspiration_criteria || moveAllowed(nodeId, color))
		{
			moves.push_back(Move(nodeId, color, delta));
		}
	}
	//}
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

Algorithm2::Move Algorithm2::getRandomMove()
{
	std::vector<int> colorClass = colorClasses.back();
	int random_idx = 1 + (rand() % (int) (colorClass.size() - 1 + 1));
	int nodeId = colorClass[random_idx - 1];

	int color = 1 + (rand() % (int) (colorClasses.size() - 1));

	int delta = 0;
	std::vector<int> neighbors = graph.getNeighbours(nodeId);
	for (std::vector<int>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
	{

		if (color == currentSol.getColor(*neighbor))
			delta += graph.getNeighbours(*neighbor).size();
	}
	delta -= neighbors.size();

	return Move(nodeId, color, delta);
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
