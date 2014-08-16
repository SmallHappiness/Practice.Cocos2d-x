#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

USING_NS_CC;
using namespace CocosDenshion;

HelloWorldHud *HelloWorld::_hud = NULL;

Scene* HelloWorld::createScene(){
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	auto hud = HelloWorldHud::create();
	_hud = hud;
	_hud->setGameLayer(layer);

    // add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(){
	if (!Layer::init())
		return false;

	SimpleAudioEngine::getInstance()->preloadEffect("pickup.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("hit.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("move.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("TileMap.mp3", true);

	std::string file = "TileMap.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	_tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
	_background = _tileMap->getLayer("Background");
	_foreground = _tileMap->getLayer("Foreground");
	_meta = _tileMap->getLayer("Meta");
	_meta->setVisible(false);

	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Objects' object group not found");
	auto spawnPoint = objects->getObject("SpawnPoint");
	CCASSERT(!spawnPoint.empty(), "SpawnPoint object not found");
	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	_player = Sprite::create("Player.png");
	_player->setPosition(x, y);
	addChild(_player);

	setViewPointCenter(_player->getPosition());

	addChild(_tileMap, -1);

	for (auto& eSpawnPoint : objects->getObjects()){
		ValueMap& dict = eSpawnPoint.asValueMap();
		if (dict["Enemy"].asInt() == 1){
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->addEnemyAtPos(Point(x, y));
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool { return true; };
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_numCollected = 0;
	_mode = 0;
	
	this->schedule(schedule_selector(HelloWorld::testCollisions));
	return true;
}

void HelloWorld::setViewPointCenter(Point position){
	auto winSize = Director::getInstance()->getWinSize();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);

	x = MIN(x, (this->_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (this->_tileMap->getMapSize().height * this->_tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}
void HelloWorld::setPlayerPosition(Point position){
	Point tileCoord = this->tileCoordForPosition(position);
	int tileGid = _meta->getTileGIDAt(tileCoord);
	if (tileGid){
		auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
		if (!properties.empty()){
			auto collision = properties["Collidable"].asString();
			auto collectable = properties["Collectable"].asString();
			if ("True" == collision){
				SimpleAudioEngine::getInstance()->playEffect("hit.mp3");
				return;
			}
			else if ("True" == collectable){
				SimpleAudioEngine::getInstance()->playEffect("pickup.mp3");
				_meta->removeTileAt(tileCoord);
				_foreground->removeTileAt(tileCoord);

				this->_numCollected++;
				this->_hud->numCollectedChanged(_numCollected);
			}
		}
	}
	SimpleAudioEngine::getInstance()->playEffect("move.mp3");
	_player->setPosition(position);
	if (_numCollected == 3) 
		this->win();
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event){
	if (_mode == 0){
		auto touchLocation = touch->getLocation();
		touchLocation = this->convertToNodeSpace(touchLocation);

		auto playerPos = _player->getPosition();
		auto diff = touchLocation - playerPos;
		if (abs(diff.x) > abs(diff.y)) {
			if (diff.x > 0)
				playerPos.x += _tileMap->getTileSize().width;
			else
				playerPos.x -= _tileMap->getTileSize().width;
		}
		else {
			if (diff.y > 0)
				playerPos.y += _tileMap->getTileSize().height;
			else
				playerPos.y -= _tileMap->getTileSize().height;
		}
		if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) && playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) && playerPos.y >= 0 && playerPos.x >= 0)
			this->setPlayerPosition(playerPos);

		this->setViewPointCenter(_player->getPosition());
	}
	else{
		auto touchLocation = touch->getLocation();
		touchLocation = this->convertToNodeSpace(touchLocation);

		auto projectile = Sprite::create("Projectile.png");
		projectile->setPosition(_player->getPosition());
		this->addChild(projectile);

		int realX;
		auto diff = touchLocation - _player->getPosition();
		if (diff.x > 0)
			realX = (_tileMap->getMapSize().width*_tileMap->getTileSize().width) + (projectile->getContentSize().width / 2);
		else
			realX = -(_tileMap->getMapSize().width*_tileMap->getTileSize().width) - (projectile->getContentSize().width / 2);
		float ratio = (float)diff.y / (float)diff.x;
		int realY = (realX - projectile->getPosition().x)*ratio + projectile->getPosition().y;
		auto realDest = Point(realX, realY);

		int offRealX = realX - projectile->getPosition().x;
		int offRealY = realY - projectile->getPosition().y;
		float length = sqrtf(offRealX*offRealX + offRealY*offRealY);
		float velocity = 480 / 1.0; // 480 (pixel) / 1 (second)
		float realMoveDuration = length / velocity;

		auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(HelloWorld::projectileMoveFinished, this));
		projectile->runAction(Sequence::create(MoveTo::create(realMoveDuration, realDest), actionMoveDone, NULL));

		_projectiles.pushBack(projectile);
	}
}
void HelloWorld::enemyMoveFinished(Object *pSender){
	Sprite* enemy = (Sprite *)pSender;
	this->animateEnemy(enemy);
}

void HelloWorld::animateEnemy(Sprite *enemy){
	float actualDuration = 0.3f;

	auto diff = ccpSub(_player->getPosition(), enemy->getPosition());
	float angleRadians = atanf((float)diff.y / (float)diff.x);
	float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
	float cocosAngle = -1 * angleDegrees;
	if (diff.x < 0)
		cocosAngle += 180.0;
	enemy->setRotation(cocosAngle);

	auto unitVector = _player->getPosition() - enemy->getPosition(); unitVector.normalize();
	CCLOG("%f %f\n", unitVector.x, unitVector.y);
	auto position = unitVector * 10;
	auto actionMove = MoveBy::create(actualDuration, position);
	auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(HelloWorld::enemyMoveFinished, this));
	enemy->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

Point HelloWorld::tileCoordForPosition(Point position){
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height*_tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
}

void HelloWorld::addEnemyAtPos(Point pos){
	auto enemy = Sprite::create("enemy1.png");
	enemy->setPosition(pos);
	this->addChild(enemy);
	this->animateEnemy(enemy);
	_enemies.pushBack(enemy);
}

void HelloWorld::testCollisions(float dt){
	for (Sprite *target : _enemies){
		auto targetRect = Rect(
			target->getPosition().x - (target->getContentSize().width / 2),
			target->getPosition().y - (target->getContentSize().height / 2),
			target->getContentSize().width,
			target->getContentSize().height);
		if (targetRect.containsPoint(_player->getPosition()))
			this->lose();
	}

	Vector <Sprite*> projectilesToDelete;

	for (Sprite* projectile : _projectiles){
		auto projectileRect = Rect(
			projectile->getPositionX() - projectile->getContentSize().width / 2,
			projectile->getPositionY() - projectile->getContentSize().height / 2,
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		Vector<Sprite*> targetsToDelete;
		for (Sprite *target : _enemies) {
			auto targetRect = Rect(
				target->getPositionX() - target->getContentSize().width / 2,
				target->getPositionY() - target->getContentSize().height / 2,
				target->getContentSize().width,
				target->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect))
				targetsToDelete.pushBack(target);
		}

		for (Sprite *target : targetsToDelete) {
			_enemies.eraseObject(target);
			this->removeChild(target);
		}

		if (targetsToDelete.size() > 0) 
			projectilesToDelete.pushBack(projectile);

		targetsToDelete.clear();
	}
	for (Sprite *projectile : projectilesToDelete) {
		_projectiles.eraseObject(projectile);
		this->removeChild(projectile);
	}
	projectilesToDelete.clear();
}
void HelloWorld::projectileMoveFinished(Object *pSender){
	Sprite *sprite = (Sprite *)pSender;
	this->removeChild(sprite); 
	_projectiles.eraseObject(sprite);
}

void HelloWorld::win(){
	GameOverScene *gameOverScene = GameOverScene::create();
	gameOverScene->getLayer()->getLabel()->setString("You Win!");
	Director::getInstance()->replaceScene(gameOverScene);
}

void HelloWorld::lose(){
	GameOverScene *gameOverScene = GameOverScene::create();
	gameOverScene->getLayer()->getLabel()->setString("You Lose!");
	Director::getInstance()->replaceScene(gameOverScene);
}

void HelloWorld::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorldHud::init(){
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	label = LabelTTF::create("0", "fonts/Marker Felt.ttf", 24.0f, Size(50, 24), TextHAlignment::RIGHT);
	label->setColor(Color3B(0, 0, 0));
	const int MARGIN = 10;
	label->setPosition(visibleSize.width - (label->getDimensions().width / 2) - MARGIN,
		label->getDimensions().height / 2 + MARGIN);
	this->addChild(label);

	MenuItem *on = MenuItemImage::create("projectile-button-on.png", "projectile-button-on.png");
	MenuItem *off = MenuItemImage::create("projectile-button-off.png", "projectile-button-off.png");
	auto toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HelloWorldHud::projectileButtonTapped, this), off, on, NULL);
	auto toggleMenu = Menu::create(toggleItem, NULL);
	toggleMenu->setPosition(on->getContentSize().width * 2, on->getContentSize().height / 2);
	this->addChild(toggleMenu);

	return true;
}

void HelloWorldHud::numCollectedChanged(int numCollected){
	char showStr[20];
	sprintf(showStr, "%d", numCollected);
	label->setString(showStr);
}

//mode 0 = moving mode
//mode 1 = ninja star throwing mode
void HelloWorldHud::projectileButtonTapped(Object *pSender){
	if (_gameLayer->getMode() == 1)
		_gameLayer->setMode(0);
	else
		_gameLayer->setMode(1);
}