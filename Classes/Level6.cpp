#include "Level6.h"
USING_NS_CC;

cocos2d::Scene* Level6::createScene()
{
	return Level6::create();
}

bool Level6::init()
{
	{
		if (!Scene::init()) return false;

		_taskCount = 0;
		_GameState = 0;
		_barValue = BARMAX6;
		_speedX = _speedY = 0.0f;
		_moveFlag = false;
		_coolTime = 0;
		auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
		background->setContentSize(Size(400, 800));
		background->setPosition(WID6 + background->getContentSize().width / 2, background->getContentSize().height / 2);
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
				Director::getInstance()->replaceScene(Level6::createScene());
			}
		});
		refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		addChild(refurbish);



		SimpleAudioEngine::getInstance()->playBackgroundMusic("6.mp3", true);//播放背景音乐

		_hero = Hero::createWithSpriteName(HEROSPEED5, 0);//初始化主角
		_hero->setPosition(26, 400);
		addChild(_hero);
		_bullet = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(-100, -100);
		addChild(_bullet);

		_aim = Sprite::create("bullet.png");
		addChild(_aim,999);

		_aim2 = Sprite::create("bullet.png");
		addChild(_aim2, 999);

	
		this->scheduleUpdate();
	}//模板代码结束

	Button* fire = Button::create("BOOM.png");//初始化发射按钮
	fire->ignoreContentAdaptWithSize(false);
	fire->setContentSize(Size(250, 250));
	fire->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type)
		{
			if (!_hero->getFire()) {
				_bullet = Bullet::createWithSpriteName(BUSP6, _hero->getRotation(), "icebullet.png");
				_bullet->setPosition(_hero->getPosition());
				addChild(_bullet);

				_hero->setFire(true);
			}
		}
	});
	fire->setPosition(Vec2(WIDTH - fire->getContentSize().width / 2, HEIGHT - fire->getContentSize().height / 2));
	addChild(fire);

	for (int i = 0;i < MAXE6;i++) //敌人的初始化
	{
		_enemy[i] = Tank::createWithSpriteName(ENEMYSP6, int(CCRANDOM_0_1() * 4 + 1), 0, "snowmonster.png");
		_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection() - 1));
		_enemy[i]->setPosition(100 + i * 51, 150 + i * 51);
		_enemy[i]->setFire(false);
		addChild(_enemy[i], 98);
	}
	for (int i = 0;i < MAXE6;i++)
	{
		_enemyBullet[i] = Bullet::createWithSpriteName(EBUSP6, (_enemy[i]->getDirection() - 1) * 90, "bullet.png");
		_enemyBullet[i]->setPosition(_enemy[i]->getPosition());
		addChild(_enemyBullet[i]);
	}

	_bar = Sprite::create("HP.png");
	_bar->setContentSize(Size(20, _barValue));
	_bar->setPosition(WID6 + _bar->getContentSize().width / 2, 300 + _bar->getContentSize().height / 2);
	addChild(_bar, 2);

	_football = Sprite::create("football.png");
	_football->setContentSize(Size(BALLSIZE, BALLSIZE));
	_football->setPosition(WID6 / 2, HEI6 / 2);
	addChild(_football, 100);

	_star = Sprite::create("star.png");
	_star->setVisible(false);
	addChild(_star, 101);

	return true;
}

void Level6::update(float delta)
{
	switch (_GameState)
	{
	case 0:
		if (!_pause)//如果还没有进入暂停状态
		{
			_aim->setPosition(_hero->getPosition());//先调整主角身上的准星
			_aim2->setPosition(_football->getPosition());//再调整足球中心的准星
			//先让走动条走走
			{
				_bar->setContentSize(Size(20, _barValue));
				_bar->setPosition(WID6 + _bar->getContentSize().width / 2, 300 + _bar->getContentSize().height / 2);
				_barValue -= 0.3;
			}
			
			if (_barValue<=0)//如果走动条走完了
			{
				_moveFlag = true; //陨石开始现世
				_star->setVisible(true); //陨石露面
				_starSize = STARMAXSIZE; //陨石先设置成最大尺寸
				_coolTime = 0; //开始冻世计时
				_barValue = BARMAX6; //走动条重新设置为满血
			}

			_football->setPosition(_football->getPositionX() + _speedX, _football->getPositionY() + _speedY);//足球移动
			for (int i = 0;i < MAXE6;i++)//检测足球是否撞到敌人
			{
				//若足球是移动状态并且撞到了敌人
				if (_enemy[i]->getBoundingBox().intersectsCircle(_football->getPosition(), BALLSIZE / 2) && (_speedX || _speedY))
				{
					_speedX = _speedY = 0.0f;//足球静止住

					if (++_taskCount >= TASKNUM6) //如果击毙敌人数量多于等于任务量
						_GameState = 1; //游戏胜利
					if (_taskCount <= TASKNUM6 - MAXE6)//这里改成如果击毙敌人数量还少的话，那么给敌人换个位置加载僵冻时间即可
					{
					resetpos1:
						_enemy[i]->setPosition(CCRANDOM_0_1() * (WID6 - 50) + 25, CCRANDOM_0_1() * (HEI6 - 50) + 25);
						if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人的新位置与主角冲突
							goto resetpos1;
						for (int j = 0;j < MAXE6;j++)
						{
							if (i != j && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))//如果新位置与另一个敌人冲突
								goto resetpos1;
						}
						_enemy[i]->setPositionX(_enemy[i]->getPositionX());
						_enemy[i]->setPositionY(_enemy[i]->getPositionY());
						_enemy[i]->setCool(80);//给敌人加载一个“僵冻”时间
					}
					else //不然就让敌人永久"消失"
					{
						_enemy[i]->setPosition(-50 - i * 60, -50 - i * 60);
						_enemy[i]->setDirection(0);
					}
					break;
				}
			}

			if (true == _moveFlag)//如果陨石已经现世或者足球正在移动
			{
				if (STARMAXSIZE <= _starSize) //如果是冻世刚开始
				{
					_star->setPosition(_hero->getPosition()); //让陨石对准主角
				}
			
				if (STARMINSIZE < _starSize) //如果陨石还没有足够接近地面（不能砸到主角或足球）
				{
					_starSize--; //陨石继续掉落
					_star->setContentSize(Size(_starSize, _starSize)); //让陨石变小（掉落中。。。)
				}
				else if(_star->getBoundingBox().intersectsCircle(_football->getPosition(),BALLSIZE/2-15))//若陨石够近了并且砸到了足球
				{
					SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
					 //给足球设置速度让它移动，并把陨石弹开
					{
						_speedX = _football->getPositionX() - _star->getPositionX();
						_speedY = _football->getPositionY() - _star->getPositionY();
						_speedX /= 5;
						_speedY /= 5;

						MoveBy*  moveBy = MoveBy::create(1.0, Vec2(-_speedX*100, -_speedY*100));
						_star->runAction(moveBy);//足球把陨石弹开了
						_moveFlag = false; //冻世结束
					}
				}
				else //不然就是陨石没被弹开（没砸到足球），而是砸到主角了
				{
					_moveFlag = false; //冻世结束
					_GameState = -2;
				}

				_coolTime++; //冻世的时间走动
			}
			else //不然玩家和敌人可以自由活动
			{
				if (_hero->getDirection()) //如果英雄处于前进状态
				{
					_hero->tankRun();
					if (_hero->isInfringeBorder(WID6, HEI6, -_hero->getContentSize().width / 2))
						_hero->tankBack();

					for (int i = 0;i < MAXE6;i++) //如果英雄撞到了别的坦克
					{
						if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox())) {
							_hero->tankBack();
							break;
						}
					}

					if (_hero->getBoundingBox().intersectsCircle(_football->getPosition(),BALLSIZE/2-12))
					{
						_hero->tankBack();
						moveByDir();
					}
				}//英雄的行走状态完毕

				 //让子弹飞
				if (_hero->getFire())
				{
					if (_bullet)//如果子弹还存在
					{
						_bullet->bulletRun();//让子弹飞
						bool removeFlag1 = false; //设置一个是否删除子弹的标识
						if (_bullet->isInfringeBorder(WID6, HEI6, _bullet->getContentSize().height / 2)) //如果子弹超越了边界
						{
							removeFlag1 = true;
							goto removeBullet1;
						}//如果子弹越界了，子弹消失

						/****如果主角的子弹击中了足球***/
						if (_bullet->getBoundingBox().intersectsRect(_football->getBoundingBox())) {
							_barValue -= 10;
							removeFlag1 = true;
							goto removeBullet1;
						}

						for (int i = 0;i < MAXE6;i++) //检测子弹与敌人的碰撞
						{
							if (_bullet->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))//如果炮弹击中了敌人
							{
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
				for (int i = 0;i < MAXE6;i++)
				{
					if (0 == _enemy[i]->getCool()) //如果该敌人的僵冻时间已经结束了
					{
						{
							_enemy[i]->setSpeed(ENEMYSP6 + _taskCount*SPADD);
							_enemy[i]->tankRun();//先让这个敌人跑一下
							if (_enemy[i]->isInfringeBorder(WID6, HEI6, -_enemy[i]->getContentSize().width / 2)) //如果该敌人越界了
							{
								_enemy[i]->tankBack(); //那么拉回来
								if (3 > _enemy[i]->getDirection()) _enemy[i]->setDirection(_enemy[i]->getDirection() + 2);
								else _enemy[i]->setDirection((_enemy[i]->getDirection() + 2) % 4);

								_enemy[i]->setRotation((_enemy[i]->getDirection() - 1) * 90);
							}

							if (_enemy[i]->getBoundingBox().intersectsCircle(_football->getPosition(), BALLSIZE / 2))//如果敌人撞到了足球
							{
								_enemy[i]->tankBack(); //先拉回来

								if (3 > _enemy[i]->getDirection()) _enemy[i]->setDirection(_enemy[i]->getDirection() + 2);//原路返回
								else _enemy[i]->setDirection((_enemy[i]->getDirection() + 2) % 4);

								_enemy[i]->setRotation((_enemy[i]->getDirection() - 1) * 90);
							}
								
													   //先检测敌人是否撞到英雄
							if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人与主角冲突了
							{
								_enemy[i]->tankBack();
								_enemy[i]->setDirection(_hero->getRotation()/90 + 1);//让敌人的方向变得跟主角一致
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								break;
							}

							//最后检测敌人是否撞到自己人
							for (int j = 0;j < MAXE6;j++)
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
				for (int i = 0;i < MAXE6;i++)
				{
					_enemyBullet[i]->bulletRun();

					if (_enemyBullet[i]->isInfringeBorder(WID6, HEI6, 0))//如果子弹越界了
					{
						_enemyBullet[i]->setPosition(_enemy[i]->getPosition());
						_enemyBullet[i]->setDirection((_enemy[i]->getDirection() - 1) * 90);
						continue;
					}

					if (_hero->getBoundingBox().intersectsRect(_enemyBullet[i]->getBoundingBox()))//如果主角被子弹击中
					{
						_enemyBullet[i]->setPosition(_enemy[i]->getPosition());
						_enemyBullet[i]->setSpeed(0);
						_enemyBullet[i]->setVisible(false);
						_GameState = -1;
						continue;
					}

					if (_enemyBullet[i]->getBoundingBox().intersectsRect(_football->getBoundingBox()))//如果该敌人子弹击中了足球
					{
						_enemyBullet[i]->setPosition(_enemy[i]->getPosition());
						_enemyBullet[i]->setDirection((_enemy[i]->getDirection() - 1) * 90);
						continue;
					}
				}
			}

		}
		break;
	case 1:
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		//把关卡数设置为1，表示该创建进入第二关了
		{
			if (6 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 6);
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
		auto failText2 = Text::create(((CCString*)chnStrings->objectForKey("failLevel61"))->getCString(), "", 64);
		failText2->setRotation(270);
		failText2->setPosition(Vec2(400, 400));
		addChild(failText2);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -3:
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Text* failText3 = Text::create("ICE be killed by bullet", "Arial", 64);
		failText3->setRotation(270);
		failText3->setPosition(Vec2(400, 400));
		addChild(failText3);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
		break;
		break;
	}
}