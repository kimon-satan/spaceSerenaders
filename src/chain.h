/*
 *  chain.h
 *  ampGame
 *
 *  Created by Simon Katan on 29/05/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Box2D.h"
#define NUM_LINKS 12
#define LINK_LENGTH 0.25

class chain{

	public:
	
	chain();
	virtual ~chain();
	
	void initBody(b2World * t, b2Body * anchor ,int owner);
	void draw();
	void release();
	b2Body * getLastLink();
	float getMass();
	
	void destroy();
	
	private:
	
	b2World * mWorld;
	b2RevoluteJoint * mJoint;
	vector <b2Body *> links;
	

};

