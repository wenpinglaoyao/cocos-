#include "Level2.h"
USING_NS_CC;
#define WID 1000 //屏幕的宽度
#define HEI 800 //屏幕的高度
#define ENEMYBUSP 6 //敌人子弹的速度
#define ENEMYALL 30; //需要消灭的总敌人数量
#define BOOMLIVE 15 //爆炸扩张的最多时间
#define ADD 1.50f    //每死一个敌人后，爆炸扩张的速度增加量
#define TASKNUM 30 //这一关要消灭敌人的任务量
#define STOPTIME 2.0f //这个是猪队友的静止时间


cocos2d::Scene* Level2::createScene()
{
	return Level2::create();
}
bool Level2::init()
{
	_taskCount = 0;
	_GameState = 0;
	_addCircle = 35.0f;
	{
		if (!Scene::init()) return false;

		auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
		background->setContentSize(Size(400, 800));
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
				Director::getInstance()->replaceScene(Level2::createScene());
			}
		});
		refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		addChild(refurbish);



		SimpleAudioEngine::getInstance()->playBackgroundMusic("2.mp3");//播放背景音乐

		_hero = Hero::createWithSpriteName(4.99f, 0);//初始化主角
		_hero->setPosition(500, 400);
		_hero->setPositionX(_hero->getPositionX());//必须让主角的位置跟类内坐标变量同步
		_hero->setPositionY(_hero->getPositionY());
		addChild(_hero);

		_pause = false;
		this->scheduleUpdate();
	}
	/**********以上这些是模板化代码*******************/


	Button* fire = Button::create("BOOM.png");//初始化发射按钮
	fire->ignoreContentAdaptWithSize(false);
	fire->setContentSize(Size(250, 250));	
	fire->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type) 
		{
			if (!_hero->getFire()) {
				_bullet = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
				_bullet->setPosition(_hero->getPosition());
				addChild(_bullet);

				_hero->setFire(true);
			}
		}
	});
	fire->setPosition(Vec2(WIDTH - fire->getContentSize().width / 2, HEIGHT - fire->getContentSize().height / 2));
	addChild(fire);

	  //下面初始化敌人
	{
		for (int i = 0;i < MAX_ENEMY;i++)
		{
			_enemy[i] = Tank::createWithSpriteName(1.01f, int(CCRANDOM_0_1() * 4 + 1), 30, "enemy2.PNG");
			_enemy[i]->setRotation(90.0f * (_enemy[i]->getDirection() - 1));
			_enemy[i]->setPosition(100 + i * 51, 100 + i * 51);
			_enemy[i]->setPositionX(_enemy[i]->getPositionX());
			_enemy[i]->setPositionY(_enemy[i]->getPositionY());
			_enemy[i]->setFire(false);
			addChild(_enemy[i]);
		}
	}//敌人初始化结束
	 //敌人子弹初始化
	for (int i = 0;i <  MAX_ENEMY;i++)
	{
		_enemyBullet[i] = Bullet::createWithSpriteName(0.0f, int(CCRANDOM_0_1() * 4 + 1), "bullet.png");
		_enemyBullet[i]->setPosition(_enemy[i]->getPosition());
		_enemyBullet[i]->setVisible(true);
		addChild(_enemyBullet[i]);
	}
	//友军初始化
	for (int i = 0;i < MAX_PARTNER;i++)
	{
		_partner[i] = Tank::createWithSpriteName(0.5f, int(CCRANDOM_0_1() * 4 + 1), 30, "partner.png");
	/*	_partner[i]->setRotation(90.0f * (_partner[i]->getDirection() - 1));*/
		_partner[i]->setPosition(100 + i * 51, 100 + i * 51);
		_partner[i]->setPositionX(_partner[i]->getPositionX());
		_partner[i]->setPositionY(_partner[i]->getPositionY());
		_partner[i]->setFire(true);
		addChild(_partner[i]);
	}

	return true;
}

void Level2::update(float delta)
{
	switch (_GameState)
	{
	case 0:
		if (!_pause)//如果还没有进入暂停状态
		{
			for ( int i = 0;i < MAX_PARTNER;i++)//更新猪队友的状态
			{
				if (_partner[i]->getFire()) //如果该队友的一个动作执行完毕
				{
					_partner[i]->setFire(false);
					int x = (int)(CCRANDOM_0_1() * WID);
					int y = (int)(CCRANDOM_0_1() * HEI);
					float line = sqrt((x - _partner[i]->getPositionX())*(x - _partner[i]->getPositionX()) +
						(y - _partner[i]->getPositionY())*(y - _partner[i]->getPositionY()));
					MoveTo* moveTo = MoveTo::create(line/50.0f, Vec2(x, y));
					DelayTime* stopTime = DelayTime::create(STOPTIME);
					CallFunc* callFunc;
					switch (i) {
					case 0:
						callFunc = CallFunc::create([&] {
							_partner[0]->setFire(true);
						});
						break;
					case 1:
						callFunc = CallFunc::create([&] {
							_partner[1]->setFire(true);
						});
						break;
					case 2:
						callFunc = CallFunc::create([&] {
							_partner[2]->setFire(true);
						});
						break;
					}

					Sequence* sequence = Sequence::create( moveTo, stopTime, callFunc,NULL);
					_partner[i]->runAction(sequence);
				}
			}
			if (_hero->getDirection()) //如果英雄处于前进状态
			{
				_hero->tankRun();
				if (_hero->isInfringeBorder(WID, HEI, -_hero->getContentSize().width / 2))
					_hero->tankBack();

				for (int i = 0;i < MAX_ENEMY;i++) //如果英雄撞到了别的坦克
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
				if (_bullet)//如果子弹还存在
				{
					_bullet->bulletRun();//让子弹飞
					bool removeFlag1 = false; //设置一个是否删除子弹的标识
					if (_bullet->isInfringeBorder(WID, HEI, _bullet->getContentSize().height / 2)) //如果子弹超越了边界
					{
						removeFlag1 = true;
						goto removeBullet1;
					}//如果子弹越界了，子弹消失

					for (int i = 0;i < MAX_ENEMY;i++) //检测子弹与敌人的碰撞
					{
						if (_bullet->getBoundingBox().intersectsRect(_enemy[i]->getBoundingBox()))//如果炮弹击中了敌人
						{
							SimpleAudioEngine::getInstance()->playEffect("BOOM1.mp3");
							//打倒敌人，敌人爆炸产生
							{
								Bullet* boom = Bullet::createWithSpriteName(20.0f, 0, "BOOM.png");//20是敌人爆炸的初始范围
								boom->setPower(0);
								boom->setPosition(_enemy[i]->getPositionX(), _enemy[i]->getPositionY());
								addChild(boom, 99);
								_BOOMVector.pushBack(boom);
							}
							if (++_taskCount >= TASKNUM) //如果击毙敌人数量多于等于任务量
								_GameState = 1; //游戏胜利
							if (_taskCount <= TASKNUM - MAX_ENEMY)//这里改成如果击毙敌人数量还少的话，那么给敌人换个位置加载僵冻时间即可
							{
							resetpos1:
								_enemy[i]->setPosition(CCRANDOM_0_1() * (WID-50)+25, CCRANDOM_0_1() * (HEI-50)+25);
								if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人的新位置与主角冲突
									goto resetpos1;
								for (int j = 0;j < MAX_ENEMY;j++) 
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
						this->removeChild(_bullet);
						_bullet = nullptr;
						_hero->setFire(false);
					}
				}
			}//英雄的子弹代码完毕

			 //轮到敌人行动了
			for (int i = 0;i < MAX_ENEMY;i++)
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
						if (_enemy[i]->isInfringeBorder(WID, HEI, -_enemy[i]->getContentSize().width / 2)) //如果该敌人越界了
							_enemy[i]->tankBack(); //那么拉回来
						//先检测敌人是否撞到英雄
						if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox())) //如果该敌人与主角冲突了
						{
							_enemy[i]->tankBack();
							_enemy[i]->setDirection(CCRANDOM_0_1() * 4 + 1);
							_enemy[i]->setRotation(90.0f*(_enemy[i]->getDirection() - 1));
							break;
						}

						//最后检测敌人是否撞到自己人
						for (int j = 0;j < MAX_ENEMY;j++)
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
							_enemyBullet[i]->setVisible(true);
							_enemyBullet[i]->setDirection(90.0f*(_enemy[i]->getDirection() - 1));
							_enemyBullet[i]->setSpeed(4.0f);
							_enemy[i]->setFire(true);
						}
					}
				}
				else  //如果敌人的僵冻时间还没有结束
					_enemy[i]->setCool(_enemy[i]->getCool() - 1);//减少敌人的僵冻时间

			}

			//该让敌人的子弹飞一会了
			for (int i = 0;i < MAX_ENEMY;i++)
			{
				_enemyBullet[i]->bulletRun();
				if (_enemyBullet[i]->isInfringeBorder(WID, HEI, _enemyBullet[i]->getContentSize().height / 2)) {
					_enemyBullet[i]->setPosition(_enemy[i]->getPosition());
					_enemyBullet[i]->setVisible(false);
					_enemyBullet[i]->setSpeed(0.0f);
					_enemy[i]->setFire(false);
				}
				if (_hero->getBoundingBox().intersectsRect(_enemyBullet[i]->getBoundingBox())) {
					//如果被敌人炮弹击中的话。。。
					_GameState = -1;
				}
			}//敌人子弹更新完毕

			//爆炸链更新
			for (int i = 0;i < _BOOMVector.size();i++)
			{
				auto boom = _BOOMVector.at(i);
				
				//检测爆炸有没有杀伤周围的单位
				{
					if (circleDetection(boom->getPosition(), boom->getContentSize().width / 2, _hero->getPosition()))
					{
						_GameState = -1;
					}

					for (int i = 0;i < MAX_ENEMY;i++)
					{
						if (!_enemy[i]->getCool() && //如果敌人的僵冻时间结束并且炸到了敌人
							circleDetection(boom->getPosition(), boom->getContentSize().width / 2, _enemy[i]->getPosition()))
						{
							if (++_taskCount >= TASKNUM) _GameState = 1;
							if (_taskCount <= TASKNUM - MAX_ENEMY) //如果还能有新敌人产生
							{
								//打倒敌人，敌人爆炸产生
								{
									Bullet* boom = Bullet::createWithSpriteName(20.0f, 0, "BOOM.png");
									boom->setPower(0);
									boom->setPosition(_enemy[i]->getPositionX(), _enemy[i]->getPositionY());
									addChild(boom, 99);
									_BOOMVector.pushBack(boom);
								}
								//更新一个新敌人
								{
								resetpos2:
									_enemy[i]->setPosition(CCRANDOM_0_1() * (WID-50)+25, CCRANDOM_0_1() * (HEI-50)+25);
									if (_enemy[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
										goto resetpos2;
									for (int j = 0;j < MAX_ENEMY;j++)
									{
										if (i != j && _enemy[i]->getBoundingBox().intersectsRect(_enemy[j]->getBoundingBox()))
											goto resetpos2;
									}
									_enemy[i]->setPositionX(_enemy[i]->getPositionX());
									_enemy[i]->setPositionY(_enemy[i]->getPositionY());
									_enemy[i]->setCool(80);//给敌人加载一个“僵冻”时间
								}
							}
							else //击杀敌人够多了，这个敌人死亡后，不再有替补上场
							{
								_enemy[i]->setPosition(-50, -50);
								_enemy[i]->setPositionX(-50);
								_enemy[i]->setPositionY(-50);
								_enemy[i]->setCool(-1);//记得给该敌人加载一个负数的僵冻时间，让负数慢慢减吧。。。
							}

							_addCircle += ADD;//最后不忘更新一个爆炸扩张的速度（随敌人被炸死的量增加而增加）
						}//检测敌人被炸更新完毕

						//如果炸到了猪队友
						for (int i = 0;i < MAX_PARTNER;i++)
						{
							if (circleDetection(boom->getPosition(), boom->getContentSize().width / 2 - 10, _partner[i]->getPosition()))
							{
								_GameState = -3;
							}
						}//检测猪队友被炸更新完毕
						if (circleDetection(boom->getPosition(), boom->getContentSize().width / 2, _hero->getPosition())) {
							_GameState = -2;
						}
					}
				}
				
				if (BOOMLIVE <= boom->getDirection()) //如果该爆炸扩张到极限
				{
					boom->setVisible(boom->getDirection() % 3);//让爆炸一闪一闪的更逼真
					/*boom->setRotation(CCRANDOM_0_1() * 360);*/ //可以考虑让爆炸旋转
					if (BOOMLIVE*2 <= boom->getDirection()) //如果爆炸存在太长时间，该让它消失了
					{
						removeChild(boom);
						_BOOMVector.eraseObject(boom);
					}
				}
				else //不然该爆炸扩张
				{

					boom->setSpeed(boom->getSpeed() + _addCircle);//让爆炸范围扩张一下
					boom->setContentSize(Size(boom->getSpeed(), boom->getSpeed()));
				}
				boom->setDirection(boom->getDirection() + 2);//增加爆炸的扩张时间
			}
		}
		break;
	case 1:
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		//把关卡数设置为1，表示该创建进入第二关了
		{
			if (2 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 2);
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
		auto failText2 = Text::create(((CCString*)chnStrings->objectForKey("failLevel21"))->getCString(), "", 64);
		failText2->setRotation(270);
		failText2->setPosition(Vec2(400, 400));
		addChild(failText2);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
	}
		break;
	case -3:
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		auto failText3 = Text::create(((CCString*)chnStrings->objectForKey("failLevel22"))->getCString(), "", 64);
		failText3->setRotation(270);
		failText3->setPosition(Vec2(400, 400));
		addChild(failText3);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的
		break;
	break;
	}
}

bool Level2::circleDetection(Point circlePos, float radius, Point tankPos)
{
	float bb = sqrt((circlePos.x - tankPos.x)*(circlePos.x - tankPos.x) + (circlePos.y - tankPos.y)*(circlePos.y - tankPos.y));
	return radius + 25.0f > bb;
}