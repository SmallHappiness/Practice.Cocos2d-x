#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Creep :public Node{
public:
	virtual bool init();
	CREATE_FUNC(Creep);
};