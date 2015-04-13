/*
 * Edge.h
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#ifndef SRC_EDGE_H_
#define SRC_EDGE_H_

namespace graphcoloring {

class Edge {
public:
	Edge(int v1, int v2);
	virtual ~Edge();

	int getV1() const;
	int getV2() const;

private:
	int v1, v2;
};

} /* namespace graphcoloring */

#endif /* SRC_EDGE_H_ */
