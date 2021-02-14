#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;

#define ROTATIME 5.0 //这个是激光旋转一圈的时间
#define ROTASPADD 0.9 //这个是激光转圈的加速度（在一个敌人身上会越转越快，换敌人后重新开始，用的是乘法）
#define WID7 1000
#define HEI7 800
#define TASKNUM7 7 //需要完成的任务量
#define HEROSPEED7 1.9f//主角的移速（完工了可以改成1.9f，现在是降低难度，方便测试这一关）
#define BUSP7 2//主角子弹的射速
#define PI 3.1415126
#define MAXDIS 750//新生假身敌人的最远距离
#define OFFSET 100//适当恩赐的最大缩短量距离
#define ROTAADD 1.00035 //激光旋转的加速度（完工了可以把4改成5）


class Level7 :public cocos2d::Scene {
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Bullet* _bullet;//英雄的炮弹
	Sprite* _layer; //激光
	double _rotaTime; //激光旋转一圈所需要的时间
	double _rotaAdd; //激光每次旋转需要增加的幅度量
	bool _isStopRota; //激光是否旋转了一圈的标志
	Sprite* _realEnemy;//真的敌人
	Sprite* _falseEnemy;//假身敌人
	Sprite* sss;
	Text* ext;
	int _taskCount;
	int _GameState;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level7);
	void update(float delta);
	void setFlasePos();
	bool isHurt(float, float);
	float transAngle(Vec2, Vec2);
};
