/*
 *  chain.cpp
 *  ampGame
 *
 *  Created by Simon Katan on 29/05/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "chain.h"

chain::chain(){


}


void chain::initBody(b2World * t, b2Body * anchor, int owner){

	mWorld = t;
	
	b2Vec2 a_pos = anchor->GetPosition();
	b2Vec2 pos = a_pos + b2Vec2(0, (float)LINK_LENGTH/2.0f);
	b2Body * b_i = anchor;

 	for(int i = 0; i < NUM_LINKS; i ++){
	
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		
		bd.fixedRotation = false;
		bd.angularDamping = 0.1;
		bd.linearDamping = 0.01;
		bd.position = pos;
		//bd.angle = (ofDegToRad(90));
			
		b2Body * b_ii = mWorld->CreateBody(&bd);
		b_ii->SetSleepingAllowed(true);
		
		b2PolygonShape bShape;
		bShape.SetAsBox(0.01, LINK_LENGTH/2);
		
		b2FixtureDef fd;
		
		fd.shape = &bShape;
		fd.density = 0.1;
		fd.friction = 0.2;
		fd.restitution = 0.01;
		fd.filter.groupIndex = -owner;
		
		b_ii->CreateFixture(&fd);
		
		b2RevoluteJointDef jd;
		jd.Initialize(b_ii, b_i, a_pos);
		
		if(b_i == anchor){
			mJoint = (b2RevoluteJoint*)mWorld->CreateJoint(&jd);
		}else{
			mWorld->CreateJoint(&jd);
		}
		
		links.push_back(b_ii);
		
		b_i = b_ii;
		pos += b2Vec2(0,LINK_LENGTH * 0.9);
		a_pos += b2Vec2(0,LINK_LENGTH * 0.9);
		
		
	}
	

}

void chain::release(){

	mWorld->DestroyJoint(mJoint);
	
}

float chain::getMass(){

	float mass = 0;
	
	for(int i = 0; i < links.size(); i++){
		
		mass += links[i]->GetMass();
	}
	
	return mass;
}


void chain::draw(){
	

	
	ofSetColor(0);
	ofNoFill();
	
	ofBeginShape();

	
	for(int i = 0; i < links.size(); i++){
		
		
		b2Vec2 pos = links[i]->GetPosition();
		/*float angle = links[i]->GetAngle();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		glRotated(ofRadToDeg(angle), 0, 0, 1);
		
		ofRect(0, 0, 0.02, LINK_LENGTH);
		glPopMatrix();*/
		ofVertex(pos.x, pos.y);
	
		
	}
	

	
	ofEndShape(false);
	
	

}


b2Body * chain::getLastLink(){
	
	return links[NUM_LINKS -1];

}

void chain::destroy(){
	
	for(int i = 0; i < links.size(); i++){
		mWorld->DestroyBody(links[i]);
	}
	
	links.clear();
}

chain::~chain(){



}