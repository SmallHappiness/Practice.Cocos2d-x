#include "Wave.h"

USING_NS_CC;

bool Wave::init(){
	if (!Node::init())
		return false;
	return true;
}

Wave* Wave::initWithCreep(Creep* creep, double SpawnRate, int TotalCreeps){
	this->creepType = creep;
	this->spawnRate = SpawnRate;
	this->totalCreeps = TotalCreeps;

	return this;
}

