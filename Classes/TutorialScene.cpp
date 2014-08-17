#include "TutorialScene.h"
#include "DataModel.h"

USING_NS_CC;

Scene* TutorialScene::createScene(){
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TutorialScene::create();

	// add layer as a child to scene
	scene->addChild(layer, 1);

	DataModel* m = DataModel::getModel();
	m->_gameLayer = layer;

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
	target->tag = 1;
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

}