/*
该英雄是坦克的克星之一
被动技能：该英雄打小兵是远程，打英雄是近战

Q技能：选定一名敌方单位，标记该单位4秒时间，在此期间，该英雄对这个单位的每次远程攻击，子弹都具有穿透效果，对沿途的敌方单位造成80%的真实伤害

W技能：待补充

E技能：待补充

终极技能：选定一个敌方英雄，持续5秒时间。在此期间，该英雄的每次普攻（近战类型的），都会对目标造成0伤害，因为普攻的伤害都转移到了被大招锁定的
敌方英雄身上
*/

/*
一个ADC

终极大招：开启大招后，在持续5秒时间内，该英雄每受一次瞬时伤害（炼金的毒气类型的不算），都会往后被击退一段距离。
*/

/*
终极大招之灵魂出窍：开启大招后，该英雄的真身无法做任何动作，并往前一段距离释放一个克隆身，克隆身的各种属性与真身一样，克隆身持续8秒时间自爆，
在此期间也可被敌人打死，那么真身恢复正常
*/
#pragma once
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include <iostream>
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace ui;
using namespace CocosDenshion;
#define MAX 6

class Level1 :public cocos2d::Scene {
public:
	bool _pause;//检测是否进入暂停状态
	Hero* _hero;//英雄坦克
	Bullet* _bullet1,*_bullet2;//英雄的两个炮弹
	Tank*  _enemy[MAX];//敌人坦克向量表
	Bullet* _bullet[MAX];//敌人的炮弹向量表
	Vector<Sprite*> _obstacleVector;//障碍物的向量表
	int _taskCount;
	int _GameState;

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Level1);
	void update(float delta);
};
