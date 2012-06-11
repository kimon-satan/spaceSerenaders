#include "testApp.h"


bool isBDestroyed(balloon b){return b.getIsDestroyed();}
bool isADestroyed(arrow a){return a.getIsDestroyed();}


//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofHideCursor();
	
	b2Vec2 gravity(0,-10);
    bool doSleep = true;
	
    mWorld  = new b2World(gravity, doSleep);
	mWorld->SetContactListener(&mCollisionListener);
	
    timeStep = 1.0f/60.0f;
	vel_i = 10;
	pos_i = 10;
	
	mReceiver.setup(12345);
	mSender.setup("localhost", 57120);
	
	
	win_rect.set(0,4.5,8,8);
	test_rect.setFromCenter(win_rect.x, win_rect.y + win_rect.height/2 - win_rect.height/6, win_rect.width * 0.9, win_rect.height/8);
	createFurniture();
	
	for(int i =0; i < 2; i ++){

		vector<balloon> tb;
		mBalloons.push_back(tb);
		vector<arrow> ta;
		mArrows.push_back(ta);
		cBalloon[i] = NULL;
		mPlayerPoints[i] =0;
	}
	
	anchor_size = 0.2;
	knightPos[0].set(-10.5,4.3);
	knightPos[1].set(10,3.8);
	
	scoreFont.loadFont("charlesScript.ttf", 50, true);
	creditFont.loadFont("charlesScript.ttf", 25, true);
	
	gameEllapsed = 0;
	gameRunning = false;
	isDemo = false;
	creditAlpha = 255;
	gameEnded = 181;
	
	knightImg[0].loadImage("blueKnight.png");
	knightImg[1].loadImage("redKnight.png");
	
	balcony.loadImage("balcony.png");
	princess.loadImage("princess.png");
	princess_pos = 0;
	target_pos = 0;
	
	
}

void testApp::createTestBody(){
	
	b2BodyDef testBd;
	testBd.position.Set(0.0f, 0.0f);
	testBd.type = b2_dynamicBody;
	mTestBody = mWorld->CreateBody(&testBd);
	
	b2CircleShape t_shape;
	t_shape.m_radius = 0.5;
	mTestBody->CreateFixture(&t_shape, 1);
	
}

void testApp::createFurniture(){
	
	//create floor 
	
	b2BodyDef groundBd;
	groundBd.position.Set(0.0f, -8.0f);
	groundBd.type = b2_staticBody;
	mGroundBody = mWorld->CreateBody(&groundBd);
	
	b2PolygonShape g_Box;
	g_Box.SetAsBox(14.0f, 0.1f);
	mGroundBody->CreateFixture(&g_Box, 1.0f);
	
	
	//create balcony
	
	b2BodyDef balconyBd;
	
	balconyBd.type = b2_staticBody;
	balconyBd.position.Set(win_rect.x,win_rect.y);
	
	mBalconyBody = mWorld->CreateBody(&balconyBd);
	
	balconyFixt[0].set(-win_rect.width/2.2,1.3,0.1, win_rect.height/2.1);
	balconyFixt[1].set(win_rect.width/2.2,1.3,0.1, win_rect.height/2.1);
	balconyFixt[2].set(0,win_rect.height/2.5, win_rect.width * 0.9,0.1);
	
	
	for(int i = 0; i < 3; i++){
		
		b2PolygonShape b_Box;
		b_Box.SetAsBox(balconyFixt[i].width/2, balconyFixt[i].height/2, b2Vec2(balconyFixt[i].x,balconyFixt[i].y), 0);
		mBalconyBody->CreateFixture(&b_Box, 1.0f);
		
	}
	
	
	
	//create anchors
	
	b2BodyDef anchorBd;
	anchorBd.type = b2_staticBody;
	b2PolygonShape aBox;
	aBox.SetAsBox(anchor_size/2, anchor_size/2);
	
	b2FixtureDef a_fd;
	a_fd.density = 1.0f;
	a_fd.friction = 1.0f;
	a_fd.restitution = 0.2;
	a_fd.shape = &aBox;
	a_fd.filter.maskBits = 0x0002;
	
	b2Vec2 a_positions[2] = {b2Vec2(-8.8,-2.5), b2Vec2(10,-3.6)};
	
	for(int i = 0; i < 2; i++){
		
		anchorBd.position = a_positions[i];
		mAnchorBodies[i] = mWorld->CreateBody(&anchorBd);
		mAnchorBodies[i]->CreateFixture(&a_fd);
		
	}
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(255);
	mWorld->Step(timeStep, vel_i, pos_i);
	mWorld->ClearForces();
	
	updateArrows();
	updateBalloons();
	updatePrincess();
	
	if(mOldChains.size() > 25){mOldChains.begin()->destroy(); mOldChains.erase(mOldChains.begin());}
	
	
	if(gameRunning || isDemo){
		
		gameEllapsed = ofGetElapsedTimef() - gameStart;
		
		if(gameEllapsed > MAX_GAME_TIME)endGame();
		
		
		while (mReceiver.hasWaitingMessages()) {
			
			ofxOscMessage m;
			mReceiver.getNextMessage(&m);
			
			if(m.getAddress() == "/beginSoft"){
				
				int player = m.getArgAsInt32(0);
				beginBalloon(player);
			}
			
			
			if(m.getAddress() == "/crescendo"){
				
				int player = m.getArgAsInt32(0);
				inflateBalloon(player);
				
			}
			
			if(m.getAddress() == "/stable"){
				
				int player = m.getArgAsInt32(0);
				float amp = m.getArgAsFloat(1);
				float peak = m.getArgAsFloat(2);
				shiftBalloon(player, amp, peak);
				
			}
			
			if(m.getAddress() == "/noteOff"){
				
				int player = m.getArgAsInt32(0);
				releaseBalloon(player);
				
			}
			
			if(m.getAddress() == "/beginSharp"){
				
				int player = m.getArgAsInt32(0);
				float amp = m.getArgAsFloat(1);
				float freq = m.getArgAsFloat(2);
				beginArrow(player, amp, freq);
			}
			
		}
	
	
		
	}else{
		
		//clear old messages
		ofxOscMessage m;
		while (mReceiver.hasWaitingMessages())mReceiver.getNextMessage(&m);
		gameEnded += 1;
		if(gameEnded > 180)creditAlpha += 1;
		
	}
	

}


void testApp::startGame(){
	
	gameEllapsed =0;
	gameStart = ofGetElapsedTimef();
	gameRunning = true;
	
	isDemo = false;
	
	for(int i =0; i < 2; i ++){
		
		for(vector<arrow>::iterator it = mArrows[i].begin(); it != mArrows[i].end(); it++)it->destroy();
		for(vector<balloon>::iterator it = mBalloons[i].begin(); it != mBalloons[i].end(); it++)it->destroy();
		
		mArrows[i].clear();
		mBalloons[i].clear();
		
		mPlayerPoints[i] =0;
		cBalloon[i] =NULL;
		
	}
	
	
	princess_pos = 0;
	target_pos = 0;
	
	for(vector<chain>::iterator it = mOldChains.begin(); it != mOldChains.end(); it++)it->destroy();
	mOldChains.clear();
	
	
	
	ofxOscMessage m;
	m.setAddress("/startGame");
	mSender.sendMessage(m);
	
	
	
}

void testApp::endGame(){
	
	gameRunning = false;
	if(target_pos > 0)target_pos = 1;
	if(target_pos < 0)target_pos = -1;
	
	ofxOscMessage m;
	m.setAddress("/endGame");
	mSender.sendMessage(m);
	
	creditAlpha = 0;
	gameEnded = 0;

}

void testApp::beginBalloon(int player){
	
	if(cBalloon[player])releaseBalloon(player);
	
	balloon b;
	b.setOwner(player);
	mBalloons[player].push_back(b);
	mBalloons[player].back().initBody(mWorld, mAnchorBodies[player]);
	cBalloon[player] = &mBalloons[player].back();
}

void testApp::inflateBalloon(int player){
	
	if(cBalloon[player]){
		cBalloon[player]->inflate();
	}
	
}


void testApp::shiftBalloon(int player, float amp, float freq){

	
	if(cBalloon[player]){
		cBalloon[player]->blow(amp, freq);
	}


}


void testApp::releaseBalloon(int player){
	
	if(cBalloon[player]){
		
		cBalloon[player]->release();
		cBalloon[player] = NULL;
	}
	
}

void testApp::beginArrow(int player, float amp, float freq){
	
	arrow a;
	b2Vec2 s_pos = (player == 0) ?b2Vec2(-9,-3) : b2Vec2(9,-3);
	a.setPos(s_pos);
	a.setOwner(player);
	mArrows[player].push_back(a);
	arrow * cArrow = &mArrows[player].back();
	
	amp = pow(amp,0.25f);
	cArrow->initBody(mWorld);
	cArrow->fire(amp, freq);
	
}

void testApp::updateArrows(){
	
	for(int i = 0; i < 2; i ++){
		
		vector<arrow>::iterator it;
		
		for(it = mArrows[i].begin(); it != mArrows[i].end(); it++){
			it->update();
			if(abs(it->getPosition().x) > X_BOUND)it->destroy();
		}
		
		it = remove_if(mArrows[i].begin() , mArrows[i].end(), isADestroyed);
		mArrows[i].erase(it, mArrows[i].end());
		
		if(mArrows[i].size() > 25){
			mArrows[i][0].destroy();
			mArrows[i].erase(mArrows[i].begin());
		}
		
	}
	
}

void testApp::updateBalloons(){
	
	vector< vector<int> > destroyList = mCollisionListener.getDestroyList();
	mCollisionListener.clearDestroyList();
	
	for(int i = 0; i < 2; i ++){
		
		vector<balloon>::iterator it;
		
		for(it = mBalloons[i].begin(); it != mBalloons[i].end(); it++){
			it->update();
			b2Vec2 pos = it->getPosition() + b2Vec2(0,it->getSize());
			
			if(pos.y > Y_BOUND){
				it->destroy();
			}else if(test_rect.inside(pos.x, pos.y)){
				
				if(!it->getIsRecieved()){
					addPoints(i, it->getSize()/MAX_BALLOON);
					
					ofxOscMessage m;
					m.setAddress("/addPoints");
					m.addFloatArg(it->getSize()/MAX_BALLOON);
					mSender.sendMessage(m);
					
					it->slowDestroy();
				}
				
			}else{
				
				for(int j = 0; j < destroyList[i].size(); j++){
					if(destroyList[i][j] == it->getIndex()){
						
						if(!it->getIsRecieved()){
							
							if(it->getIsFixed()){
								it->release();
								cBalloon[i] = NULL;
							}
							mOldChains.push_back(it->pop());
							addPoints((i+1)%2, it->getSize()/MAX_BALLOON);
							ofxOscMessage m;
							m.setAddress("/pop");
							m.addFloatArg(it->getSize()/MAX_BALLOON);
							mSender.sendMessage(m);
						}
						
						break;
					}
				}
				
			}
		}
		
		it = remove_if(mBalloons[i].begin() , mBalloons[i].end(), isBDestroyed);
		mBalloons[i].erase(it, mBalloons[i].end());
		
		if(cBalloon[i])cBalloon[i] = &mBalloons[i].back(); //update the pointer now everything has moved
		
	}
	
}


void testApp::updatePrincess(){
	
	
	if(princess_pos == target_pos){
		
	}else if(princess_pos > target_pos + 0.01){
		princess_pos -= 0.01;
	}else if(princess_pos < target_pos - 0.01){
		princess_pos += 0.01;
	}
	


}


void testApp::addPoints(int player, float points){
	
	if(gameRunning || isDemo){
		
		//points = pow(points,2.0f);
		mPlayerPoints[player] += points;
		mPlayerPoints[player] = min(mPlayerPoints[player],4.0f);
		
		float mul = (player > 0)? 1 : -1;
		float div = (float)gameEllapsed/(float)MAX_GAME_TIME;
		target_pos += mul * points/(5.0f - mPlayerPoints[player]); // moves further as game goes on
	
		if(target_pos > 1){
			target_pos = 1;
			endGame();
		}else if(target_pos < -1){
			target_pos = -1;
			endGame();
		}
		
	}
	

}

//--------------------------------------------------------------
void testApp::draw(){
	
	/*ofSetColor(0);
	ofDrawBitmapString("ellapsed: " + ofToString(gameEllapsed, 2),20,40);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2) , 20,20);
	ofDrawBitmapString("mx: "+ofToString(w_x,2)+ "  my: " + ofToString(w_y,2),20,60 );*/
	
	
	glPushMatrix();
	glTranslatef(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0);
	
	ofSetColor(255);
	
	glPushMatrix();
	glScalef(50.0, -50.0, 0);
	
	
	ofSetColor(0);
	ofLine(-14, -6, 14, -6);
	
	
	b2Vec2 gp = mGroundBody->GetPosition();
	//ofRect(test_rect);
	

	//ofRect(gp.x, gp.y, 28, 0.2);
	
	ofEnableAlphaBlending();
	for(int i =0; i < 2; i ++){
		b2Vec2 ap = mAnchorBodies[i]->GetPosition();
		//ofRect(ap.x, ap.y, anchor_size, anchor_size);
		ofSetColor(255,255);
		glPushMatrix();
		glScalef(1, -1, 1);
		knightImg[i].draw(knightPos[i].x, knightPos[i].y, 7.3 * 0.536, 7.3);
		glPopMatrix();
	}
	
	
	//ofCircle(mTestBody->GetPosition().x, mTestBody->GetPosition().y, 0.5);
	
	ofSetColor(0);
	glPushMatrix();
	glTranslatef(mBalconyBody->GetPosition().x, mBalconyBody->GetPosition().y, 0);
	
	ofSetColor(100);
	ofFill();
	ofRect(0, 0, win_rect.width *0.9, win_rect.height * 0.8);
	ofSetColor(255);
		glPushMatrix();
	
		glScalef(1, -1, 1);
		balcony.draw(0, 0, win_rect.width, win_rect.height);
		princess.draw(princess_pos * win_rect.width/2.5, -0.5, 2.5 , 2.5 * 0.91);
	
		glPopMatrix();
	//ofSetColor(0, 0, 255);
	//for(int i = 0; i < 3; i++)ofRect(balconyFixt[i]);
	glPopMatrix();
	

	

	
	for(int i  =0; i < 2; i++){
		
		for(vector<balloon>::iterator it = mBalloons[i].begin(); it != mBalloons[i].end(); it++){
			it->draw();
		}
		
		for(vector<arrow>::iterator it = mArrows[i].begin(); it != mArrows[i].end(); it++){
			it->draw();
		}
		
		for(vector<chain>::iterator it = mOldChains.begin(); it != mOldChains.end(); it++){
			it->draw();
		}
	}
	
	glPopMatrix();
	if(!gameRunning){
		
		ofSetColor(255,0,0);
		
		scoreFont.drawString("Game Over", -200,100);
		
		
		if(gameEnded > 180){
			ofEnableAlphaBlending();
			ofSetColor(0,0,0,creditAlpha);
			
			creditFont.drawString("Game design \n & coding \n    by \n\nSimon Katan", 300, -300);
			creditFont.drawString("Graphic design \n     by\n\n Sara Gelfgren", -580, -325);
			creditFont.drawString("  Voice-over by\n\nAdam de la Cour", -600, -100);
			
			ofDisableAlphaBlending();
		}
		
	}
	
	glPopMatrix();
	
	ofDisableAlphaBlending();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'S')startGame();
	if(key == 'D')isDemo = !isDemo;
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
	
	
	w_x = x - ofGetScreenWidth()/2;
	w_y = y - ofGetScreenHeight()/2;
	w_x /= 50.0f;
	w_y /= -50.0f;
	
	//mTestBody->SetTransform(b2Vec2(w_x,w_y),0);
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	//inflateBalloon(0);
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	//beginBalloon(0);
	//if(cBalloon[0])shiftBalloon(0, 0.5, 0.5);
	
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	//releaseBalloon(0);
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}