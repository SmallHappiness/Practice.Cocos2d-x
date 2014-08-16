#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

HelloWorldHud *HelloWorld::_hud = NULL;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	auto hud = HelloWorldHud::create();
	_hud = hud;

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

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool { return true; };
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_numCollected = 0;


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
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event){
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
	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
		playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
		this->setPlayerPosition(playerPos);

	this->setViewPointCenter(_player->getPosition());
}

cocos2d::Point HelloWorld::tileCoordForPosition(cocos2d::Point position){
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height*_tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
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

	return true;
}

void HelloWorldHud::numCollectedChanged(int numCollected){
	char showStr[20];
	sprintf(showStr, "%d", numCollected);
	label->setString(showStr);
}
