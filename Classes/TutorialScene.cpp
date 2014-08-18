#include "TutorialScene.h"
#include "DataModel.h"
#include "GameHUD.h"

USING_NS_CC;

Scene* TutorialScene::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TutorialScene::create();
	auto myGameHUD = GameHUD::shareHUD();

	// add layer as a child to scene
	scene->addChild(layer, 1);

	scene->addChild(myGameHUD, 1);

	DataModel* m = DataModel::getModel();
	m->_gameLayer = layer;
	m->_gameHUDLayer = myGameHUD;

	return scene;
}

void TutorialScene::FollowPath(Node* sender){
	Creep *creep = (Creep *)sender;

	WayPoint * waypoint = creep->getNextWaypoint();

	int moveDuration = creep->moveDuration;
	auto actionMove = MoveTo::create(moveDuration, waypoint->getPosition());
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(TutorialScene::FollowPath));
	creep->stopAllActions();
	creep->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

bool TutorialScene::init(){
	if (!Layer::init())
		return false;

	this->tileMap = TMXTiledMap::create("TileMap.tmx");
	this->background = tileMap->layerNamed("Background");
	this->background->setAnchorPoint(ccp(0, 0));
	this->addChild(tileMap, 0);

	this->addWaypoint();
	this->addWaves();

	this->scheduleUpdate();
	this->schedule(schedule_selector(TutorialScene::gameLogic), 1.0f);
	this->currentLevel = 0;

	return true;
}

void TutorialScene::addWaves(){
	DataModel *m = DataModel::getModel();

	Wave *wave = NULL;
	wave = Wave::create()->initWithCreep(FastRedCreep::creep(), 0.3, 50);
	m->waves.pushBack(wave);
	wave = NULL;
	wave = Wave::create()->initWithCreep(StrongGreenCreep::creep(), 1.0, 5);
	m->waves.pushBack(wave);
	wave = NULL;
}

Wave* TutorialScene::getCurrentWave(){
	DataModel *m = DataModel::getModel();
	Wave * wave = (Wave *)m->waves.at(this->currentLevel);

	return wave;
}

Wave* TutorialScene::getNextWave(){
	DataModel *m = DataModel::getModel();

	this->currentLevel++;

	if (this->currentLevel > 1)
		this->currentLevel = 0;

	Wave * wave = (Wave *)m->waves.at(this->currentLevel);

	return wave;
}

void TutorialScene::addWaypoint(){
	DataModel *m = DataModel::getModel();
	auto *objects = this->tileMap->objectGroupNamed("Objects");
	WayPoint *wp = NULL;
	std::string stringWithFormat = "Waypoint";
	int wayPointCounter = 0;
	ValueMap wayPoint;
	wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));
	while (wayPoint.begin() != wayPoint.end())	{
		int x = wayPoint.at("x").asInt();
		int y = wayPoint.at("y").asInt();
		wp = WayPoint::create();
		wp->setPosition(ccp(x, y));
		m->waypoints.pushBack(wp);
		wayPointCounter++;
		wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));
	}
	wp = NULL;
}

void TutorialScene::addTarget(){
	DataModel *m = DataModel::getModel();
	Wave* wave = this->getCurrentWave();
	if (wave->totalCreeps < 0)
		return;
	wave->totalCreeps--;

	Creep *target = NULL;
	int random = CCRANDOM_0_1() * 2;
	if (random == 0)
		target = FastRedCreep::creep();
	else
		target = StrongGreenCreep::creep();

	WayPoint *waypoint = target->getCurrentWaypoint();
	target->setPosition(waypoint->getPosition());
	waypoint = target->getNextWaypoint();
	this->addChild(target, 1);

	int moveDuration = target->moveDuration;
	auto actionMove = CCMoveTo::create(moveDuration, waypoint->getPosition());
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(TutorialScene::FollowPath));
	target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
	target->setTag(1);
	m->targets.pushBack(target);
}

void TutorialScene::gameLogic(float dt){
	DataModel *m = DataModel::getModel();
	Wave * wave = this->getCurrentWave();
	static double lastTimeTargetAdded = 0;

	double now = 0;
	if (lastTimeTargetAdded == 0 || now - lastTimeTargetAdded >= wave->spawnRate){
		this->addTarget();
		lastTimeTargetAdded = now;
	}
}

void TutorialScene::update(float dt){
	DataModel *m = DataModel::getModel();
	Vector <Projectile*> projectilesToDelete;

	for each(Projectile *projectile in m->projectiles){
		Rect projectileRect = Rect(projectile->getPositionX() - (projectile->getContentSize().width / 2),
			projectile->getPositionY() - (projectile->getContentSize().height / 2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		Vector <Creep*> targetsToDelete;

		for each(Creep *target in m->targets){
			Rect targetRect = Rect(target->getPositionX() - (target->sprite->getContentSize().width / 2),
				target->getPositionY() - (target->sprite->getContentSize().height / 2),
				target->sprite->getContentSize().width,
				target->sprite->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect)){
				projectilesToDelete.pushBack(projectile);

				Creep *creep = target;
				creep->curHp -= 1;

				if (creep->curHp <= 0)
					targetsToDelete.pushBack(creep);
				break;
			}
		}

		for each(Creep *target in targetsToDelete){
			m->targets.eraseObject(target);
			this->removeChild(target, true);
		}
	}

	for each(Projectile *projectile in projectilesToDelete){
		m->projectiles.eraseObject(projectile);
		this->removeChild(projectile, true);
	}
}

cocos2d::Point TutorialScene::tileCoordForPosition(cocos2d::Point position){
	int x = position.x / this->tileMap->getTileSize().width;
	int y = (this->tileMap->getMapSize().height *this->tileMap->getTileSize().height - position.y) / this->tileMap->getTileSize().height;
	return ccp(x, y);
}

void TutorialScene::addTower(Point pos){
	DataModel *m = DataModel::getModel();
	Tower *target = NULL;
	Point towerLoc = this->tileCoordForPosition(pos);

	int tileGid = this->background->tileGIDAt(towerLoc);
	Value props = this->tileMap->propertiesForGID(tileGid);
	bool buildable = !props.isNull();
	if (!props.isNull()){
		ValueMap map = props.asValueMap();

		int type_int = map.at("buildable").asInt();
		if (1 == type_int){
			target = MachineGunTower::tower();
			target->setPosition(ccp((towerLoc.x * 32) + 16, this->tileMap->getContentSize().height - (towerLoc.y * 32) - 16));
			target->coord = towerLoc;
			this->addChild(target, 1);
			target->setTag(1);
			m->towers.pushBack(target);
		}
		else
			buildable = false;
	}
	if (!buildable) log("Tile Not Buildable");
}

bool TutorialScene::canBuildOnTilePosition(Point pos){
	Point towerLoc = this->tileCoordForPosition(pos);
	int tileGid = this->background->getTileGIDAt(towerLoc);
	DataModel* m = DataModel::getModel();

	for each(Tower *tower in m->towers){
		if (tower->coord == towerLoc)
			return false;
	}
	Value props = this->tileMap->getPropertiesForGID(tileGid);
	if (!props.isNull()){
		ValueMap map = props.asValueMap();
		int type_int = 0;
		if (map.size() == 0)
			type_int = 0;
		else
			type_int = map.at("buildable").asInt();
		if (1 == type_int)
			return true;
	}

	return false;
}

Point TutorialScene::boundLayerPos(Point newPos){
	Size winSize = CCDirector::getInstance()->getWinSize();
	Point retval = newPos;
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, tileMap->getContentSize().width + winSize.width);
	retval.y = MIN(0, retval.y);
	retval.y = MAX(tileMap->getContentSize().height + winSize.height, retval.y);
	return retval;
}