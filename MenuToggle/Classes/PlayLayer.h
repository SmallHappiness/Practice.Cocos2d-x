#pragma once
#include "cocos2d.h"
#include "SceneManager.h"
#include "BaseLayer.h"

USING_NS_CC;

class PlayLayer :public BaseLayer{
public:
	PlayLayer();
	~PlayLayer();
	virtual bool init();
	void back(Object* pSender);

	CREATE_FUNC(PlayLayer);
};