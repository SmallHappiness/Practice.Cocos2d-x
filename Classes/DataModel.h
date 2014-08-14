#pragma once
#include "cocos2d.h"
#include "WayPoint.h"
#include "Creep.h"
#include "Wave.h"
#include "TutorialScene.h"
#include "Tower.h"
#include "GameHUD.h"

USING_NS_CC;

class DataModel{
public:
	TutorialScene* _gameLayer;
	static DataModel* getModel();
	Vector <WayPoint*> waypoints;
	Vector <Creep*> targets;
	Vector <Wave*> waves;
	Vector<Tower*> towers;
	GameHUD* _gameHUDLayer;
private:
	DataModel(){};
	static DataModel* m_pInstance;
};
