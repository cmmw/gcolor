/*
 * Solution.h
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#ifndef SRC_SOLUTION_H_
#define SRC_SOLUTION_H_

#include <vector>

namespace graphcoloring {

using namespace std;

class Solution {
public:
	Solution(int num_nodes, int k);
	virtual ~Solution();

	int getColor(int nodeId) const;
	void setColor(int nodeId, int color);

	vector<int> getDomainValues(int nodeId) const;
	void setDomainValues(int nodeId, vector<int> values);

	int getK() const;
	int getNum_Nodes() const;

	void printSolution() const;
private:
	vector<int> colors;
	vector<vector<int> > domainValues;
	int k;
	int num_nodes;
};

} /* namespace graphcoloring */

#endif /* SRC_SOLUTION_H_ */
