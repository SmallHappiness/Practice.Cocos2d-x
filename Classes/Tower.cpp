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
	this->target = this->getClosestTarget();

	if (this->target != NULL){
		Point shootVector = this->target->getPosition() - this->getPosition();
		float shootAngle = ccpToAngle(shootVector);
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);

		float rotateSpeed = 0.5 / M_PI;
		float rotateDuration = fabs(shootAngle *rotateSpeed);

		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), CallFunc::create(this, callfunc_selector(MachineGunTower::finishFiring)), NULL));
	}
}
void MachineGunTower::finishFiring(){
	DataModel* m = DataModel::getModel();
	if (this->target != NULL && this->target->curHp > 0 && this->target->curHp < 100){
		this->nextProjectile = Projectile::projectile();
		this->nextProjectile->setPosition(this->getPosition());
		
		this->getParent()->addChild(this->nextProjectile, 1);
		m->projectiles.pushBack(this->nextProjectile);

		float delta = 1.0f;
		Vec2 shootVector = -(this->target->getPosition() - this->getPosition());
		Vec2 normalizedShootVector = ccpNormalize(shootVector);
		Vec2 overshotVector = normalizedShootVector * 320;
		Point offscreenPoint = this->getPosition() - overshotVector;

		this->nextProjectile->runAction(Sequence::create(MoveTo::create(delta, offscreenPoint), CallFuncN::create(this, callfuncN_selector(MachineGunTower::creepMoveFinished)), NULL));
		this->nextProjectile->setTag(2);
		this->nextProjectile = NULL;
	}
}

void MachineGunTower::creepMoveFinished(cocos2d::Node* sender){
	DataModel* m = DataModel::getModel();
	Sprite* sprite = (Sprite *)sender;
	this->getParent()->removeChild(sprite, true);
	m->projectiles.eraseObject((Projectile*)sprite);
}