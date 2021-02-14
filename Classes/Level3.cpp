#include "Level3.h"
USING_NS_CC;

cocos2d::Scene* Level3::createScene()
{
	return Level3::create();
}

bool Level3::init()
{
	{
		if (!Scene::init()) return false;

		auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
		background->setContentSize(Size(600, 800));
		background->setPosition(WID + background->getContentSize().width / 2, background->getContentSize().height / 2);
		addChild(background);

		Button* up = Button::create("up.png");//增加一个控制坦克向上的按钮
		up->setRotation(45);
		up->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type) {
			case Widget::TouchEventType::BEGAN:_hero->setDirection(1);_hero->setRotation(0);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(1);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		up->setPosition(Vec2(1200, 300));
		addChild(up);

		Button* down = Button::create("up.png");//向下的按钮
		down->setRotation(180 + 45);
		down->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type) {
			case Widget::TouchEventType::BEGAN:_hero->setDirection(3);_hero->setRotation(180);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(3);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		down->setPosition(Vec2(1200, 100));
		addChild(down);

		Button* left = Button::create("up.png");//向左的按钮
		left->setRotation(270 + 45);
		left->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type) {
			case Widget::TouchEventType::BEGAN:_hero->setDirection(4); _hero->setRotation(270);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(4);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		left->setPosition(Vec2(1100, 200));
		addChild(left);

		Button* right = Button::create("up.png");//向右的按钮
		right->setRotation(90 + 45);
		right->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type) {
			case Widget::TouchEventType::BEGAN:_hero->setDirection(2);_hero->setRotation(90);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(2);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		right->setPosition(Vec2(1300, 200));
		addChild(right);

		Button* pause = Button::create("button.png");//增加一个暂停按钮
		pause->setTitleText("pause");
		pause->setTitleFontSize(32);
		pause->setRotation(90);
		pause->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::BEGAN == type) _pause = !_pause;
		});
		pause->setPosition(Vec2(1120, 800 - pause->getContentSize().width / 2));
		addChild(pause);

		Button* refurbish = Button::create("button.png");//增加一个刷新关卡的按钮
		refurbish->setTitleText("New");
		refurbish->setTitleFontSize(32);
		refurbish->setRotation(270);
		refurbish->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::ENDED == type) {
				Director::getInstance()->replaceScene(Level3::createScene());
			}
		});
		refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		addChild(refurbish);



		SimpleAudioEngine::getInstance()->playBackgroundMusic("3.mp3");//播放背景音乐

		_hero = Hero::createWithSpriteName(4.99f, 0);//初始化主角
		_hero->setPosition(500, 400);
		addChild(_hero);
		_bullet = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(-100,-100);
		addChild(_bullet);

		Button* fire = Button::create("BOOM.png");//初始化发射按钮
		fire->ignoreContentAdaptWithSize(false);
		fire->setContentSize(Size(250, 250));
		fire->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::BEGAN == type)
			{
				if (!_hero->getFire()) {
					_bullet->setPosition(_hero->getPosition());
					_bullet->setSpeed(16);
					_bullet->setDirection(_hero->getRotation());
					_hero->setFire(true);

					//创建四个方向的声波
					for (int direction = 0;direction <= 270;direction += 90)
					{
						auto sound = Bullet::createWithSpriteName(SOUND_SPEED, direction, "sound.png");
						sound->setRotation(direction);
						sound->setPosition(_hero->getPositionX(),_hero->getPositionY());
						_soundVector.pushBack(sound);
						addChild(sound, 100);
					}//声波结束
				}
			}
		});
		fire->setPosition(Vec2(WIDTH - fire->getContentSize().width / 2, HEIGHT - fire->getContentSize().height / 2));
		addChild(fire);
		_pause = false;
		this->scheduleUpdate();
	}
	//下面初始化敌人
	{
	for (int i = 0;i < MAXENEMY3;i++)
	{
		_enemy[i] = Tank::createWithSpriteName(ENEMYSPEED, int(CCRANDOM_0_1() * 4 + 1), 0, "snowmonster.png");
		_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection() - 1));
		_enemy[i]->setPosition(100 + i * 51, 100 + i * 51);
		_enemy[i]->setFire(false);
		addChild(_enemy[i],98);
	}
	for (int i = 0;i < MAXENEMY3;i++)
	{
		_enemy[i]->setDirection(4);
		_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection() - 1));
		_enemy[i]->setPosition(300 + (i+1) * 59,100);
		_enemy[i]->setFire(false);
	}
	}//敌人初始化结束

	//在中间添加一个大石块
	_stone = Sprite::create("obstacle.png");
	_stone->setContentSize(Size(STONESIZE, STONESIZE));
	_stone->setPosition(WID / 2, HEI / 2);
	addChild(_stone, 99);
	return true;
}

void Level3::update(float delta)
{
	switch (_GameState)
	{
	case 0:
		if (_taskCount >= TASKNUM && _snowVector.size()<=0 && _soundVector.size()<=0)
		{
			_GameState = 1;
		}
		if (!_pause)//如果还没有进入暂停状态
		{
			if (_hero->getDirection()) //如果英雄处于前进状态
			{
				_hero->tankRun();
				if (_hero->isInfringeBorder(WID, HEI, -_hero->getContentSize().width / 2))
					_hero->tankBack();
				if (tankColl(_hero, _stone))
					_hero->tankBack();

				for (int i = 0;i < MAXENEMY3;i++) //如果英雄撞到了别的坦克
				{
					if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox())) 
					{
						if (_enemy[i]->getCool())
						{
							_GameState = -2;
						}
						if (backDetection(_enemy[i], _hero->getDirection(), _hero->getSpeed()))
						{
							_hero->tankBack();
						}
					}
				}
			}//英雄的行走状态完毕

			 //让子弹飞
			if (_hero->getFire())
			{
				_bullet->bulletRun();

				if (_bullet->isInfringeBorder(WID, HEI, _bullet->getContentSize().height / 2))
				{
					_bullet->setPosition(1000,1000);
					_hero->setFire(false);
				}

				for (int i = 0;i < MAXENEMY3;i++)
				{
					if (tankColl(_bullet, _enemy[i]))//如果子弹击中了雪怪
					{
						SimpleAudioEngine::getInstance()->playEffect("BOOM1.mp3");
						_bullet->setPosition(1000, 1000);
						_bullet->setSpeed(0.0f);
						_hero->setFire(false);
						++_taskCount;
						//创建四个方向的声波
						for (int direction = 0;direction <= 270;direction += 90)
						{
							auto sound = Bullet::createWithSpriteName(SOUND_SPEED, direction, "sound.png");
							sound->setRotation(direction);
							sound->setPosition(_enemy[i]->getPositionX(), _enemy[i]->getPositionY());
							_soundVector.pushBack(sound);
							addChild(sound, 100);
						}//声波结束

						_enemy[i]->setPosition(-100-i*60, -100-i*60);
						_enemy[i]->setDirection(0);
					}
				}
			}//英雄的子弹代码完毕

			 //轮到敌人行动了
			for (int i = MAXENEMY3-1;i >= 0;i--)
			{
				if (_enemy[i]->getCool())
				{
					if (_enemy[i]->getCool() == 1)
					{
						_enemy[i]->setTexture("snowmonster.png");
						_enemy[i]->setSpeed(ENEMYSPEED);
					}
					_enemy[i]->setCool(_enemy[i]->getCool() - 1);
				}
				{
					if (CCRANDOM_0_1() < 1.0f / 970.0f) //改变最后那个数据来改变转向概率
					{
						_enemy[i]->setDirection(_enemy[i]->getDirection()%4+1);
						_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
					}//如果这个敌人改变方向了，那就算了，后面就不测试了
					else
					{
						_enemy[i]->tankRun();//先让这个敌人跑一下
						if (_enemy[i]->isInfringeBorder(WID, HEI, -25)) //如果该敌人越界了
						{
							_enemy[i]->tankBack(); //那么拉回来
							_enemy[i]->setDirection(_enemy[i]->getDirection() % 4 + 1);
							_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							goto endDetectionColl;
						}

						//先检测敌人是否撞到英雄
						if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人与主角冲突了
						{
							_enemy[i]->tankBack();
							_enemy[i]->setDirection(_enemy[i]->getDirection() % 4 + 1);
							_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							if (_enemy[i]->getCool())//主角被生气雪怪撞死
							{
								_GameState = -2;
							}
							goto endDetectionColl;
							break;
						}

						//检测敌人是否撞到石头
						if (_enemy[i]->getBoundingBox().intersectsRect(_stone->getBoundingBox()))
						{
							_enemy[i]->tankBack();
							_enemy[i]->setDirection(_enemy[i]->getDirection() % 4 + 1);
							_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							goto endDetectionColl;
						}

						//最后检测敌人是否撞到自己人
						for (int j = 0;j < MAXENEMY3;j++)
						{
							if (j != i &&/* _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox())*/
								tankColl(_enemy[i],_enemy[j]))
							{
								_enemy[i]->tankBack();
								_enemy[i]->setDirection(_enemy[i]->getDirection() % 4 + 1);
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								if (_enemy[i]->getCool() + _enemy[i]->getCool() > 0)
								{
									if (!_enemy[i]->getCool())
									{
										_enemy[i]->setTexture("dreadmonster.png");
										_enemy[i]->setCool(COOLTIME);
										_enemy[i]->setRotation((_enemy[i]->getDirection() - 1) * 90);
										_enemy[i]->setSpeed(ENEMYSPEED * 8);
									}
									else
									{
										_enemy[j]->setTexture("dreadmonster.png");
										_enemy[j]->setCool(COOLTIME);
										_enemy[j]->setRotation((_enemy[i]->getDirection() - 1) * 90);
										_enemy[j]->setSpeed(ENEMYSPEED * 8);
									}
								}
								break;
							}
						}
						//最后检测这个雪怪是否看到主角
						if (!_enemy[i]->getCool() && isShoot(_enemy[i]))
						{
							{
								_enemy[i]->setTexture("dreadmonster.png");
								_enemy[i]->setCool(COOLTIME);
								//下面两行代码是改变雪怪的方向，让它背对主角逃跑
								if (3 > _enemy[i]->getDirection()) _enemy[i]->setDirection(_enemy[i]->getDirection() + 2);
								else _enemy[i]->setDirection((_enemy[i]->getDirection() + 2) % 4);

								_enemy[i]->setRotation((_enemy[i]->getDirection() - 1) * 90);
								_enemy[i]->setSpeed(ENEMYSPEED * 8);
								//创建四个方向的声波
								for (int direction = 0;direction <= 270;direction += 90)
								{
									auto sound = Bullet::createWithSpriteName(SOUND_SPEED, direction, "sound.png");
									sound->setRotation(direction);
									sound->setPosition(_enemy[i]->getPositionX(), _enemy[i]->getPositionY());
									_soundVector.pushBack(sound);
									addChild(sound, 100);
								}//声波结束
							}
						}
					endDetectionColl:;
					}
				}
			}

			//该让声波飞一会了
			for (int si = 0;si < _soundVector.size();si++)
			{
				auto sound = _soundVector.at(si);
				sound->bulletRun();
				if (sound->getPositionX() > HEI || sound->getPositionX() < 0
					|| sound->getPositionY() > WID || sound->getPositionY() < 0)
				{
					createSnowLine(sound->getDirection());
					removeChild(sound, true);
					_soundVector.eraseObject(sound);
				}
			}//声波更新完毕

			//雪崩的检测
			for (int i = 0;i < _snowVector.size();i++)
			{
				auto snow = _snowVector.at(i);
				snow->bulletRun();

				_stone->setContentSize(Size(20, 20));//先让巨石缩小
				if (tankColl(snow, _stone))
					goto removeSnow;
				_stone->setContentSize(Size(STONESIZE, STONESIZE));//再让巨石变大

				_hero->setContentSize(Size(16, 16));//让主角变小
				if (tankColl(_hero, snow)) //如果主角被雪崩碰到
				{
					_GameState = -1;
					goto removeSnow;
				}
				_hero->setContentSize(Size(50, 50));//让主角变大

				if (snow->getPositionX() > HEI +50|| snow->getPositionX() < -50
					|| snow->getPositionY() > WID+50 || snow->getPositionY() < -50)
					goto removeSnow;
				continue;
			removeSnow:
				removeChild(snow);
				_snowVector.eraseObject(snow);
			}
		}//是否更新状态逻辑完毕
		break;
	case 1:
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		//把关卡数设置为4，表示该创建进入第4关了
		{
			if (3 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 3);
				UserDefault::getInstance()->flush();
			}
		}
		_hero->setScale(10.0f);
		Text* winText = Text::create("You Win!!Next level?", "Arial", 64);
		winText->setRotation(270);
		addChild(winText, 199);

		auto next = Button::create("button.png");
		next->setTitleText("Next");
		next->setRotation(270);
		next->setTitleFontSize(32);
		next->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::ENDED == type) //在这里面添加进入下一关开场的逻辑
			{
				Director::getInstance()->replaceScene(HelloWorld::createLevelGame());
			}
		});
		next->setPosition(Vec2(600, 400));
		addChild(next, 199);
		_GameState = 2;
	}
	break;
	case -1://最后这个可以不加花括号，因为这是最后一个case分支了，不会存在初始化被case跳过的编译报错
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();//停止播放背景音乐
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		auto failText = Text::create(((CCString*)chnStrings->objectForKey("failLevel32"))->getCString(), "", 64);
		failText->setRotation(270);
		failText->setPosition(Vec2(400, 400));
		addChild(failText,199);
		_snowVector.clear();
		_soundVector.clear();
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -2:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		auto failText2 = Text::create(((CCString*)chnStrings->objectForKey("failLevel31"))->getCString(), "", 64);
		failText2->setRotation(270);
		failText2->setPosition(Vec2(400, 400));
		addChild(failText2,199);
		_snowVector.clear();
		_soundVector.clear();
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -3:
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Text* failText3 = Text::create("GameOver!partner be killed by BOOM", "Arial", 64);
		failText3->setRotation(270);
		failText3->setPosition(Vec2(400, 400));
		addChild(failText3);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
		break;
		break;
	}
}

/****下面这个是一个递归函数，用来实现主角推土机的功能，让主角
可以推着敌人、障碍物一起前进，由于在cocos引擎中没有这种轮子，
所以这里我们得自己做一个****/
bool Level3::backDetection(Tank* tank, int direction, float speed)
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
	tank->setSpeed(0);

	if (this->isInfringeBorder(tank,direction,-tank->getContentSize().width / 2))//如果这个坦克移位后越界了
	{ 
		backTank(tank, direction, speed);
		tank->setSpeed(ENEMYSPEED);
		return true;
	}
	if (tankColl(tank,_stone))//如果这个坦克撞到巨石了
	{
		backTank(tank, direction, speed);
		tank->setSpeed(ENEMYSPEED);
		return true;
	}
		

	for (int i = 0;i < MAXENEMY3;i++)
	{
		if (_enemy[i]->getSpeed() && tankColl(_enemy[i],tank))//如果该坦克重叠了别的坦克
		{
			if (backDetection(_enemy[i], direction, speed)==true)
			{
				backTank(tank, direction, speed);
				tank->setSpeed(ENEMYSPEED);
				return true;
			}
		}
	}
	tank->setSpeed(ENEMYSPEED);
	return false;
}

bool Level3::isInfringeBorder(Tank* tank, int direction,int limit)
{
	switch (direction)
	{
	case 1:
		if (tank->getPositionY() >= HEI + limit)
			return true;
	case 3:
		if (tank->getPositionY() <= -limit)
			return true;
		break;
	case 2:
		if (tank->getPositionX() >= WID + limit)
			return true;
		break;
	case 4:
		if (tank->getPositionX() <= -limit)
			return true;
		break;
	}
	return false;
}
void Level3::backTank(Tank* tank, int direction, float speed)
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

bool Level3::tankColl(Sprite* t1, Sprite* t2)//这一关要自己写个碰撞检测函数，不然有bug
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

bool Level3::isShoot(Tank* tank)//是否该发射
{
	float offsetX = _hero->getPositionX() - tank->getPositionX();
	float offsetY = _hero->getPositionY() - tank->getPositionY();
	switch (tank->getDirection())
	{
	case 1:
		if (offsetX<=50 && offsetX>=-50 && _hero->getPositionY() > tank->getPositionY())
			return true;
		break;
	case 3:
		if (offsetX <= 50 && offsetX >= -50 &&_hero->getPositionY() < tank->getPositionY())
			return true;
		break;
	case 4:
		if (offsetY <= 50 && offsetY >= -50 &&_hero->getPositionX() < tank->getPositionX())
			return true;
		break;
	case 2:
		if (offsetY <= 50 && offsetY >= -50 &&_hero->getPositionX() > tank->getPositionX())
			return true;
		break;
	}
	return false;
}

void Level3::createSnowLine(int direction)
{
	switch (direction)
	{
	case 0:
		for (int i = 0;i < WID;i += SNOWDISTANCE)
		{
			auto snow = Bullet::createWithSpriteName(SNOW_SPEED, 180, "snow.png");
			snow->setRotation(180);
			snow->setPosition(i, HEI + 25);
			_snowVector.pushBack(snow);
			addChild(snow);
		}
		break;
	case 90:
		for (int i = 0;i < HEI;i += SNOWDISTANCE)
		{
			auto snow = Bullet::createWithSpriteName(SNOW_SPEED, 270, "snow.png");
			snow->setRotation(270);
			snow->setPosition(WID+25,i);
			_snowVector.pushBack(snow);
			addChild(snow);
		}
		break;
	case 180:
		for (int i = 0;i < WID;i += SNOWDISTANCE)
		{
			auto snow = Bullet::createWithSpriteName(SNOW_SPEED, 0, "snow.png");
			snow->setRotation(0);
			snow->setPosition(i, -25);
			_snowVector.pushBack(snow);
			addChild(snow);
		}
		break;
	case 270:
		for (int i = 0;i < WID;i += SNOWDISTANCE)
		{
			auto snow = Bullet::createWithSpriteName(SNOW_SPEED, 90, "snow.png");
			snow->setRotation(90);
			snow->setPosition(-25,i);
			_snowVector.pushBack(snow);
			addChild(snow);
		}
		break;
	}
}