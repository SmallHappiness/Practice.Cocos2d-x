TileGame
========

Pratice for using tilemap.

## Chapter 01
* auto關鍵字要謹慎使用，得考慮有否USING_NS_CC;，否則Point和cocos2d::Point不等價。
* ver2.x的Point在Ver3.x後正名為Vec2。
* C++11支援匿名方法如

`listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool { return true; };`

## Chapter 02
* 利用HUD技術，在畫面顯示要表達的訊息。
* 創建Layer，初始化如create()和init()不可馬虎，Scene->addChild(Layer, XX);的動作也是。

## Chapter 03
* 使用Director來替換Scene，便能重新開始遊戲。
* 使用Mode來切換移動或者射飛鏢模式。儘管並非最佳解，但算可行解。
* 事件的使用仍待熟稔。

`enemy->runAction(Sequence::create(actionMove, actionMoveDone, NULL));`

`projectile->runAction(Sequence::create(MoveTo::create(realMoveDuration, realDest), actionMoveDone, NULL));`

`this->schedule(schedule_selector(HelloWorld::testCollisions));`
