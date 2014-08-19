#include "Creep.h"
#include "WayPoint.h"
#include "DataModel.h"

USING_NS_CC;

bool Creep::init(){
	if (!Sprite::init())
		return false;
	return true;
}

Creep* Creep::initWithCreep(Creep* copyFrom){
	return NULL;
}

Creep* FastRedCreep::creep(){
	auto creep = Creep::create();
	creep->sprite = Sprite::create("Enemy1.png");
	creep->addChild(creep->sprite, 0);
	creep->curHp = 10;
	creep->moveDuration = 4;
	creep->curWaypoint = 0;
	return creep;
}

Creep* StrongGreenCreep::creep(){
	auto creep = Creep::create();
	creep->sprite = Sprite::create("Enemy2.png");
	creep->addChild(creep->sprite, 0);
	creep->curHp = 20;
	creep->moveDuration = 8;
	creep->curWaypoint = 0;
	return creep;
}

WayPoint* Creep::getCurrentWaypoint(){
	DataModel* m = DataModel::getModel();
	WayPoint* waypoint = (WayPoint *)m->waypoints.at(this->curWaypoint);
	return waypoint;
}

WayPoint* Creep::getNextWaypoint(){
	DataModel* m = DataModel::getModel();
	int lastWaypoint = (int)m->waypoints.size();
	this->curWaypoint++;
	if (this->curWaypoint > lastWaypoint)
		this->curWaypoint = lastWaypoint - 1;

	WayPoint *waypoint = (WayPoint *)m->waypoints.at(curWaypoint);
	return waypoint;
}

void Creep::creepLogic(float dt){
	WayPoint* waypoint = this->getCurrentWaypoint();

	Point waypointVector = waypoint->getPosition() - this->getPosition();
	float waypointAngle = ccpToAngle(waypointVector);
	float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * waypointAngle);

	float rotateSpeed = 0.5 / M_PI;
	float rotateDuration = fabs(waypointAngle * rotateSpeed);

	this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
}

