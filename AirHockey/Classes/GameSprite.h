#pragma once
#include "cocos2d.h"

USING_NS_CC;

class GameSprite :public Sprite{
public:
	CC_SYNTHESIZE(Point, _nextPosition, NextPosition);
	CC_SYNTHESIZE(Point, _vector, Vector);
	CC_SYNTHESIZE(Touch*, _touch, Touch);

	GameSprite();
	~GameSprite();

	static GameSprite* gameSpriteWithFile(const char* pszFileName);

	virtual void setPosition(const Point &pos);
	inline float radius(){
		return getTexture()->getContentSize().width*0.5f;
	}
};