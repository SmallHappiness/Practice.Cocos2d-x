#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class MenuLayer :public BaseLayer{
public:
	virtual bool init();
	void onNewGame(Object* pSender);
	void onCredits(Object* pSender);

	CREATE_FUNC(MenuLayer);
};