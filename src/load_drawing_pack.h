/*
 * load_drawing_pack.h
 *
 *  Created on: Aug 23, 2013
 *      Author: lucky
 */
#include <fstream>
#include "drawing.h"
#ifndef LOAD_DRAWING_PACK_H_
#define LOAD_DRAWING_PACK_H_
class load_drawing_pack{
public:
	bool load(std::vector<drawing>&drawing_holder);
};



#endif /* LOAD_DRAWING_PACK_H_ */
