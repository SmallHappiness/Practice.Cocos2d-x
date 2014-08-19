#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	//member function
	void selectSpriteForTouch(Point touchLocation);
	void panForTranslation(Point translation);
	Point boundLayerPos(Point newPos);

	//variables
	cocos2d::Sprite* background;
	cocos2d::Sprite* selSprite;
	cocos2d::Vector <cocos2d::Sprite*> movableSprites;
};

#endif // __HELLOWORLD_SCENE_H__
