#pragma once
#include "cocos2d.h"
#include "WayPoint.h"
#include "Creep.h"
#include "Wave.h"
#include "TutorialScene.h"

USING_NS_CC;

class DataModel{
public:
	TutorialScene* _gameLayer;
	static DataModel* getModel();
	Vector <WayPoint*> waypoints;
	Vector <Creep*> targets;
	Vector <Wave*> waves;
private:
	DataModel(){};
	static DataModel* m_pInstance;
};