#include "Level4.h"
USING_NS_CC;

cocos2d::Scene* Level4::createScene()
{
	return Level4::create();
}


bool Level4::init()
{
	{
		if (!Scene::init()) return false;

		_taskCount = 0;
		_GameState = 0;
		_countInterrogate = 0;
		_sirCount = 1;
		auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
		background->setContentSize(Size(600, 800));
		background->setPosition(WID4 + background->getContentSize().width / 2, background->getContentSize().height / 2);
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
				Director::getInstance()->replaceScene(Level4::createScene());
			}
		});
		refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		addChild(refurbish);



		SimpleAudioEngine::getInstance()->playBackgroundMusic("4.mp3");//播放背景音乐

		_hero = Hero::createWithSpriteName(HEROSPEED, 0);//初始化主角
		_hero->setPosition(26, 400);
		addChild(_hero);
		_bullet = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(-100, -100);
		addChild(_bullet);

		_bullet = Bullet::createWithSpriteName(BUSP, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(_hero->getPositionX(), _hero->getPositionY());
		addChild(_bullet);
		this->scheduleUpdate();
	}//模板代码结束

	//初始化四个石头
	{
	_stone[0] = Sprite::create("obstacle.png");
	_stone[0]->setPosition(STONESIZE4 / 2, STONESIZE4 / 2);
	addChild(_stone[0]);
	_stone[1] = Sprite::create("obstacle.png");
	_stone[1]->setPosition(STONESIZE4 / 2, HEI4 - STONESIZE4 / 2);
	addChild(_stone[1]);
	_stone[2] = Sprite::create("obstacle.png");
	_stone[2]->setPosition(WID4 - STONESIZE4 / 2, HEI4 - STONESIZE4 / 2);
	addChild(_stone[2]);
	_stone[3] = Sprite::create("obstacle.png");
	_stone[3]->setPosition(WID4 - STONESIZE4 / 2, STONESIZE4 / 2);
	addChild(_stone[3]);
	}//石头初始化结束

	std::string s = "sir";
	s.append(StringUtils::format("%d.png", _sirCount));
	_sir = Sprite::create(s.c_str());
	_sir->setPosition(WID4 / 2, HEI4 / 2);
	addChild(_sir);

	for (int i = 0;i < MAXE4;i++)
	{
		_enemy[i] = Tank::createWithSpriteName(ESP4, int(CCRANDOM_0_1() * 4 + 1), 30, "enemy4.png");
		_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection() - 1));
		_enemy[i]->setPosition(80 + i * 51, 80 + i * 51);
		_enemy[i]->setContentSize(Size(50, 50));
		_enemy[i]->setFire(false);
		addChild(_enemy[i], 98);
	}
	for (int i = 0;i < MAXE4;i++)
	{
		int x, y;
		resetPos:
		x = CCRANDOM_0_1()*(WID4-152);
		y = CCRANDOM_0_1()*(HEI4-152);
		x += 76;y += 76;
		_enemy[i]->setPosition(x, y);
		
		for (int j = 0;j < MAXE4;j++)
		{
			if (i!=j&&tankColl(_enemy[i], _enemy[j]))
				goto resetPos;
		}
	}

	return true;
}

void Level4::update(float delta)
{
	switch (_GameState)
	{
	case 0:

		if (!_pause)//如果还没有进入暂停状态
		{
			if (_taskCount >= 3 && _countInterrogate<=0)//如果抓获长官的数量足够多
			{
				_GameState = 1;
			}
			if (_countInterrogate-- == 0)
			{
				_sir->setVisible(true);
			}
			if (_hero->getDirection()) //如果英雄处于前进状态
			{
				_hero->tankRun();
				if (_hero->isInfringeBorder(WID4, HEI4, -_hero->getContentSize().width / 2))
					_hero->tankBack();
				for (int i = 0;i < 4;i++)
				{
					if (tankColl(_hero, _stone[i]))
						_hero->tankBack();
				}

				for (int i = 0;i < MAXE4;i++) //如果英雄撞到了别的坦克
				{
					if (tankColl(_hero,_enemy[i]))
					{
						if (backDetection(_enemy[i], _hero->getDirection(), _hero->getSpeed()))
						{
							_hero->tankBack();
						}
					}
				}

				if (tankColl(_hero, _sir) && _countInterrogate<=0)//如果抓到了可以露世的长官
				{
					//更换长官头像，随机安放新长官位置
					_countInterrogate = INTERROGATETIME;
					_sirCount++;
					_taskCount++;
					std::string s = "sir";
					s.append(StringUtils::format("%d.png", _sirCount));
					_sir->setTexture(s.c_str());
					_sir->setVisible(false);
					int x = CCRANDOM_0_1() * 300 - 150;
					int y = CCRANDOM_0_1() * 300 - 150;
					_sir->setPosition(WID4 / 2 + x, HEI4 / 2 + y);
				}
			}//英雄的行走状态完毕

			 //让子弹飞
			{
				_bullet->bulletRun();

				if (_bullet->isInfringeBorder(WID4, HEI4, _bullet->getContentSize().height / 2))//如果子弹越界了
				{
					_bullet->setPosition(_hero->getPositionX(), _hero->getPositionY());
					_bullet->setDirection(_hero->getRotation());
					_GameState = -1;
				}

				for (int i = 0;i < MAXE4;i++)
				{
					if (tankColl(_bullet, _enemy[i]))
					{
						SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
						_bullet->setPosition(_hero->getPositionX(),_hero->getPositionY());
						_bullet->setDirection(_hero->getRotation());

						_enemy[i]->setPosition(-50, -50);
						_enemy[i]->setDirection(0);
					}
				}
				for (int i = 0;i < 4;i++)
				{
					if (tankColl(_bullet, _stone[i]))
					{
						_bullet->setPosition(_hero->getPositionX(), _hero->getPositionY());
						_bullet->setDirection(_hero->getRotation());
					}
				}
			}//英雄的子弹代码完毕

			 //轮到敌人行动了
			for (int i = MAXE4 - 1;i >= 0;i--)
			{
				if (0 == _enemy[i]->getCool()) //如果该敌人的僵冻时间已经结束了
				{
					{
						_enemy[i]->tankRun();//先让这个敌人跑一下
						if (_enemy[i]->isInfringeBorder(WID4, HEI4, -25)) //如果该敌人越界了
						{
							_enemy[i]->tankBack(); //那么拉回来
							if (3 > _enemy[i]->getDirection()) _enemy[i]->setDirection(_enemy[i]->getDirection() + 2);
							else _enemy[i]->setDirection((_enemy[i]->getDirection() + 2) % 4);

							_enemy[i]->setRotation((_enemy[i]->getDirection() - 1) * 90);
							goto endDetectionColl;
						}

						//先检测敌人是否撞到英雄
						if (tankColl(_enemy[i],_hero)) //如果该敌人与主角冲突了
						{
							_enemy[i]->tankBack();
							_enemy[i]->setDirection((_hero->getRotation()/90)+1);
							_enemy[i]->setRotation((_enemy[i]->getDirection() - 1) * 90);
							goto endDetectionColl;
							break;
						}

						//检测敌人是否撞到石头
						for (int j = 0;j < 4;j++)
						{
							if (tankColl(_enemy[i],_stone[j]))
							{
								_enemy[i]->tankBack();
								if (3 > _enemy[i]->getDirection()) _enemy[i]->setDirection(_enemy[i]->getDirection() + 2);
								else _enemy[i]->setDirection((_enemy[i]->getDirection() + 2) % 4);
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								goto endDetectionColl;
							}
						}

						//最后检测敌人是否撞到自己人
						for (int j = 0;j < MAXE4;j++)
						{
							if (j != i && tankColl(_enemy[i], _enemy[j]))
							{
								_enemy[i]->tankBack();
								///****以下几句代码让着这个碰到自己人的敌人往回走***/
								//{
								//	if (3 > _enemy[i]->getDirection()) _enemy[i]->setDirection(_enemy[i]->getDirection() + 2);
								//	else _enemy[i]->setDirection((_enemy[i]->getDirection() + 2) % 4);
								//	_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								//}
								///***往回走的代码结束***/
								break;
							}
						}
					endDetectionColl:;
					}
				}
				else  //如果敌人的僵冻时间还没有结束
					_enemy[i]->setCool(_enemy[i]->getCool() - 1);//减少敌人的僵冻时间

			}

		}//是否更新状态逻辑完毕
		break;
	case 1:
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		//把关卡数设置为4，表示该创建进入第4关了
		{
			if (4 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 4);
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
				Director::getInstance()->replaceScene(HelloWorld::createScene());
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
		auto failText = Text::create(((CCString*)chnStrings->objectForKey("failLevel41"))->getCString(), "", 64);
		failText->setRotation(270);
		failText->setPosition(Vec2(400, 400));
		addChild(failText, 199);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -2:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Text* failText2 = Text::create("hero be killed by bullet", "Arial", 64);
		failText2->setRotation(270);
		failText2->setPosition(Vec2(400, 400));
		addChild(failText2, 199);
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