#pragma once
#include "cocos2d.h"
#include "GameSprite.h"
#define GOAL_WIDTH 400

class GameLayer : public cocos2d::Layer{
	GameSprite *_player1, *_player2, *_ball;
	Array* _players;
	LabelTTF* _player1ScoreLabel, *_player2ScoreLabel;
	Size _screenSize;
	int _player1Score, _player2Score;
	void playerScore(int player);

public:
	static Scene* createScene();
	~GameLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(GameLayer);
	virtual void onTouchesBegan(Set* pTouches, Event* event);
	virtual void onTouchesMoved(Set* pTouches, Event* event);
	virtual void onTouchesEnded(Set* pTouches, Event* event);
	void update(float dt);
};
