#ifndef  __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <iostream>
#define WIDTH 1400
#define HEIGHT 800
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;//加上这句才能使用Text文本

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void update(float delta);

	static cocos2d::Scene* createLevelGame();
	static int level;
	Sprite* _authorSprite;
	Text * _bgStory;
	Text* _levelTxt;
	bool _authorFlag;
};


class Tank :public Sprite
{
protected:
	float _speed;//这个是当前坦克的行走速度
	
	int _direction;//这个代表当前坦克的行走方向
	bool _fire;//这个是当前坦克是否处于开火状态的标记
	unsigned int _cool;//这个代表新出生坦克的僵冻时间，也可以用作无敌时间
public:
	Tank() {
		_direction = int(CCRANDOM_0_1()) * 4; //随机初始化该坦克刚诞生时的方向
	}
	static Tank* createWithSpriteName(float speed,int direction,int cool,const std::string& name)//用于坦克精灵的初始化函数
	{
		Tank* tank = new Tank();
		if (tank && tank->initWithFile(name.c_str()))
		{
			tank->autorelease();
			tank->_speed = speed;
			tank->_direction = direction;
			tank->_cool = cool;
			return tank;
		}
		CC_SAFE_DELETE(tank);
		return nullptr;
	}

	void setSpeed(float speed) { _speed = speed; } //设置坦克的行走速度
	float getSpeed() { return _speed; }//获取坦克的行走速度

	void setFire(bool fire) { _fire = fire; }
	bool getFire() { return _fire; }

	void setDirection(int direction) { _direction = direction; }
	int getDirection() { return _direction; }

	void setCool(int cool) { _cool = cool; }
	int getCool() { return _cool; }

	/*坦克前进*/
	void tankRun()
	{
		switch (this->_direction)
		{
		case 1://向上走
			this->setPositionY(this->getPositionY() + this->_speed);
			break;

		case 3://向下走
			this->setPositionY(this->getPositionY() - this->_speed);
			break;

		case 4://向左走
			this->setPositionX(this->getPositionX() - this->_speed);
			break;

		case 2://向右走
			this->setPositionX(this->getPositionX() + this->_speed);
			break;
		}
	}
	/*坦克后退*/
	void tankBack()
	{
		switch (this->_direction)
		{
		case 1://因为方向朝上，所以向下撤退
			this->setPositionY(this->getPositionY() - this->_speed);
			break;

		case 3://因为方向向下，所以向上撤退
			this->setPositionY(this->getPositionY() + this->_speed);
			break;

		case 4://因为方向向左，所以向右撤退
			this->setPositionX(this->getPositionX() + this->_speed);
			break;

		case 2://因为方向向右，所以向左撤退
			this->setPositionX(this->getPositionX() - this->_speed);
			break;
		}
	}

	/*坦克是否越界*/
	bool isInfringeBorder(int WID, int HEI, int limit) {
		switch (_direction) {
		case 1:
			if (this->getPositionY() >= HEI + limit) 
				return true;
		case 3:
			if (this->getPositionY() <= -limit)
				return true;
			break;
		case 2:
			if (this->getPositionX() >= WID + limit) 
				return true;
			break;
		case 4:
			if (this->getPositionX() <= -limit) 
				return true;
			break;
		}
		return false;
	}
};

class Hero :public Tank {
public:
	static Hero* createWithSpriteName(float speed, int direction) {
		Hero* hero = new Hero();
		if (hero && hero->initWithFile("hero.png")) {
			hero->autorelease();
			hero->_speed = speed;
			hero->_direction = direction;
			return hero;
		}
		CC_SAFE_DELETE(hero);
		return nullptr;
	}
};

class Bullet :public Sprite
{
protected:
	int _direction;//炮弹的方向
	float _speed;//炮弹的速度
	int _power;//炮弹的威力

public:
	static Bullet* createWithSpriteName(float speed,int direction,const std::string& name) {
		Bullet* bullet = new Bullet();
		if (bullet && bullet->initWithFile(name)) {
			bullet->autorelease();
			bullet->_speed = speed;
			bullet->_direction = direction;
			return bullet;
		}
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}

	void setDirection(int direction) { _direction = direction; }//设置炮弹方向
	int getDirection() { return _direction; }
	void setSpeed(float speed) { _speed = speed; }//设置炮弹速度
	float getSpeed() { return _speed; }
	void setPower(int power) { _power = power; }//设置炮弹威力
	int getPower() { return _power; }//获取炮弹威力

	void bulletRun()
	{
		switch (this->_direction)
		{
		case 0://向上走
			this->setPositionY(this->getPositionY() + this->_speed);
			break;

		case 180://向下走
			this->setPositionY(this->getPositionY() - this->_speed);
			break;

		case 270://向左走
			this->setPositionX(this->getPositionX() - this->_speed);
			break;

		case 90://向右走
			this->setPositionX(this->getPositionX() + this->_speed);
			break;
		}
	}

	bool isInfringeBorder(int WID, int HEI,int limit) {
		switch (_direction)
		{
		case 0:
			if (this->getPositionY() >= HEI + limit) return true;
		case 180:
			if (this->getPositionY() <= -limit) return true;
			break;
		case 270:
			if (this->getPositionX() <= -limit) return true;
			break;
		case 90:
			if (this->getPositionX() >= WID+limit) return true;
			break;
		}
		return false;
	}
};

class BOSS :public Sprite
{
	int _life;//这个是boss的生命值
	int _power;//这个是boss的蓝量
};
#endif // __HELLOWORLD_SCENE_H__

