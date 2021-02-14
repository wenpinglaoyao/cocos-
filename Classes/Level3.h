#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;

#define MAXENEMY3 4//敌人的最多数量
#define SNOW_SPEED 1.3//雪崩前进的速度
#define SOUND_SPEED 1.3//声波前进的速度
#define WID 800
#define HEI 800
#define ENEMYBUSP 6//敌人子弹的速度
#define TASKNUM 4
#define ENEMYSPEED 0.9f
#define SNOWDISTANCE 42//雪块之间的距离
#define STONESIZE 78 //石头的尺寸
#define COOLTIME 180

class Level3 :public cocos2d::Scene {
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Bullet* _bullet;//英雄的炮弹
	Tank*  _enemy[4];//敌人坦克向量表
	Vector<Bullet*> _soundVector; //声波的向量表
	Vector<Bullet*> _snowVector; //雪崩的向量表
	Sprite* _stone;
	int _taskCount;
	int _GameState;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level3);
	void update(float delta);
	bool backDetection(Tank* tank,int direction,float speed);//推坦克的检测函数
	bool isInfringeBorder(Tank* tank, int direction,int limit); //乱向的检测越界函数
	void backTank(Tank* tank, int directin, float);
	bool tankColl(Sprite*, Sprite*);
	bool isShoot(Tank*);
	void createSnowLine(int);
};