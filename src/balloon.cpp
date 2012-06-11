/*
 *  balloon.cpp
 *  ampGame
 *
 *  Created by Simon Katan on 28/05/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "balloon.h"

int balloon::index = 0;

b2Vec2 balloon::lCircPos[5] = {b2Vec2(-0.25,0.15), b2Vec2(-0.24,0.21),b2Vec2(-0.23,0.18), b2Vec2(-0.25,0.14), b2Vec2(-0.25,0.14)};
b2Vec2 balloon::rCircPos[5] = {b2Vec2(0.23,0.21), b2Vec2(0.25,0.23),b2Vec2(0.25,0.08), b2Vec2(0.25,0.25), b2Vec2(0.25,0.25)};

float balloon::lCircRad[5] = {0.25,0.23,0.25,0.25,0.25};
float balloon::rCircRad[5] = {0.25,0.25,0.25,0.25,0.25};

b2Vec2 balloon::anchorPoint[5] = {b2Vec2(0.1,-0.45), b2Vec2(0.04,-0.41),b2Vec2(-0.17,-0.47), b2Vec2(0.13,-0.45), b2Vec2(0.17,-0.45)};
b2Vec2 balloon::lTPoint[5] = {b2Vec2(-0.33,-0.14), b2Vec2(-0.35,-0.04),b2Vec2(-0.47,0), b2Vec2(-0.33,-0.14), b2Vec2(-0.33,-0.14)};
b2Vec2 balloon::rTPoint[5] = {b2Vec2(0.44,0), b2Vec2(0.39,-0.04),b2Vec2(0.36,-0.22), b2Vec2(0.45,0.05), b2Vec2(0.45,0.05)};

balloon::balloon(){

	mSize = 0.2f;
	isFixed = true;
	isDestroyed = false;
	mBody = NULL;
	ud = NULL;
	alpha =255;
	isRecieved = false;
}


void balloon::initBody(b2World * t, b2Body * anchor){
	
	index += 1;
	mIndex = index;
	
	mWorld = t;
	
	string imgName = (owner == 0)? "blue" : "red";
	imgNum = ofRandom(0,5);
	imgName = imgName + ofToString(imgNum + 1,0) + ".png";
	img.loadImage(imgName);
	
	mChain.initBody(t, anchor, mIndex);
	
	
	mBody = createBody();
	createFixtures();
	mJoint = createJoint();
	
	ud = new userData();
	ud->owner = owner;
	ud->objType = 0;
	ud->objIndex = mIndex;
	mBody->SetUserData(ud);
	
	mPos = mBody->GetPosition();
	angle = mBody->GetAngle();
	
	
	
}




b2Body * balloon::createBody(){
	
	b2Vec2 c_pos = mChain.getLastLink()->GetPosition();
	
	ofVec2f vec;
	
	vec =  ofVec2f(-anchorPoint[imgNum].x, -anchorPoint[imgNum].y);
	vec.normalize();
	vec = vec * (mSize - 0.1);
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	
	bd.position = c_pos + b2Vec2(vec.x, vec.y);
	bd.angle = angle;
	bd.fixedRotation = false;
	bd.angularDamping = 0.1;
	bd.linearDamping = 0.01;
	
	b2Body * b = mWorld->CreateBody(&bd);
	b->SetSleepingAllowed(true);
	
	return b;

}

void balloon::createFixtures(){
	
	b2FixtureDef fd;
	
	fd.density = 0.05;
	fd.friction = 0.2;
	fd.restitution = 0.3;
	fd.filter.groupIndex = -mIndex;
	
	b2CircleShape lShape;
	lShape.m_radius = lCircRad[imgNum] * mSize * 2;
	lShape.m_p = utils::mul(lCircPos[imgNum],mSize * 2);
	
	fd.shape = &lShape;
	
 	mFixtures[0] = mBody->CreateFixture(&fd);
	
	b2CircleShape rShape;
	rShape.m_radius = rCircRad[imgNum] * mSize * 2;
	rShape.m_p =  utils::mul(rCircPos[imgNum],mSize * 2);
	
	fd.shape = &rShape;
	
 	mFixtures[1] = mBody->CreateFixture(&fd);
	
	b2PolygonShape triShape;
	const b2Vec2 verts[3] = {utils::mul(anchorPoint[imgNum],mSize *2), utils::mul(rTPoint[imgNum],mSize*2), utils::mul(lTPoint[imgNum],mSize*2)};
	triShape.Set(&verts[0], 3);
	
	fd.shape = &triShape;
	fd.density = 0.3;
	
	
	mFixtures[2] = mBody->CreateFixture(&fd);
	
	
}


b2RevoluteJoint * balloon::createJoint(){
	
	mAnchor = mChain.getLastLink();
	b2RevoluteJointDef dj;
	dj.Initialize(mBody, mAnchor, mAnchor->GetPosition());
	dj.upperAngle = 0.1 * b2_pi;
	dj.upperAngle = -0.1 * b2_pi;
	dj.enableLimit = true;
	b2RevoluteJoint * joint = (b2RevoluteJoint*)mWorld->CreateJoint(&dj);
	return joint;
	
}

void balloon::update(){
	
	if(mBody && !isRecieved){
		
		mPos = mBody->GetPosition();
		angle = mBody->GetAngle();
		int windDir = (owner == 0) ? 1 : -1;
		float r = (float)(MAX_BALLOON - mSize)/(float)MAX_BALLOON;
		float v = mSize/(float)MAX_BALLOON;
		float up_speed = 1.05 + 0.15 * r;
		
		if(isFixed){
			
			mBody->ApplyForce(b2Vec2(0,(mBody->GetMass() + mChain.getMass()) * 9.8f), mPos);
			
		}else{
			
			mBody->ApplyForce(b2Vec2(mBody->GetMass() * windDir,(mBody->GetMass() + mChain.getMass()) * 9.8f * up_speed), mPos);
			
		}
		
		
	}else if(isRecieved){
		
		alpha *= 0.9;
		if(alpha < 1)destroy();
	
	}
	
	
	
}

void balloon::inflate(){
	
	mSize += 0.01f;
	mSize = min(mSize, (float)MAX_BALLOON);
	
	b2Vec2 c_pos  = mJoint->GetAnchorA();
	
	ofVec2f vec;
	
	vec =  ofVec2f(mPos.x, mPos.y) - ofVec2f(c_pos.x, c_pos.y);
	vec.normalize();
	vec *= (mSize - 0.1);
	mPos = c_pos + b2Vec2(vec.x,vec.y); 
	
	//mPos =  mAnchor->GetPosition();
	mBody->SetTransform(mPos, angle);
	
	mWorld->DestroyJoint(mJoint);
	for(int i = 0; i <3; i ++)mBody->DestroyFixture(mFixtures[i]);
	
	createFixtures();	
	mJoint = createJoint();
	
	
	
}

void balloon::release(){

	isFixed = false;
	mChain.release();
	
}

chain balloon::pop(){
	
	mWorld->DestroyJoint(mJoint);
	baseWeapon::destroy();
	return mChain;
}


void balloon::blow(float amp, float peak){
	
	float a = sin(ofDegToRad(ofGetFrameNum()%360)) * 0.4 * b2_pi;
	float r = (float)mSize/(float)MAX_BALLOON;
	ofVec2f v(0,mBody->GetMass() * 0.5);
	v.rotateRad(a);
	
	mBody->ApplyForce(b2Vec2(v.x,v.y),mPos);
}

void balloon::draw(){
	
	mChain.draw();
	
	glPushMatrix();
		
	glTranslatef(mPos.x, mPos.y, 0);
	glRotatef(ofRadToDeg(angle), 0, 0, 1);
	
	glPushMatrix();
	glScalef(1, -1, 1);
	ofEnableAlphaBlending();
	ofSetColor(255, alpha);
	img.draw(0, 0, mSize*2, mSize*2);
	ofDisableAlphaBlending();
	glPopMatrix();
	
		/*glPushMatrix();
		glScalef(mSize*2, mSize*2, 1);
		ofSetColor(0, 0, 255);
		ofNoFill();
		ofCircle(lCircPos[imgNum].x, lCircPos[imgNum].y, lCircRad[imgNum]);
		ofCircle(rCircPos[imgNum].x, rCircPos[imgNum].y, rCircRad[imgNum]);
		ofFill();
		ofSetColor(0, 255, 0);
		ofRect(anchorPoint[imgNum].x, anchorPoint[imgNum].y, 0.1, 0.1);
		ofNoFill();
		ofBeginShape();
		ofVertex(anchorPoint[imgNum].x, anchorPoint[imgNum].y);
		ofVertex(lTPoint[imgNum].x, lTPoint[imgNum].y);
		ofVertex(rTPoint[imgNum].x, rTPoint[imgNum].y);
		ofVertex(anchorPoint[imgNum].x, anchorPoint[imgNum].y);
		ofEndShape(true);
		glPopMatrix();*/
	
	glPopMatrix();
	

	
	
	
}


void balloon::slowDestroy(){
	
	isRecieved = true;
	mChain.destroy();
	
}

void balloon::destroy(){
	
	mChain.destroy();
	baseWeapon::destroy();


}




float balloon::getSize(){
	
	return mSize;

}

bool balloon::getIsRecieved(){return isRecieved;}

bool balloon::getIsFixed(){return isFixed;}

balloon::~balloon(){
	


}