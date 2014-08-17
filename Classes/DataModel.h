#pragma once
#include "cocos2d.h"
#include "WayPoint.h"
#include "Creep.h"
#include "TutorialScene.h"

USING_NS_CC;

class DataModel{
public:
	TutorialScene* _gameLayer;
	Vector<WayPoint*> waypoints;
	Vector<Creep*> targets;
	Vector<Wave*> waves;
	static DataModel* getModel();
private:
	DataModel(){};
	static DataModel * m_pInstance;
};