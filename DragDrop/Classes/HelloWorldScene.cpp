#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(){
	if (!Layer::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();

	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB565); //16bit, no alpha
	background = Sprite::create("blue-shooting-stars.png");
	background->setAnchorPoint(Point(0, 0));
	this->addChild(background);

	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::DEFAULT);

	std::string images[] = { "bird.png", "cat.png", "dog.png", "turtle.png" };
	int images_length = 4;
	for (int i = 0; i < images_length; i++){
		std::string image = images[i];
		Sprite* sprite = Sprite::create(image);
		float offsetFraction = (i + 1.0) / (images_length + 1);
		sprite->setPosition(winSize.width*offsetFraction, winSize.height / 2);
		this->addChild(sprite);

		movableSprites.pushBack(sprite);
	}
	selSprite = NULL;

	//lock event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event){
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	this->selectSpriteForTouch(touchLocation);
	return true;
}
void HelloWorld::onTouchMoved(Touch* touch, Event* event){
	Point touchLocation = this->convertTouchToNodeSpace(touch);

	Point oldTouchLocation = this->convertToNodeSpace(touch->getPreviousLocation());

	Point translation = touchLocation - oldTouchLocation;
	this->panForTranslation(translation);
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event){

}

void HelloWorld::selectSpriteForTouch(Point touchLocation){
	Sprite* newSprite = NULL;
	for (Sprite* sprite : movableSprites){
		if (sprite->getBoundingBox().containsPoint(touchLocation)){
			newSprite = sprite;
			break;
		}
	}

	//select again
	if (newSprite != NULL && newSprite == selSprite)
		return;

	//select other pet
	if (newSprite != selSprite){
		if (selSprite != NULL){
			selSprite->stopAllActions();
			selSprite->runAction(RotateTo::create(0.1f, 0.0f));
		}

		//play animation to know what is selected
		if (newSprite != NULL){
			RotateBy* rotLeft = RotateBy::create(0.1f, -14.0f);
			RotateBy* rotCenter = RotateBy::create(0.1f, 0.0f);
			RotateBy* rotRight = RotateBy::create(0.1f, 14.0f);
			Sequence* rotSeq = Sequence::create(rotLeft, rotCenter, rotRight, rotCenter, NULL);
			newSprite->runAction(RepeatForever::create(rotSeq));
		}
		selSprite = newSprite;
	}
}

Point HelloWorld::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -background->getContentSize().width + winSize.width);

	retval.y = this->getPosition().y;

	return retval;
}

void HelloWorld::panForTranslation(Point translation){
	if (selSprite){
		Point newPos = selSprite->getPosition() + translation;
		selSprite->setPosition(newPos);
	}
	else{
		Point newPos = this->getPosition() + translation;
		this->setPosition(this->boundLayerPos(newPos));
	}
}