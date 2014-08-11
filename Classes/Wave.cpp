#include "Wave.h"

USING_NS_CC;

bool Wave::init(){
	if (!Node::init()){
		return false;
	}
	return true;
}
Wave* Wave::initWithCreep(Creep* creep, double spawnRate, int TotalCreeps){
	this->CreepType = creep;
	this->spawnRate = spawnRate;
	this->totalCreeps = TotalCreeps;

	return this;
}