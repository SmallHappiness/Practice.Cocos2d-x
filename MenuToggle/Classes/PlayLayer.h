#pragma once
#include "cocos2d.h"
#include "SceneManager.h"

USING_NS_CC;

class PlayLayer :public Layer{
public:
	PlayLayer();
	~PlayLayer();
	virtual bool init();
	void back(Object* pSender);

	CREATE_FUNC(PlayLayer);
};