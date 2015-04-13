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

namespace graphcoloring {

using namespace std;

Solution::Solution(int num_nodes, int k) : colors(num_nodes, -1), k(k),  num_nodes(num_nodes) {
	vector<int> vec;
	for (int i=1; i<=k; i++) {
		vec.push_back(i);
	}

	// initialize domainValues for each node with all colors
	for (int i=0; i<num_nodes; i++) {
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

vector<int> Solution::getDomainValues(int nodeId) const {
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
