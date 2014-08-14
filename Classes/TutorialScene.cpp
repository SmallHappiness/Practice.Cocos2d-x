#include "TutorialScene.h"
#include "DataModel.h"
#include "Tower.h"
#include <Vector>
#include <string>

USING_NS_CC;

Scene* TutorialScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = TutorialScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

	auto myGameHUD = GameHUD::shareHUD();
	DataModel *m = DataModel::getModel();
	m->_gameHUDLayer = myGameHUD;

	scene->addChild(myGameHUD, 1);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TutorialScene::init(){
	if (!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	std::string file = "01.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	this->_tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
	this->_background = _tileMap->layerNamed("Background");

	this->addWayPoint();
	this->addWaves();
	this->scheduleUpdate();
	this->schedule(schedule_selector(TutorialScene::gameLogic), 1.0f);
	this->currentLevel = 0;

	addChild(_tileMap, -1);
	return true;
}



void TutorialScene::FollowPath(Node *sender){
	Creep *creep = (Creep *)sender;

	WayPoint *waypoint = creep->getNextWaypoint();

	int moveDuration = creep->moveDuration;
	auto actionMove = MoveTo::create(moveDuration, waypoint->getPosition());
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(TutorialScene::FollowPath));
	creep->stopAllActions();
	creep->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

// Here we read the value from the class ‘creep’. And we make the enemy move. 

void TutorialScene::addWaves()
{
	DataModel *m = DataModel::getModel();

	Wave *wave = NULL;

	wave = Wave::create()->initWithCreep(FastRedCreep::creep(), 0.3, 75);
	m->waves.pushBack(wave);
	wave = NULL;

	wave = Wave::create()->initWithCreep(StrongGreenCreep::creep(), 1.0, 10);
	m->waves.pushBack(wave);
	wave = NULL;
}

// Here, we set the parameters for the class ‘Wave’ about the creep type, spawn rate and the number of the creep.

void TutorialScene::addWayPoint()
{
	DataModel *m = DataModel::getModel();
	auto *objects = this->_tileMap->objectGroupNamed("Objects");
	WayPoint *wp = NULL;

	std::string stringWithFormat = "Waypoint";
	int wayPointCounter = 0;
	ValueMap wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));
	while (wayPoint.begin() != wayPoint.end()){
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
	Wave *wave = this->getCurrentWave();

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
	Wave *wave = this->getCurrentWave();
	static double lastTimeTargetAdded = 0;

	double now = 0;
	if (lastTimeTargetAdded == 0 || now - lastTimeTargetAdded >= wave->spawnRate){
		this->addTarget();
		lastTimeTargetAdded = now;
	}
}

void TutorialScene::update(float dt){

}

Wave *TutorialScene::getCurrentWave(){
	DataModel *m = DataModel::getModel();
	Wave *wave = (Wave *)m->waves.at(this->currentLevel);

	return wave;
}

Wave *TutorialScene::getNextWave(){
	DataModel *m = DataModel::getModel();

	this->currentLevel++;
	if (this->currentLevel > 1)
		this->currentLevel = 0;

	Wave *wave = (Wave *)m->waves.at(this->currentLevel);

	return wave;
}

Point TutorialScene::tileCoordForPosition(Point position){
	int x = position.x / this->_tileMap->getTileSize().width;
	int y = ((this->_tileMap->getMapSize().height * this->_tileMap->getTileSize().height) - position.y) / this->_tileMap->getTileSize().height;
	return ccp(x, y);
}
bool TutorialScene::canBuildOnTilePosition(Point pos)
{
	Point towerLoc = this->tileCoordForPosition(pos);
	int tileGid = this->_background->getTileGIDAt(towerLoc);
	Value props = this->_tileMap->getPropertiesForGID(tileGid);
	ValueMap map = props.asValueMap();
	int type_int;
	if (map.size() == 0)
	{
		type_int = 0;
	}
	else
	{
		type_int = map.at("buildable").asInt();
	}

	if (1 == type_int)
	{
		return true;
	}
	return false;
}


void TutorialScene::addTower(Point pos){
	DataModel *m = DataModel::getModel();

	Tower *target = NULL;
	Point towerLoc = this->tileCoordForPosition(pos);
	int tileGid = this->_background->tileGIDAt(towerLoc);
	Value props = this->_tileMap->propertiesForGID(tileGid);
	ValueMap map = props.asValueMap();

	int type_int = map.at("buildable").asInt();
	if (1 == type_int){
		target = MachineGunTower::tower();
		target->setPosition(ccp((towerLoc.x * 32) + 16, this->_tileMap->getContentSize().height - (towerLoc.y * 32) - 16));
		this->addChild(target, 1);
		target->setTag(1);
		m->towers.pushBack(target);
	}
	else{
		log("Tile Not Buildable");
	}
}

Point TutorialScene::boundLayerPos(Point newPos){
	Size winSize = CCDirector::getInstance()->getWinSize();
	Point retval = newPos;
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, _tileMap->getContentSize().width + winSize.width);
	retval.y = MIN(0, retval.y);
	retval.y = MAX(_tileMap->getContentSize().height + winSize.height, retval.y);
	return retval;
}

void TutorialScene::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
