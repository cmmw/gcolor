/*
 * Edge.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: ixi
 */

#include "Edge.h"

namespace graphcoloring {

Edge::Edge(int v1, int v2) : v1(v1), v2(v2) {
}

Edge::~Edge() {
}

unsigned int Edge::getV1() const {
	return v1;
}

unsigned int Edge::getV2() const {
	return v2;
}

} /* namespace graphcoloring */
