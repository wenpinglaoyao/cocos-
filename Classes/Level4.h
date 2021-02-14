#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;

#define MAXE4 6//敌人的最多数量
#define BUSP 2.5f//主角子弹射速
#define WID4 800
#define HEI4 800
#define ESP4 0.5//敌人的速度
#define TASKNUM4 4
#define STONESIZE4 50
#define HEROSPEED 2.0f
#define INTERROGATETIME 300//审问所需的时间

class Level4 :public cocos2d::Scene {
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Bullet* _bullet;//英雄的炮弹
	Tank*  _enemy[MAXE4];//敌人坦克向量表
	Sprite* _stone[4];
	Sprite* _sir;//这个是长官
	int _countInterrogate;//这个是一个长官被抓后，审问开始的计时
	int _taskCount;
	int _GameState;
	int _sirCount;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level4);
	void update(float delta);
	/****下面这个是一个递归函数，用来实现主角推土机的功能，让主角
	可以推着敌人、障碍物一起前进，由于在cocos引擎中没有这种轮子，
	所以这里我们得自己做一个****/
	bool backDetection(Tank* tank, int direction, float speed)
	{
		switch (direction)//首先把坦克按照方向移位一个速度量
		{
		case 1:
			tank->setPositionY(tank->getPositionY() + speed);
			break;
		case 3:
			tank->setPositionY(tank->getPositionY() - speed);
			break;
		case 4:
			tank->setPositionX(tank->getPositionX() - speed);
			break;
		case 2:
			tank->setPositionX(tank->getPositionX() + speed);
			break;
		}
		tank->setCool(1);

		if (this->isInfringeBorder(tank, direction, -tank->getContentSize().width / 2))//如果这个坦克移位后越界了
		{
			backTank(tank, direction, speed);
			return true;
		}
		for (int i = 0;i < 4;i++)
		{
			if (tankColl(tank, _stone[i]))//如果这个坦克撞到巨石了
			{
				backTank(tank, direction, speed);
				return true;
			}
		}


		for (int i = 0;i < MAXE4;i++)
		{
			if (!_enemy[i]->getCool() && tankColl(_enemy[i], tank))//如果该坦克重叠了别的坦克
			{
				if (backDetection(_enemy[i], direction, speed) == true)
				{
					backTank(tank, direction, speed);
					return true;
				}
			}
		}

		return false;
	}
	bool isInfringeBorder(Tank* tank, int direction, int limit)
	{
		switch (direction)
		{
		case 1:
			if (tank->getPositionY() >= HEI4 + limit)
				return true;
		case 3:
			if (tank->getPositionY() <= -limit)
				return true;
			break;
		case 2:
			if (tank->getPositionX() >= WID4 + limit)
				return true;
			break;
		case 4:
			if (tank->getPositionX() <= -limit)
				return true;
			break;
		}
		return false;
	}
	void backTank(Tank* tank, int direction, float speed)
	{
		switch (direction)
		{
		case 1:
			tank->setPositionY(tank->getPositionY() - speed);
			break;
		case 3:
			tank->setPositionY(tank->getPositionY() + speed);
			break;
		case 4:
			tank->setPositionX(tank->getPositionX() + speed);
			break;
		case 2:
			tank->setPositionX(tank->getPositionX() - speed);
			break;
		}
		//tank->setPosition(tank->getPositionX(), tank->getPositionY());
	}
	bool tankColl(Sprite* t1, Sprite* t2)//这一关要自己写个碰撞检测函数，不然有bug
	{
		float wid = t1->getContentSize().width / 2 + t2->getContentSize().width / 2;
		float hei = t1->getContentSize().height / 2 + t2->getContentSize().height / 2;
		wid = wid*wid;
		hei = hei*hei;
		if ((t1->getPositionX() - t2->getPositionX())*(t1->getPositionX() - t2->getPositionX()) <= wid &&
			(t1->getPositionY() - t2->getPositionY())*(t1->getPositionY() - t2->getPositionY()) <= hei)
			return true;
		return false;
	}
};