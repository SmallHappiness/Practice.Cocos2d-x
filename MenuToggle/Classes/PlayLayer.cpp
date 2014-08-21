#include "PlayLayer.h"

USING_NS_CC;

PlayLayer::PlayLayer(){

}

PlayLayer::~PlayLayer(){
}

bool PlayLayer::init(){
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	MenuItemFont *back = MenuItemFont::create("back", CC_CALLBACK_1(PlayLayer::back, this));
	Menu *menu = Menu::create(back, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menu);

	return true;
}

void PlayLayer::back(Object* pSender){
	SceneManager::goMenu();
}
