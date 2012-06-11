/*
 *  arrow.h
 *  ampGame
 *
 *  Created by Simon Katan on 28/05/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "baseWeapon.h"


class arrow : public baseWeapon{

	public:
	
	arrow();
	virtual ~arrow();
	
	void initBody(b2World * t);
	
	void update();
	void draw();
	void fire(float amp, float pitch);
	
	
	private:
	
	ofImage img;
	
	


};