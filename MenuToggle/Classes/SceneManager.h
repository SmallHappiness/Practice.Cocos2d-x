#pragma once
#include "cocos2d.h"
#include "MenuLayer.h"
#include "PlayLayer.h"
#include "CreditLayer.h"

USING_NS_CC;

class SceneManager : public Object{
public:
	SceneManager(){};
	~SceneManager(){};

	static void goMenu();
	static void goPlay();
	static void goCredit();

private:
	static void go(Layer* layer);
	static Scene* wrap(Layer* layer);
};