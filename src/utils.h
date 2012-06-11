/*
 *  utils.h
 *  spaceSerenaders
 *
 *  Created by Simon Katan on 11/06/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once


#include "Box2D.h"
#include "ofMain.h"

class utils{

	public:
	
	static b2Vec2 mul(b2Vec2 b, float mul);
	static b2Vec2 mul(b2Vec2 b, b2Vec2 mul);
	
	

};