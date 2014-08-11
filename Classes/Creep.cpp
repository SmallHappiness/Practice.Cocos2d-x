#include "Creep.h"
#include "WayPoint.h"
#include "DataModel.h"

USING_NS_CC;

bool Creep::init(){
	if (!Sprite::init()){
		return false;
	}
	return true;
}
Creep* Creep::initWithCreep(Creep* copyFrom){
	return NULL;
}
Creep* FastRedCreep::creep(){
	auto creep = Creep::create();
	creep->sprite = Sprite::create("Enemy1.png");
	creep->setScale(0.4);
	creep->addChild(creep->sprite, 0);
	creep->curHp = 10;
	creep->moveDuration = 3;
	creep->curWaypoint = 0;
	return creep;
}

Creep* StrongGreenCreep::creep(){
	auto creep = Creep::create();
	creep->sprite = Sprite::create("Enemy2.png");
	creep->setScale(0.4);
	creep->addChild(creep->sprite, 0);
	creep->curHp = 30;
	creep->moveDuration = 8;
	creep->curWaypoint = 0;
	return creep;
}
WayPoint* Creep::getCurrentWaypoint(){
	DataModel* m = DataModel::getModel();
	WayPoint* waypoint = (WayPoint*)m->waypoints.at(this->curWaypoint);
	return waypoint;
}
WayPoint* Creep::getNextWaypoint(){
	DataModel* m = DataModel::getModel();
	if (this->curWaypoint != 7)
		this->curWaypoint++;
	else
		this->curWaypoint = 0;
	CCLOG("%d", this->curWaypoint);
	WayPoint *waypoint = (WayPoint*)m->waypoints.at(curWaypoint);
	return waypoint;
}