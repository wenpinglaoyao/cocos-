#include "Level5.h"
USING_NS_CC;

cocos2d::Scene* Level5::createScene()
{
	return Level5::create();
}

bool Level5::init()
{
	{
		if (!Scene::init()) return false;

		_taskCount = 0;
		_GameState = 0;
		_barValue = BARMAX;
		auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
		background->setContentSize(Size(400, 800));
		background->setPosition(WID5 + background->getContentSize().width / 2, background->getContentSize().height / 2);
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
				Director::getInstance()->replaceScene(Level5::createScene());
			}
		});
		refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		addChild(refurbish);



		SimpleAudioEngine::getInstance()->playBackgroundMusic("5.mp3",true);//播放背景音乐

		_hero = Hero::createWithSpriteName(HEROSPEED5, 0);//初始化主角
		_hero->setPosition(26, 400);
		addChild(_hero);
		/*_bullet = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(-100, -100);
		addChild(_bullet);

		_bullet = Bullet::createWithSpriteName(BUSP5, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(_hero->getPositionX(), _hero->getPositionY());
		addChild(_bullet);*/
		this->scheduleUpdate();
	}//模板代码结束

	Button* fire = Button::create("BOOM.png");//初始化发射按钮
	fire->ignoreContentAdaptWithSize(false);
	fire->setContentSize(Size(250, 250));
	fire->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type)
		{
			if (!_hero->getFire()) {
				_bullet = Bullet::createWithSpriteName(BUSP5, _hero->getRotation(), "icebullet.png");
				_bullet->setPosition(_hero->getPosition());
				addChild(_bullet);

				for (int i = 0;i < MAXE5;i++)
				{
					if (_enemy[i]->getDirection())
					{
						Bullet* bullet = Bullet::createWithSpriteName(EBUSP5, (_enemy[i]->getDirection() - 1) * 90, "bullet.png");
						bullet->setPosition(_enemy[i]->getPosition());
						bullet->bulletRun();bullet->bulletRun();//先让敌人炮弹飞出自身范围
						addChild(bullet);
						_enemyBulletVector.pushBack(bullet);
					}
				}
				_hero->setFire(true);
			}
		}
	});
	fire->setPosition(Vec2(WIDTH - fire->getContentSize().width / 2, HEIGHT - fire->getContentSize().height / 2));
	addChild(fire);

	for (int i = 0;i < MAXE5;i++)
	{
		_enemy[i] = Tank::createWithSpriteName(ENEMYSP5, int(CCRANDOM_0_1() * 4 + 1), 0, "enemy2.PNG");
		_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection() - 1));
		_enemy[i]->setPosition(100 + i * 51, 100 + i * 51);
		_enemy[i]->setFire(false);
		addChild(_enemy[i], 98);
	}

	_bar = Sprite::create("HP.png");
	_bar->setContentSize(Size(20, _barValue));
	_bar->setPosition(WID5 + _bar->getContentSize().width / 2,300+_bar->getContentSize().height/2);
	addChild(_bar, 2);
	return true;
}

void Level5::update(float delta)
{
	switch (_GameState)
	{
	case 0:
		if (!_pause)//如果还没有进入暂停状态
		{
			{
				_bar->setContentSize(Size(20, _barValue));
				_bar->setPosition(WID5 + _bar->getContentSize().width / 2, 300 + _bar->getContentSize().height / 2);
			}
			_barValue--;
			if (!_barValue) _barValue = BARMAX;
			
			if (_hero->getDirection()) //如果英雄处于前进状态
			{
				_hero->tankRun();
				if (_hero->isInfringeBorder(WID5, HEI5, -_hero->getContentSize().width / 2))
					_hero->tankBack();

				for (int i = 0;i < MAXE5;i++) //如果英雄撞到了别的坦克
				{
					if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox())) {
						_hero->tankBack();
						break;
					}
				}
				for (int i = 0;i < _iceVector.size();i++)
				{
					auto ice = _iceVector.at(i);
					if (_hero->getBoundingBox().intersectsRect(ice->getBoundingBox()))
					{
						_hero->tankBack();
						break;
					}
				}
			}//英雄的行走状态完毕

			 //让子弹飞
			if (_hero->getFire())
			{
				if (_bullet)//如果子弹还存在
				{
					_bullet->bulletRun();//让子弹飞
					bool removeFlag1 = false; //设置一个是否删除子弹的标识
					if (_bullet->isInfringeBorder(WID5, HEI5, _bullet->getContentSize().height / 2)) //如果子弹超越了边界
					{
						removeFlag1 = true;
						goto removeBullet1;
					}//如果子弹越界了，子弹消失

					for (int i = 0;i < MAXE5;i++) //检测子弹与敌人的碰撞
					{
						if (_bullet->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))//如果炮弹击中了敌人
						{
							SimpleAudioEngine::getInstance()->playEffect("beng.mp3");
							auto ice = Sprite::create("ice.png");
							ice->setPosition(_enemy[i]->getPosition());
							addChild(ice);
							_iceVector.pushBack(ice);
							if (++_taskCount >= TASKNUM5) //如果击毙敌人数量多于等于任务量
								_GameState = 1; //游戏胜利
							if (_taskCount <= TASKNUM5 - MAXE5)//这里改成如果击毙敌人数量还少的话，那么给敌人换个位置加载僵冻时间即可
							{
							resetpos1:
								_enemy[i]->setPosition(CCRANDOM_0_1() * (WID5 - 50) + 25, CCRANDOM_0_1() * (HEI5 - 50) + 25);
								if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人的新位置与主角冲突
									goto resetpos1;
								for (int j = 0;j < MAXE5;j++)
								{
									if (i != j && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))//如果新位置与另一个敌人冲突
										goto resetpos1;
								}
								for (auto ice : _iceVector)
								{
									if (_enemy[i]->getBoundingBox().intersectsRect(ice->getBoundingBox()))
										goto resetpos1;
								}
								_enemy[i]->setPositionX(_enemy[i]->getPositionX());
								_enemy[i]->setPositionY(_enemy[i]->getPositionY());
								_enemy[i]->setCool(180);//给敌人加载一个“僵冻”时间
							}
							else //不然就让敌人永久"消失"
							{
								_enemy[i]->setPosition(-50-i*60, -50-i*60);
								_enemy[i]->setDirection(0);
							}
							removeFlag1 = true;
							goto removeBullet1;//这个跳出要加，不然子弹已经删除了，但还会被测试的
						}
					}
				removeBullet1:
					if (true == removeFlag1)
					{
						this->removeChild(_bullet);
						_bullet = nullptr;
						_hero->setFire(false);
					}
				}
			}//英雄的子弹代码完毕

			 //轮到敌人行动了
			for (int i = 0;i < MAXE5;i++)
			{
				if (0 == _enemy[i]->getCool()) //如果该敌人的僵冻时间已经结束了
				{
					if (_barValue<=1) //改变最后那个数据来改变转向概率
					{
						_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
						_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
					}//如果这个敌人改变方向了，那就算了，后面就不测试了
					else
					{
						_enemy[i]->tankRun();//先让这个敌人跑一下
						if (_enemy[i]->isInfringeBorder(WID5, HEI5, -_enemy[i]->getContentSize().width / 2)) //如果该敌人越界了
							_enemy[i]->tankBack(); //那么拉回来
												   //先检测敌人是否撞到英雄
						if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人与主角冲突了
						{
							_enemy[i]->tankBack();
							_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
							_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							break;
						}

						//检测敌人是否撞到冰块
						for (auto ice : _iceVector)
						{
							if (_enemy[i]->getBoundingBox().intersectsRect(ice->getBoundingBox()))
							{
								_enemy[i]->tankBack();
								_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							}
						}

						//最后检测敌人是否撞到自己人
						for (int j = 0;j < MAXE5;j++)
						{
							if (j != i && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))
							{
								_enemy[i]->tankBack();
								_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								break;
							}
						}
					}
				}
				else  //如果敌人的僵冻时间还没有结束
					_enemy[i]->setCool(_enemy[i]->getCool() - 1);//减少敌人的僵冻时间

			}

			//该敌人的子弹行动了
			for (int i = 0;i < _enemyBulletVector.size();i++)
			{
				auto bullet = _enemyBulletVector.at(i);
				bullet->bulletRun();

				if (bullet->isInfringeBorder(WID5, HEI5, 0))//如果子弹越界了
				{
					removeChild(bullet, true);
					_enemyBulletVector.eraseObject(bullet);
					for (int i = 0;i < MAXE5;i++) _enemy[i]->setVisible(true);
					continue;
				}

				if (_hero->getBoundingBox().intersectsRect(bullet->getBoundingBox()))//如果主角被子弹击中
				{
					removeChild(bullet, true);
					_enemyBulletVector.eraseObject(bullet);
					_GameState = -1;
					continue;
				}

				for (int j = 0;j < MAXE5;j++)//检测敌人子弹与敌人的碰撞
				{
					_enemy[j]->setVisible(true);//先让敌人显形
					if (_enemy[j]->getCool() > 1)//如果敌人还处于刚出世阶段
					{
						_enemy[j]->setVisible(false);//再让敌人隐形，怕打死一个敌人新生的位置正好在另一个炮弹发射轨迹内
					}
					else if(_enemy[j]->getBoundingBox().intersectsRect(bullet->getBoundingBox()))//如果这颗炮弹与某一个敌人碰撞了
					{
						removeChild(bullet, true);
						_enemyBulletVector.eraseObject(bullet);
						_GameState = -2;
						goto END;
					}
				}

				for (int j = 0;j < _iceVector.size();j++)//检测敌人的子弹是否与被冰冻的敌人碰撞
				{
					auto ice = _iceVector.at(j);
					if (ice->getBoundingBox().intersectsRect(bullet->getBoundingBox()))
					{
						removeChild(bullet);
						_enemyBulletVector.eraseObject(bullet);
						_GameState = -3;
						goto END;
					}
				}
			END:;
			}
	
		}
		break;
	case 1:
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		//把关卡数设置为1，表示该创建进入第二关了
		{
			if (5 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 5);
				UserDefault::getInstance()->flush();
			}
		}
		_hero->setScale(10.0f);
		Text* winText = Text::create("You Win!!Next level?", "Arial", 64);
		winText->setRotation(270);
		addChild(winText, 99);

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
		addChild(next, 99);
		_GameState = 2;
	}
	break;
	case -1://最后这个可以不加花括号，因为这是最后一个case分支了，不会存在初始化被case跳过的编译报错
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();//停止播放背景音乐
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		auto failText = Text::create(((CCString*)chnStrings->objectForKey("failLevel11"))->getCString(), "", 64);
		failText->setRotation(270);
		failText->setPosition(Vec2(400, 400));
		addChild(failText);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -2:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		auto failText2 = Text::create(((CCString*)chnStrings->objectForKey("failLevel51"))->getCString(), "", 64);
		failText2->setRotation(270);
		failText2->setPosition(Vec2(400, 400));
		addChild(failText2);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -3:
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		auto failText3 = Text::create(((CCString*)chnStrings->objectForKey("failLevel52"))->getCString(), "", 64);
		failText3->setRotation(270);
		failText3->setPosition(Vec2(400, 400));
		addChild(failText3);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
		break;
		break;
	}
}