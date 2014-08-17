#include "Tower.h"
#include "DataModel.h"

Tower* MachineGunTower::tower(){
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("MachineGunTurret.png");
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

Creep* Tower::getClosestTarget(){
	Creep *closestCreep = NULL;
	double maxDistant = 99999;

	DataModel *m = DataModel::getModel();

	for each(Sprite *target in m->targets){
		Creep *creep = (Creep *)target;
		double curDistance = ccpDistance(this->getPosition(), creep->getPosition());

		if (curDistance < maxDistant){
			closestCreep = creep;
			maxDistant = curDistance;
		}
	}
	if (maxDistant < this->range)
		return closestCreep;
	return NULL;
}

void MachineGunTower::towerLogic(float dt){
	
}