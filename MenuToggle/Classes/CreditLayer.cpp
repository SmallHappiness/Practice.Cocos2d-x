#include "CreditLayer.h"

USING_NS_CC;

CreditLayer::CreditLayer(){

}

CreditLayer::~CreditLayer(){
}

bool CreditLayer::init(){
	if (!BaseLayer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	MenuItemFont *back = MenuItemFont::create("back", CC_CALLBACK_1(CreditLayer::back, this));
	Menu *menu = Menu::create(back, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menu);

	return true;
}

void CreditLayer::back(Object* pSender){
	SceneManager::goMenu();
}
