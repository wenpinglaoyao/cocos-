#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;

#define MAXE5 4//敌人的最多数量
#define ENEMYSP5 1//敌人的速度
#define BUSP5 25.5f//主角子弹射速
#define WID5 1000
#define HEI5 800
#define EBUSP5 17 //敌人的射速
#define TASKNUM5 24
#define HEROSPEED5 5.0f
#define BARMAX 220

class Level5 :public cocos2d::Scene {
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Bullet* _bullet;//英雄的炮弹
	Sprite* _bar;
	Tank*  _enemy[MAXE5];//敌人坦克
	Vector<Bullet*> _enemyBulletVector;
	Vector<Sprite*> _iceVector;
	int _taskCount;
	int _GameState;
	int _barValue;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level5);
	void update(float delta);
};
