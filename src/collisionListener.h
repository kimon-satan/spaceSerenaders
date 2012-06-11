/*
 *  collisionListener.h
 *  spaceSerenaders
 *
 *  Created by Simon Katan on 04/06/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "Box2D.h"
#include "ofMain.h"
#include "userData.h"
#include "balloon.h"

class collisionListener: public b2ContactListener{

	public:
	
	collisionListener();
	
	void PostSolve(b2Contact * contact, const b2ContactImpulse *impulse);

	vector< vector<int> > getDestroyList();
	void clearDestroyList();
	
	private:
	
	vector< vector<int> > destroyList;
	

};