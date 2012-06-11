/*
 *  baseWeapon.cpp
 *  spaceSerenaders
 *
 *  Created by Simon Katan on 01/06/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "baseWeapon.h"

void baseWeapon::destroy(){
	
	if(mBody){
		mWorld->DestroyBody(mBody);
	}
	if(ud)delete ud;
	isDestroyed = true;
	
}


bool baseWeapon::getIsDestroyed(){
	
	return isDestroyed;
	
}

//getters and setters

b2Vec2 baseWeapon::getPosition(){
	
	return mBody->GetPosition();
}

void baseWeapon::setOwner(int t){

	owner = t;
	
}

int baseWeapon::getOwner(){

	return owner;
}

int baseWeapon::getIndex(){
	
	return mIndex;

}


void baseWeapon::setPos(b2Vec2 t_pos){

	mPos = t_pos;

}