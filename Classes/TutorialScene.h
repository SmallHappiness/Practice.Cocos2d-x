#pragma once
#include "cocos2d.h"
#include "Creep.h"
#include "WayPoint.h"
#include "Wave.h"
#include "GameHUD.h"

class TutorialScene : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_background;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	int currentLevel;
	void addWayPoint();
	void addWaves();
	void FollowPath(Node *sender);
	void gameLogic(float dt);
	void addTarget();
	virtual void update(float dt);
	Wave* getCurrentWave();
	Wave* getNextWave();
	void addTower(Point pos);
	Point tileCoordForPosition(Point position);
	bool canBuildOnTilePosition(Point pos);
	Point boundLayerPos(Point newPos);
	Point position;
	GameHUD *gameHUD;

	static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(TutorialScene);
};
