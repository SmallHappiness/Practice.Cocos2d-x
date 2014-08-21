#pragma once
#include "cocos2d.h"
#include "SceneManager.h"

USING_NS_CC;

class CreditLayer :public Layer{
public:
	CreditLayer();
	~CreditLayer();
	virtual bool init();
	void back(Object* pSender);

	CREATE_FUNC(CreditLayer);
};