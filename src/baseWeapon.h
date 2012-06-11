/*
 *  baseWeapon.h
 *  spaceSerenaders
 *
 *  Created by Simon Katan on 01/06/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "Box2D.h"
#include "ofMain.h"
#include "userData.h"

class baseWeapon{

	public:
	

	virtual void update() = 0;
	virtual void draw() = 0;

	virtual void destroy();
	
	//getters and setters
	
	b2Vec2 getPosition();
	bool getIsDestroyed();
	void setOwner(int t);
	int getOwner();
	int getIndex();
	void setPos(b2Vec2 t_pos);
	
	
	protected:
	
	int owner;
	
	b2Body * mBody;
	b2World * mWorld;
	b2Vec2 mPos;
	float angle;
	int mIndex;
	bool isDestroyed;
	userData * ud;

};