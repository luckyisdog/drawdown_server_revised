/*
 * drawing.h
 *
 *  Created on: Aug 23, 2013
 *      Author: lucky
 */
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include "coordinate_point.h"
#ifndef DRAWING_H_
#define DRAWING_H_

class drawing{
	std::string name;
	std::vector<coordinate_point> data;
	std::string retrieveData();
};



#endif /* DRAWING_H_ */
