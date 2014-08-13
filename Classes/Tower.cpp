#include "Tower.h"
#include "DataModel.h"

Tower* MachineGunTower::tower(){
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("MachineGunTower.png");
	tower->setScale(0.5);
	tower->addChild(tower->sprite, 0);
	tower->range = 200;
	tower->schedule(schedule_selector(towerLogic), 0.2);
	return tower;
}

bool MachineGunTower::init(){
	if (!Sprite::init())
		return false;
	return true;
}

void MachineGunTower::towerLogic(float dt){
	
}