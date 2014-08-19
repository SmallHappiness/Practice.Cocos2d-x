#pragma once
#include "cocos2d.h"
#include "Creep.h"
#include "Projectile.h"

USING_NS_CC;

class Tower: public Sprite {
public:
	cocos2d::Point coord;
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
	void finishFiring(); 
	Projectile* nextProjectile;
	void creepMoveFinished(cocos2d::Node* sender);
};