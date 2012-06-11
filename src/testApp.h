#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "balloon.h"
#include "arrow.h"
#include "collisionListener.h"

#define X_BOUND 14
#define Y_BOUND 14
#define MAX_GAME_TIME 300

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
private:
	
	void createFurniture();
	void createTestBody();
	
	void beginBalloon(int player);
	void inflateBalloon(int player);
	void releaseBalloon(int player);
	void shiftBalloon(int player, float amp, float freq);
	void beginArrow(int player, float amp, float freq);
	
	void updateArrows();
	void updateBalloons();
	
	void updatePrincess();
	void addPoints(int player, float points);
	
	void startGame();
	void endGame();
	
	b2World * mWorld;
	b2Body * mTestBody;
	b2Body* mGroundBody, * mAnchorBodies[2], * mBalconyBody;
	float vel_i, pos_i, timeStep;
	ofxOscReceiver mReceiver;
	ofxOscSender mSender;
	
	float anchor_size;
	float balcony_width, balcony_height;
	
	ofRectangle win_rect, test_rect;
	
	vector<vector<balloon> >mBalloons;
	vector<vector<arrow> >mArrows;
	vector<chain> mOldChains;
	
	balloon * cBalloon[2];
	
	collisionListener mCollisionListener;
	
	ofTrueTypeFont scoreFont;
	ofTrueTypeFont creditFont;
	
	ofVec2f knightPos[2];
	ofRectangle balconyFixt[3];
	ofImage knightImg[2], balcony, princess;
	
	float princess_pos, target_pos;
	float mPlayerPoints[2];
	
	int gameEllapsed, gameStart;
	bool gameRunning, isDemo;
	
	float w_x, w_y;
	
	float creditAlpha;
	int gameEnded;
	
		
};
