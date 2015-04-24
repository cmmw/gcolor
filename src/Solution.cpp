/*
 * Solution.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#include "Solution.h"
#include <iostream>

#include <iostream>
#include <fstream>
#include "Constants.h"

extern int visitedNodes;

namespace graphcoloring {

using namespace std;

Solution::Solution(int num_nodes, int k) : colors(num_nodes, -1), k(k),  num_nodes(num_nodes) {

	// initialize domainValues for each node with full color vector
	for (int i=0; i<num_nodes; i++) {
		vector<int> vec;

		for (int j=1; j<=k; j++) {
			vec.push_back(j);
		}

		domainValues.push_back(vec);
	}
}

Solution::~Solution() {
}

int Solution::getColor(int nodeId) const {
	return colors[nodeId];
}

void Solution::setColor(int nodeId, int color) {
	colors[nodeId] = color;
}

vector<int> Solution::getDomainValues(int nodeId) {
	return domainValues[nodeId];
}

void Solution::setDomainValues(int nodeId, vector<int> values) {
	domainValues[nodeId] = values;
}

int Solution::getK() const {
	return k;
}

int Solution::getNum_Nodes() const {
	return num_nodes;
}

void Solution::printSolution() const {
	cout << "Solution: " << endl;
	for (int i=0; i<num_nodes; i++) {
		cout << i << ": " << colors[i] << endl;
	}
	cout << "Visited nodes: " << visitedNodes << std::endl;
}

void Solution::writeGraphVizFile(string fileName, Graph graph) const {
	ofstream outFile(fileName.c_str());
	  if (outFile.is_open())
	  {
		  outFile << "graph G {\n";

		  outFile << "  {\n";
		  outFile << "    " << graphviz_node_settings << "\n";

		  for (int i=0; i<num_nodes; i++) {
			  outFile << "    n" << i << " [fillcolor=" << graphviz_colors[colors[i]-1] << "]\n";
		  }

		  outFile << "  }\n";


		  for (int i=0; i<graph.getNum_Edges(); i++) {
			  outFile << "  n" << graph.getEdge(i).getV1()
					  << " -- n" << graph.getEdge(i).getV2() << ";\n";
		  }

		  outFile << "}\n";

		  outFile.close();
	  }
	  else {
		  cout << "Unable to open file for output: " << fileName << endl;
	  }

}

} /* namespace graphcoloring */
