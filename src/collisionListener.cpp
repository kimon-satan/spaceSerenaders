/*
 *  collisionListener.cpp
 *  spaceSerenaders
 *
 *  Created by Simon Katan on 04/06/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "collisionListener.h"

collisionListener::collisionListener(){

	vector<int> temp;
	
	for(int i=0;i < 2; i++)destroyList.push_back(temp);

}

vector< vector<int> > collisionListener::getDestroyList(){

	return destroyList;
	
}

void collisionListener::clearDestroyList(){
	
	for(int i =0; i < 2; i ++)destroyList[i].clear();

}

void collisionListener::PostSolve(b2Contact * contact, const b2ContactImpulse *impulse){

	
	const b2Body * a = contact->GetFixtureA()->GetBody();
	const b2Body * b = contact->GetFixtureB()->GetBody();
	
	userData * aData = (userData *)a->GetUserData(); 
	userData * bData = (userData *)b->GetUserData(); 
	
	if(aData && bData){
		
		//if(impulse->normalImpulses[0] > 0.01){
		
		if(aData->objType == 0 && bData->objType == 1){
			
			destroyList[aData->owner].push_back(aData->objIndex);
			
		}else if(bData->objType == 0 && aData->objType == 1){
		
			destroyList[bData->owner].push_back(bData->objIndex);
			
		}
	
	}


}