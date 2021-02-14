#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

using namespace ui;
using namespace CocosDenshion;


#define WID8 1000
#define HEI8 800
#define HEROSPEED8 5.0F //主角的移速
#define BOSSSPEED 4//boss的移速
#define BOSSHP 10000  //BOSS的生命值
#define BOSSWID 100//boss的身体宽度
#define HEROHP 100 //主角的血量
#define AGGRESSIVITY 3.0f //英雄的攻击力，与下面boss蓝量恢复值的对比关系，决定了最后较量的轮回数量
#define POWERRESUME 3.0f//BOSS的蓝量恢复值
#define LAYERWID 5//激光的宽度
#define SPARKSIZE 50//激光射中boss时迸溅出的火花的最大尺寸
#define REDUCE 1.7  //火花尺寸的减小幅度，值越大火花消失的越快
#define SOLDERSPEED 5//boss普攻子弹的前进速度
#define NORMALHURT 10//boss一般技能的伤害值
#define ANGLEADD 30//这个决定天女散花的数量，值越低越厉害
#define BOOMNUM 26 //狂轰滥炸的数量
#define BOOMSMALLMAXSIZE 200 //狂轰滥炸炸弹精灵的最大尺寸
#define BOOMSMALLMINSIZE  100 //最小尺寸，与上面那个构成了炸弹的隐形时间，最小尺寸过后，狂轰滥炸炸弹就现行了
#define BOOMSMALLSUB 0.7//这个决定爆炸圈炸弹缩小的速度
#define BOOMRADIUSADD 6.5//狂轰滥炸冲击波半径的增加率（爆炸速度）
#define BOOMMAXSIZE 380 //狂轰滥炸冲击波的最大尺寸
#define METEORSHOWERNUM 38//天外飞仙陨石的数量
#define REDSTONELETHSLITY 0.022//这个是激光对红石（彼岸花雨的爆开石）的杀伤力，值越小玩家的难度越小（迅速反应过来停火）
#define STONERAND  0//这个是天外飞仙陨石中红石出现的概率，值越大出现红石的概率越小
#define SKILLSTARTSPEED 1 //这个代表终极技能的初始速度（在此基础上越来越快！）
#define SPEEDADD 1.00999//终极技能的加速度
#define HPRECOVER 7000//boss回光返照恢复生命值
#define POWERRECOVER 1000//boss回光返照蓝量恢复值

class Level8 :public cocos2d::Scene {
	int _GameState;
	Hero* _hero;
	Sprite* _sea[4];
	Sprite* _layer;//主角的激光精灵
	Sprite* _spark;//主角激光攻击BOSS时产生的火花
	Sprite* BOSS;
	Sprite* _falseBOSS;//这个是boss开大招时的假身
	float _BOSSHP;//boss的生命值
	float _BOSSPowerVal;//boss的蓝量值
	double long _speedX, _speedY;//boss终极技能时的移动速度
	int _skillFlagNum;
	Sprite* _BOSSXueTiao;
	Sprite* _BOSSPOWER;
	bool _pause; //用来标志游戏是否进入暂停状态
	bool _isAttack; //用来标志玩家是否开火状态
	bool _isSkill;  //用来标志boss是否进入金蝉脱壳状态
	unsigned int _stateTime[5]; //玩家状态（上下左右或停留原地）的计时，用来搭配神出鬼没技能的生存判定

	Vector<Sprite*> _solderVector; //普攻的子弹向量表
	Vector<Sprite*> _flowerVector;//普攻子弹爆开花的向量表
	Vector<Sprite*> _stoneVector; //流雨飞石技能的陨石向量表
	Vector<Sprite*> _BOOMSmallVector;//狂轰滥炸前身的现世，越来越小
	Vector<Sprite*> _BOOMVector; //狂轰滥炸真身开始爆炸！
	Vector<Sprite*> _deadBOOM; //boss死亡时，自爆点的向量表

	Text * _text;

	int _deadNum;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level8);
	void update(float delta);

};

