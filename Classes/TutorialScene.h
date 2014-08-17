#pragma once
#include "cocos2d.h"
#include "Creep.h"
#include "WayPoint.h"
#include "Wave.h"

USING_NS_CC;

class TutorialScene : public Layer{
public:
	TMXTiledMap* tileMap;
	TMXLayer* background;
	int currentLevel;

	static Scene* createScene();
	void addWaypoint();
	void addWaves();
	virtual bool init();
	virtual void update(float dt);
	void FollowPath(Node* sender);
	Wave* getCurrentWave();
	Wave* getNextWave();
	void gameLogic(float dt);
	void addTarget();
	cocos2d::Point tileCoordForPosition(cocos2d::Point position);
	bool canBuildOnTilePosition(cocos2d::Point pos);
	void addTower(Point pos);
	Point boundLayerPos(Point newPos);
	CREATE_FUNC(TutorialScene);
};