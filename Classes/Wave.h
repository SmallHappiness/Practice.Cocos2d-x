#pragma  once
#include "cocos2d.h"
#include "Creep.h"

USING_NS_CC;

class Wave :public Node{
public:
	Point position;
	int totalCreeps;
	double spawnRate;
	Creep* creepType;
	virtual bool init();
	Wave* initWithCreep(Creep* creep, double SpawnRate,int TotalCreeps);
	CREATE_FUNC(Wave);
};