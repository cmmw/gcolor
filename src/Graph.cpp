/*
 * Graph.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "Logger.h"



namespace graphcoloring {

using namespace std;

Graph::Graph() : num_nodes(-1), num_edges(-1) {
}

Graph::Graph(string fileName) : num_nodes(0), num_edges(0) {
	ifstream ifs( fileName.c_str() );
	if( ifs.fail() ) {
		cerr << "could not open input file " << fileName << "\n";
		exit( -1 );
	}

	string line;
	string garbage;
	int edgeId = 0;

	LOG << "Reading instance from file " << fileName << "\n";

	while (getline(ifs, line)) {
		istringstream output(line);


		switch (line[0]) {
		case 'c':
			// comment line, do nothing
			break;
		case 'p':
			// header line
			output >> garbage >> garbage >> num_nodes >> num_edges;

			incidentEdges.resize( num_nodes );
			break;
		case 'e':
			if (num_nodes == 0) {
				cout << "Edge line found before header line. Input file invalid?" << endl;
			}
			else {
				// edge line
				unsigned int v1, v2;
				output >> garbage >> v1 >> v2;
				if(v1 == v2)
					break;
				edges.push_back(Edge(v1-1, v2-1));			//Ignore self-loops
				incidentEdges[v1-1].push_back( edgeId );
				incidentEdges[v2-1].push_back( edgeId );
				edgeId++;
			}
			break;
		default:
			break;
		}
	}

	ifs.close();

	num_edges = edges.size();

	LOG << "Number of nodes: " << num_nodes << "\n";
	LOG << "Number of edges: " << num_edges << "\n";

//	cout << "Incidency list:" << "\n";
//	for( int v = 0; v < num_nodes; v++ ) {
//		cout << v << ": ";
//		for (vector<unsigned int>::iterator it = incidentEdges[v].begin();
//				it != incidentEdges[v].end(); it++) {
//			cout << "(" << edges[*it].getV1() << "," << edges[*it].getV2() << "), ";
//		}
//		cout << "\n";
//	}


}

Graph::~Graph() {
}


Edge Graph::getEdge(int edgeId) const {
	return edges[edgeId];
}

vector<Edge> Graph::getIncidentEdges(int nodeId) const {
	vector<Edge> temp;

	for (vector<unsigned int>::const_iterator it = incidentEdges[nodeId].begin();
			it != incidentEdges[nodeId].end(); it++) {
		temp.push_back(edges[*it]);
	}

	return temp;
}

vector<int> Graph::getNeighbours(int nodeId) const {
	vector<int> neighbours;

	for (vector<unsigned int>::const_iterator it = incidentEdges[nodeId].begin();
			it != incidentEdges[nodeId].end(); it++) {

		if (edges[*it].getV1() == nodeId) {
			neighbours.push_back(edges[*it].getV2());
		}
		else {
			neighbours.push_back(edges[*it].getV1());
		}
	}


	return neighbours;
}

int Graph::getNum_Nodes() const {
	return num_nodes;
}
int Graph::getNum_Edges() const {
	return num_edges;
}

} /* namespace graphcoloring */
