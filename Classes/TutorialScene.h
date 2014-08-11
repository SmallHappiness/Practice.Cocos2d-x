#pragma once
#include "cocos2d.h"

class TutorialScene : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_background;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(TutorialScene);
};