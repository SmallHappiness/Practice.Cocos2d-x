#pragma once
#include "cocos2d.h"
#include "Creep.h"

USING_NS_CC;

class Wave :public Node{
public:
	Point position;
	int totalCreeps;
	double spawnRate;
	Creep* CreepType;

	virtual bool init();
	Wave* initWithCreep(Creep* creep, double spawnRate, int TotalCreeps);
	CREATE_FUNC(Wave);
};