/*
 *  balloon.h
 *  ampGame
 *
 *  Created by Simon Katan on 28/05/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "baseWeapon.h"
#include "chain.h"
#include "userData.h"
#include "utils.h"

#define MAX_BALLOON 2




class balloon : public baseWeapon {
	
	public:
	
	balloon();
	virtual ~balloon();
	
	void initBody(b2World * t, b2Body * anchor);
	void update();
	void inflate();
	void release();
	void blow(float amp, float peak);
	void draw();
	
	void destroy();
	void slowDestroy();
	chain pop();
	float getSize();
	bool getIsRecieved();
	bool getIsFixed();
	
	
	static int index;
		
	private:
	
	void createFixtures();
	b2RevoluteJoint * createJoint();
	b2Body * createBody();
	
	b2Body * mAnchor;
	b2RevoluteJoint * mJoint;
	b2Fixture * mFixtures[3];
	chain mChain;
	float wind_speed;
	
	float mSize;
	bool isFixed, isRecieved;
	float alpha;
	
	ofImage img;
	int imgNum;
	
	static b2Vec2 lCircPos[5];
	static b2Vec2 rCircPos[5];
	static float   lCircRad[5];
	static float   rCircRad[5];
	
	static b2Vec2 anchorPoint[5];
	static b2Vec2 lTPoint[5];
	static b2Vec2 rTPoint[5];
	
};


