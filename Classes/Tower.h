#pragma once
#include "cocos2d.h"
#include "Creep.h"

class Tower: public Sprite {
public:
	int range;
	Sprite* sprite;
	Creep* target;
	Sprite * selSpriteRange;
	Creep* getClosestTarget();
	CREATE_FUNC(Tower);
};

class MachineGunTower : public Tower {
public:
	static Tower* tower();
	bool virtual init();
	void towerLogic(float dt);
};