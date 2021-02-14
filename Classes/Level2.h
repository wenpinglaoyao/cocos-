#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;
#define MAX_ENEMY 9 //敌人的最多数量
#define MAX_PARTNER 3 //猪队友的最多数量

class Level2 :public cocos2d::Scene {
public:
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Bullet* _bullet;//英雄的炮弹
	Tank* _partner[MAX_PARTNER];//友军坦克数组
	Tank*  _enemy[MAX_ENEMY];//敌人坦克向量表
	Bullet* _enemyBullet[MAX_ENEMY];//敌人的炮弹
	Vector<Bullet*> _BOOMVector; //爆炸的向量表
	int _taskCount;
	int _GameState;
	float _addCircle;//敌人爆炸半径的增加量

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level2);
	void update(float delta);
	bool circleDetection(Point circlePos, float radius, Point tankPos);//爆炸杀伤的检测函数
};
