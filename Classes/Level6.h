#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;

#define MAXE6 12//敌人的最多数量
#define ENEMYSP6 0.4//敌人的速度
#define BUSP6 25.5f//主角子弹射速
#define WID6 1000
#define HEI6 800
#define EBUSP6 5 //敌人的射速
#define TASKNUM6 12
#define HEROSPEED5 4.0f
#define BARMAX6 200 //走动条的最长长度
#define BALLSIZE 70 //足球的尺寸
#define OFFSETMOVE 0.33f //撞到足球后的小小偏移量，方便玩家微调位置
#define STARMAXSIZE 140 //陨石的最大尺寸
#define STARMINSIZE 80 //陨石的最小尺寸
#define OVERTIME 500 //冻结时间的最长限度，超过了就游戏失败
#define SPADD 0.05 //这个是每死一个敌人后，剩余敌人的加速度量


class Level6 :public cocos2d::Scene {
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Sprite* _aim; //这个是准星，方便玩家对准用的
	Sprite* _aim2;
	Bullet* _bullet;//英雄的炮弹
	Tank*  _enemy[MAXE6];//敌人坦克
	Bullet* _enemyBullet[MAXE6];
	Sprite* _bar;
	Sprite* _star; //陨石精灵
	Sprite* _football; //足球精灵
	float _speedX, _speedY;//足球移动时的速度
	float _starSize;//陨石的实际尺寸
	bool _moveFlag; //陨石是否现世足球是否移动的标记
	int _coolTime; //被冻结的时间统计
	int _taskCount;
	int _GameState;
	float _barValue;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level6);
	void update(float delta);
	void moveByDir()
	{
		switch (_hero->getDirection())
		{
		case 1:
		case 3:
			if (_hero->getPositionX() < _football->getPositionX())
				_hero->setPositionX(_hero->getPositionX() - OFFSETMOVE);
			if (_hero->getPositionX() >= WID6 - 25 || _hero->getPositionX() <= 0 + 25)//如果偏移后越界了
				_hero->setPositionX(_hero->getPositionX() + OFFSETMOVE);//那么拉回来
			for (int i = 0;i < MAXE6;i++)
			{
				if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))
				{
					_hero->setPositionX(_hero->getPositionX() + OFFSETMOVE);//那么拉回来
					break;
				}
			}


			if (_hero->getPositionX() > _football->getPositionX())
				_hero->setPositionX(_hero->getPositionX() + OFFSETMOVE);
			if (_hero->getPositionX() >= WID6 - 25 || _hero->getPositionX() <= 0 + 25)//如果偏移后越界了
				_hero->setPositionX(_hero->getPositionX() - OFFSETMOVE);//那么拉回来
			for (int i = 0;i < MAXE6;i++)
			{
				if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))
				{
					_hero->setPositionX(_hero->getPositionX() - OFFSETMOVE);//那么拉回来
					break;
				}
			}
			break;

		case 2:
		case 4:
			if (_hero->getPositionY() < _football->getPositionY())//先偏移一个小小量
				_hero->setPositionY(_hero->getPositionY() - OFFSETMOVE);
			if(_hero->getPositionY()>=HEI6-25 ||_hero->getPositionY()<=0+25)//如果偏移后越界了
				_hero->setPositionY(_hero->getPositionY() + OFFSETMOVE);//那么拉回来
			for (int i = 0;i < MAXE6;i++)
			{
				if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))
				{
					_hero->setPositionY(_hero->getPositionY() + OFFSETMOVE);//那么拉回来
					break;
				}
			}


			if (_hero->getPositionY() > _football->getPositionY())
				_hero->setPositionY(_hero->getPositionY() + OFFSETMOVE);
			if (_hero->getPositionY() >= HEI6 - 25 || _hero->getPositionY() <= 0 + 25)
				_hero->setPositionY(_hero->getPositionY() - OFFSETMOVE);
			for (int i = 0;i < MAXE6;i++)
			{
				if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))
				{
					_hero->setPositionY(_hero->getPositionY() - OFFSETMOVE);//那么拉回来
					break;
				}
			}
			break;
		}
	}
};
