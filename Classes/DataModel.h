#pragma once
#include "cocos2d.h"
#include "WayPoint.h"
#include "Creep.h"
#include "Tower.h"
#include "GameHUD.h"
#include "TutorialScene.h"

USING_NS_CC;

class DataModel{
public:
	TutorialScene* _gameLayer;
	GameHUD* _gameHUDLayer;
	Vector<WayPoint*> waypoints;
	Vector<Creep*> targets;
	Vector<Wave*> waves;
	Vector<Tower*> towers;
	static DataModel* getModel();
private:
	DataModel(){};
	static DataModel * m_pInstance;
};