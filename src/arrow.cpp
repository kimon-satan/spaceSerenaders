/*
 *  arrow.cpp
 *  ampGame
 *
 *  Created by Simon Katan on 28/05/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "arrow.h"

arrow::arrow(){

	mPos.Set(-10,-5);
	isDestroyed = false;
	
}

void arrow::initBody(b2World * t){
	
	
	mWorld = t;
	
	int imgNum = ofRandom(1,3);
	string fp = "arrow" + ofToString(imgNum,0) + ".png";
	img.loadImage(fp);
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = mPos;
	bd.fixedRotation = false;
	bd.angularDamping = 0.1;
	bd.linearDamping = 0.01;
	
	mBody = mWorld->CreateBody(&bd);
	mBody->SetSleepingAllowed(true);
	
	b2PolygonShape bShape;
	bShape.SetAsBox(0.75, 0.025);
	
	b2FixtureDef fd;
	
	fd.shape = &bShape;
	fd.density = 0.1;
	fd.friction = 1.0;
	fd.restitution = 0.4;
	
	mBody->CreateFixture(&fd);
	
	b2PolygonShape hShape;
	hShape.SetAsBox(0.05, 0.05, b2Vec2(0.75,0), 0);
	
	mBody->CreateFixture(&hShape, 0.15);
	
	ud = new userData();
	ud->owner = owner;
	ud->objType = 1;
	mBody->SetUserData(ud);
	
	
}

void arrow::update(){
	
	mPos = mBody->GetPosition();
	angle = mBody->GetAngle();
	
}

void arrow::fire(float amp, float pitch){
	
	int x_dir = (owner == 0)? 1 :-1;
	mBody->SetTransform(mBody->GetPosition(), b2_pi * owner + pitch * x_dir);
	mBody->ApplyForce(b2Vec2(8.0 * x_dir * amp ,mBody->GetMass() * 9.8f * 100.0f * pitch), mPos);
	
}


void arrow::draw(){
	
	ofSetColor(0);
	ofFill();
	
	glPushMatrix();
		glTranslatef(mPos.x, mPos.y, 0);
		glRotatef(ofRadToDeg(angle), 0, 0, 1);

			ofRect(0, 0, 1.5, 0.05);
			
			ofEnableAlphaBlending();
			ofSetColor(255,255);
			img.draw(0, 0, 1.5, 0.225);
			ofDisableAlphaBlending();

	glPopMatrix();
	
}



arrow::~arrow(){


}