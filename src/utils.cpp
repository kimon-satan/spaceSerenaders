/*
 *  utils.cpp
 *  spaceSerenaders
 *
 *  Created by Simon Katan on 11/06/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "utils.h"

b2Vec2 utils::mul(b2Vec2 b, float mul){

	return b2Vec2(b.x * mul, b.y * mul);
}


b2Vec2 utils::mul(b2Vec2 b, b2Vec2 mul){
	
	return b2Vec2(b.x * mul.x, b.y * mul.y);
	
}