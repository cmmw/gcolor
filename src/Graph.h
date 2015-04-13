/*
 * Graph.h
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#ifndef SRC_GRAPH_H_
#define SRC_GRAPH_H_

#include <vector>
#include <list>
#include <string>
#include "Edge.h"

namespace graphcoloring {

using namespace std;

class Graph {
public:
	Graph();
	Graph(string fileName);
	virtual ~Graph();



private:
	int num_nodes;
	int num_edges;

	vector<Edge> edges;
	vector<list<unsigned int> > incidentEdges;
};

} /* namespace graphcoloring */

#endif /* SRC_GRAPH_H_ */
