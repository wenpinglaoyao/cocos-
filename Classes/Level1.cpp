#include "Level1.h"
USING_NS_CC;

Scene* Level1::createScene()
{
	return Level1::create();
}

bool Level1::init()
{
	_taskCount = 0;
	_GameState = 0;//这些最好放在最先执行的位置，不然游戏状态等还没有更新呢，背景音乐也没法播放啊
	{
		if (!Scene::init()) return false;

	auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
	background->setContentSize(Size(400, 800));
	background->setPosition(1000 + background->getContentSize().width / 2, background->getContentSize().height / 2);
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
	down->setRotation(180+45);
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
	left->setRotation(270+45);
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
	right->setRotation(90+45);
	right->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		switch (type) {
		case Widget::TouchEventType::BEGAN:_hero->setDirection(2);_hero->setRotation(90);break;
		case Widget::TouchEventType::MOVED:_hero->setDirection(2);break;
		case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
		case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
		}
	});
	right->setPosition(Vec2(1300,200));
	addChild(right);

	Button* pause = Button::create("button.png");//增加一个暂停按钮
	pause->setTitleText("pause");
	pause->setTitleFontSize(32);
	pause->setRotation(90);
	pause->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type) _pause = !_pause;
	});
	pause->setPosition(Vec2(1120,800 - pause->getContentSize().width/2));
	addChild(pause);

	Button* refurbish = Button::create("button.png");//增加一个刷新关卡的按钮
	refurbish->setTitleText("New");
	refurbish->setTitleFontSize(32);
	refurbish->setRotation(270);
	refurbish->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::ENDED == type) {
			Director::getInstance()->replaceScene(Level1::createScene());
		}
	});
	refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width/2));
	addChild(refurbish);



	SimpleAudioEngine::getInstance()->playBackgroundMusic("1.mp3");//播放背景音乐

	_hero = Hero::createWithSpriteName(4.99f, 0);//初始化主角
	_hero->setPosition(500, 400);
	_hero->setPositionX(_hero->getPositionX());//必须让主角的位置跟类内坐标变量同步
	_hero->setPositionY(_hero->getPositionY());
	addChild(_hero);

	_pause = false;
	this->scheduleUpdate();
	}
     /**********以上这些是模板化代码*******************/


	Button* fire = Button::create("BOOM.png"); //因为第一关一次性射两颗子弹，所以没把它带进模板化里面
	fire->ignoreContentAdaptWithSize(false);//通过这个设置为false来调整button尺寸大小
	fire->setContentSize(Size(250, 250));
	fire->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type) {
			if (!_hero->getFire()) {
				_bullet1 = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
				_bullet1->setPosition(_hero->getPosition());
				_bullet1->bulletRun();_bullet1->bulletRun();_bullet1->bulletRun();_bullet1->bulletRun();//先让第一个炮弹飞一会
				addChild(_bullet1);

				_bullet2 = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
				_bullet2->setPosition(_hero->getPosition());
				addChild(_bullet2);

				_hero->setFire(true);
			}
		}
	});
	fire->setPosition(Vec2(WIDTH - fire->getContentSize().width/2,HEIGHT - fire->getContentSize().height/2));
	addChild(fire);

	//下面初始化四周墙壁
	{for (int i = 25;i <= 800;i += 50) {
		auto temp = Sprite::create("obstacle.png");
		temp->setContentSize(Size(50, 50));
		temp->setPosition(25, i);
		_obstacleVector.pushBack(temp);
		addChild(_obstacleVector.at(_obstacleVector.size() - 1));
	}
	for (int i = 25;i <= 1000;i += 50) {
		auto temp = Sprite::create("obstacle.png");
		temp->setContentSize(Size(50, 50));
		temp->setPosition(i, 25);
		_obstacleVector.pushBack(temp);
		addChild(_obstacleVector.at(_obstacleVector.size() - 1));
	}
	for (int i = 25;i <= 800;i += 50) {
		auto temp = Sprite::create("obstacle.png");
		temp->setContentSize(Size(50, 50));
		temp->setPosition(975, i);
		_obstacleVector.pushBack(temp);
		addChild(_obstacleVector.at(_obstacleVector.size() - 1));
	}
	for (int i = 25;i <= 1000;i += 50) {
		auto temp = Sprite::create("obstacle.png");
		temp->setContentSize(Size(50, 50));
		temp->setPosition(i, 775);
		_obstacleVector.pushBack(temp);
		addChild(_obstacleVector.at(_obstacleVector.size() - 1));
	}}//墙壁初始化结束

	//下面初始化6个敌人
	{
		for (int i = 0;i < 6;i++)
		{
			_enemy[i] = Tank::createWithSpriteName(1.01f, int(CCRANDOM_0_1() * 4 + 1),30,"enemy1.PNG");
			_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection()-1));
			_enemy[i]->setPosition(100 + i * 51, 100 + i * 51);
			_enemy[i]->setPositionX(_enemy[i]->getPositionX());
			_enemy[i]->setPositionY(_enemy[i]->getPositionY());
			_enemy[i]->setFire(false);
			addChild(_enemy[i]);
		}
	}//敌人初始化结束
	for (int i = 0;i < 6;i++)//敌人子弹初始化
	{
		_bullet[i] = Bullet::createWithSpriteName(0.0f, int(CCRANDOM_0_1() * 4 + 1),  "bullet.png");
		_bullet[i]->setPosition(_enemy[i]->getPosition());
		_bullet[i]->setVisible(true);
		addChild(_bullet[i]);
	}
	return true;
}

void Level1::update(float delta)
{
	switch (_GameState)
	{
	case 0:
		if (!_pause)//如果还没有进入暂停状态
		{
			if (_hero->getDirection()) //如果英雄处于前进状态
			{
				_hero->tankRun();
				if (_hero->isInfringeBorder(1000, 800, -_hero->getContentSize().width / 2))
					_hero->tankBack();

				for (auto obstacle : _obstacleVector)//如果英雄撞到了障碍物
				{
					if (_hero->getBoundingBox().intersectsRect(obstacle->getBoundingBox())) {
						_hero->tankBack();
						break;
					}
				}
				for (int i = 0;i < 6;i++) //如果英雄撞到了别的坦克
				{
					if (_hero->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox())) {
						_hero->tankBack();
						break;
					}
				}
			}//英雄的行走状态完毕

			 //让子弹飞
			if (_hero->getFire())
			{
				if (_bullet1) {
					_bullet1->bulletRun();
					bool removeFlag1 = false;
					if (_bullet1->isInfringeBorder(1000, 800, _bullet2->getContentSize().height / 2)) {
						_GameState = -1;
						removeFlag1 = true;
						goto removeBullet1;
					}//如果子弹越界了，子弹消失

					for (int i = 0;i < 6;i++) {
						if (_bullet1->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox())) {
							if (++_taskCount >= 20) _GameState = 1;
							SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
							if (_taskCount <= 14)//这里改成如果击毙敌人数量还少的话，那么给敌人换个位置加载僵冻时间即可
							{
							resetpos1:
								_enemy[i]->setPosition(CCRANDOM_0_1() * 1000, CCRANDOM_0_1() * 800);
								if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
									goto resetpos1;//如果新生敌人的位置与主角重叠，那么重新生成新位置
								/***如果新生敌人的位置与另一个敌人重叠，那么重新生成新位置***/
								for (int j = 0;j < 6;j++) {
									if (i != j && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))
										goto resetpos1; 
								}
								/***如果新生敌人的位置与墙壁重叠，那么重新生成新位置****/
								for (int j = 0;j < _obstacleVector.size();j++) {
									auto obstacle = _obstacleVector.at(j);
									if (obstacle->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))
										goto resetpos1;
								}
								_enemy[i]->setPositionX(_enemy[i]->getPositionX());
								_enemy[i]->setPositionY(_enemy[i]->getPositionY());
								_enemy[i]->setCool(80);//给敌人加载一个“僵冻”时间
							}
							else //不然就让敌人永久"消失"
							{
								_enemy[i]->setPosition(-50, -50);
								_enemy[i]->setPositionX(-50);
								_enemy[i]->setPositionY(-50);
								_enemy[i]->setCool(-1);//记得给该敌人加载一个负数的僵冻时间，让负数慢慢减吧。。。
							}
							removeFlag1 = true;
							goto removeBullet1;//这个跳出要加，不然子弹已经删除了，但还会被测试的
						}
					}
				removeBullet1:
					if (true == removeFlag1)
					{
						removeChild(_bullet1);
						_bullet1 = nullptr;
					}
				}
				if (_bullet2)
				{
					_bullet2->bulletRun();
					bool removeFlag2 = false;
					if (_bullet2->isInfringeBorder(1000, 800, _bullet2->getContentSize().height / 2))
					{
						_GameState = -2;
						removeFlag2 = true;
						goto removeBullet2;
					}//如果子弹越界了，子弹消失

					 //检测子弹与敌人的碰撞
					for (int i = 0;i < 6;i++) {
						if (_bullet2->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox())) {
							if (++_taskCount >= 20) _GameState = 1;
							SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
							if (_taskCount <= 14)//这里改成如果击毙敌人数量还少的话，那么给敌人换个位置加载无敌时间即可
							{
							resetpos2:
								_enemy[i]->setPosition(CCRANDOM_0_1() * 1000, CCRANDOM_0_1() * 800);
								if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
									goto resetpos2;
								for (int j = 0;j < 6;j++) {
									if (i != j && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))
										goto resetpos2;
								}
								for (int j = 0;j < _obstacleVector.size();j++) {
									auto obstacle = _obstacleVector.at(j);
									if (obstacle->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))
										goto resetpos2;
								}
								_enemy[i]->setPositionX(_enemy[i]->getPositionX());
								_enemy[i]->setPositionY(_enemy[i]->getPositionY());
								_enemy[i]->setCool(80);//给敌人加载一个“僵冻”时间
							}
							else {
								_enemy[i]->setPosition(-50, -50);
								_enemy[i]->setPositionX(-50);
								_enemy[i]->setPositionY(-50);
								_enemy[i]->setCool(-1);//记得给该敌人加载一个负数的僵冻时间，让负数慢慢减吧。。。
							}

							removeFlag2 = true;
							goto removeBullet2;//这个跳出要加，不然子弹已经删除了，但还会被测试的
						}
					}
				removeBullet2:
					if (removeFlag2 == true) {
						removeChild(_bullet2);
						_bullet2 = nullptr;
					}
				}
				if (nullptr == _bullet1 && nullptr == _bullet2)
					_hero->setFire(false);
			}//英雄的子弹代码完毕

			 //轮到敌人行动了
			for (int i = 0;i < 6;i++)
			{
				if (0 == _enemy[i]->getCool()) //如果该敌人的僵冻时间已经结束了
				{
					if (CCRANDOM_0_1() < 1.0f / 170.0f) //改变最后那个数据来改变转向概率
					{
						_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
						_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
					}//如果这个敌人改变方向了，那就算了，后面就不测试了
					else
					{
						_enemy[i]->tankRun();//先让这个敌人跑一下


						if (_enemy[i]->isInfringeBorder(1000, 800, -_enemy[i]->getContentSize().width / 2)) _enemy[i]->tankBack();

						//先检测敌人是否撞到英雄
						if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) {
							_enemy[i]->tankBack();
							_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
							_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							break;
						}
						//在检测敌人是否撞墙
						for (int j = 0;j < _obstacleVector.size();j++)
						{
							auto obstacle = _obstacleVector.at(j);
							if (_enemy[i]->getBoundingBox().intersectsRect(obstacle->getBoundingBox()))
							{
								_enemy[i]->tankBack();
								_enemy[i]->setDirection((_enemy[i]->getDirection() + 1) % 4 + 1);//敌人碰壁原路返回！不随机转向
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								break;
							}
						}
						//最后检测敌人是否撞到自己人
						for (int j = 0;j < 6;j++)
						{
							if (j != i && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))
							{
								_enemy[i]->tankBack();
								_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
								_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
								break;
							}
						}
						//最后检测这个敌人是否该发射炮弹
						if (false == _enemy[i]->getFire()) {
							_bullet[i]->setVisible(true);
							_bullet[i]->setDirection(90.0f*(_enemy[i]->getDirection() - 1));
							_bullet[i]->setSpeed(3.0f);
							_enemy[i]->setFire(true);
						}
					}
				}
				else  //如果敌人的僵冻时间还没有结束
					_enemy[i]->setCool(_enemy[i]->getCool() - 1);//减少敌人的僵冻时间
		
			}

			//该让敌人的子弹飞一会了
			for (int i = 0;i < 6;i++)
			{
				_bullet[i]->bulletRun();
				if (_bullet[i]->isInfringeBorder(1000, 800, _bullet[i]->getContentSize().height / 2) &&0 <= _enemy[i]->getCool()) {
					_bullet[i]->setPosition(_enemy[i]->getPosition());
					_bullet[i]->setVisible(false);
					_bullet[i]->setSpeed(0.0f);
					_enemy[i]->setFire(false);
					_GameState = -3;
				}
				if (_hero->getBoundingBox().intersectsRect(_bullet[i]->getBoundingBox())) {
					//如果被敌人炮弹击中的话。。。
					_GameState = -4;
				}
				for (int j = 0;j < _obstacleVector.size();j++)
				{
					auto obstacle = _obstacleVector.at(j);
					if (obstacle->getBoundingBox().intersectsRect(_bullet[i]->getBoundingBox())) {
						_bullet[i]->setPosition(_enemy[i]->getPosition());
						_bullet[i]->setVisible(false);
						_bullet[i]->setSpeed(0.0f);
						_enemy[i]->setFire(false);
					}
				}
			}

			//检测主角炮弹是否与四周墙壁发生碰撞
			for (int i = 0;i < _obstacleVector.size();i++)
			{
				auto obstacle = _obstacleVector.at(i);
				if (_bullet1 && obstacle->getBoundingBox().intersectsRect(_bullet1->getBoundingBox()))
				{
					removeChild(obstacle);
					removeChild(_bullet1);
					_obstacleVector.eraseObject(obstacle);
					_bullet1 = nullptr;
					break;
				}
				if (_bullet2 && obstacle->getBoundingBox().intersectsRect(_bullet2->getBoundingBox()))
				{
					removeChild(_bullet2);
					removeChild(obstacle);
					_obstacleVector.eraseObject(obstacle);
					_bullet2 = nullptr;
				}
			}//障碍物的代码（主要是与英雄子弹检测）完毕
		}
		break;
	case 1:
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		{
			if (1 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 1);
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
	case -2:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();//停止播放背景音乐
		CCDictionary* chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		Text* failText2 = Text::create(((CCString*)chnStrings->objectForKey("failLevel12"))->getCString(), "", 64);
		failText2->setRotation(270);
		failText2->setPosition(Vec2(400, 400));
		addChild(failText2);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -3:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();//停止播放背景音乐
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		Text* failText3 = Text::create(((CCString*)chnStrings->objectForKey("failLevel13"))->getCString(), "", 64);
		failText3->setRotation(270);
		failText3->setPosition(Vec2(400, 400));
		addChild(failText3);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	case -4:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();//停止播放背景音乐

	
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		const char * sss = ((CCString *)chnStrings->objectForKey("failLevel11"))->getCString();
		Text* failText4 = Text::create(sss, "", 64);
		failText4->setRotation(270);
		failText4->setPosition(Vec2(400, 400));
		addChild(failText4);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
	break;
	}
}