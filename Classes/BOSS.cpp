#include "BOSS.h"
USING_NS_CC;

cocos2d::Scene* Level8::createScene()
{
	return Level8::create();
}

bool Level8::init()
{
	if (!Scene::init()) return false;

	_GameState = 0;//游戏状态是进行中
	_BOSSHP = BOSSHP; //为boss设置生命值
	_BOSSPowerVal = 9700; //为boss设置初始蓝量值
	_speedX = CCRANDOM_0_1() * 2 - 1;
	_speedY = CCRANDOM_0_1() * 2 - 1;

	/****添加海绵宝宝的介绍，胜利时用***/
	{
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
		_text = Text::create(((CCString*)chnStrings->objectForKey("bossStory"))->getCString(), "", 26);
		_text->setRotation(270);
		_text->setPosition(Vec2(900, 400));
		_text->setVisible(false);
		addChild(_text);
	}
	/****介绍海绵宝宝的代码段结束***/

	/****添加一个背景层****/
	{
		auto background = Sprite::create("BG.png");
		background->setContentSize(Size(400, 800));
		background->setPosition(WID8 + background->getContentSize().width / 2, background->getContentSize().height / 2);
		addChild(background, 100);
	}
	/****背景层逻辑结束****/

	/****添加上下左右四个按钮*****/
	{
		Button* up = Button::create("up.png");//增加一个控制坦克向上的按钮
		up->setRotation(45);
		up->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type)
			{
			case Widget::TouchEventType::BEGAN: _hero->setDirection(1);if (!_isAttack)_hero->setRotation(0);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(1);if (!_isAttack)_hero->setRotation(0);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		up->setPosition(Vec2(1200, 300));
		addChild(up, 101);

		Button* down = Button::create("up.png");//向下的按钮
		down->setRotation(180 + 45);
		down->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:_hero->setDirection(3);if (!_isAttack)_hero->setRotation(180);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(3);if (!_isAttack)_hero->setRotation(180);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		down->setPosition(Vec2(1200, 100));
		addChild(down, 101);

		Button* left = Button::create("up.png");//向左的按钮
		left->setRotation(270 + 45);
		left->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type) {
			case Widget::TouchEventType::BEGAN:_hero->setDirection(4);if (!_isAttack)_hero->setRotation(270);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(4);if (!_isAttack)_hero->setRotation(270);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		left->setPosition(Vec2(1100, 200));
		addChild(left, 101);

		Button* right = Button::create("up.png");//向右的按钮
		right->setRotation(90 + 45);
		right->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type) {
			case Widget::TouchEventType::BEGAN:_hero->setDirection(2);if (!_isAttack)_hero->setRotation(90);break;
			case Widget::TouchEventType::MOVED:_hero->setDirection(2);if (!_isAttack)_hero->setRotation(90);break;
			case Widget::TouchEventType::ENDED:_hero->setDirection(0);break;
			case Widget::TouchEventType::CANCELED:_hero->setDirection(0);break;
			}
		});
		right->setPosition(Vec2(1300, 200));
		addChild(right, 101);
	}
	/****上下左右按钮添加逻辑结束*****/

	/****添加暂停与刷新这两个按钮****/
	{
		Button* pause = Button::create("button.png"); //增加一个暂停按钮
		pause->setTitleText("pause");
		pause->setTitleFontSize(32);
		pause->setRotation(90);
		pause->setScaleY(4.0);
		pause->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::BEGAN == type) _pause = !_pause;
		});
		pause->setPosition(Vec2(1150, 800 - pause->getContentSize().width / 2));
		addChild(pause, 102);

		//Button* refurbish = Button::create("button.png"); //增加一个刷新按钮
		//refurbish->setTitleText("New");
		//refurbish->setTitleFontSize(32);
		//refurbish->setRotation(270);
		//refurbish->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type)
		//{
		//	if (Widget::TouchEventType::ENDED == type)
		//		Director::getInstance()->replaceScene(Level8::createScene());
		//});
		//refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		//addChild(refurbish, 103);
	}
	/****暂停与刷新俩按钮的添加逻辑结束****/

	SimpleAudioEngine::getInstance()->playBackgroundMusic("8.mp3");//播放关卡背景音乐

	/*****激光的添加，要早在fire前面，因为fire里面要用到它****/
	{
		_layer = Sprite::create("BG.png");
		_layer->setAnchorPoint(Vec2(0.5, 0));
		_layer->setVisible(false);
		addChild(_layer, 2);
	}
	/****激光添加结束****/
	
	/****添加主角****/
	{
		_hero = Hero::createWithSpriteName(HEROSPEED8, 10);
		_hero->setPosition(900, 400);
		_hero->setCool(HEROHP);//这里给主角设置一个最大生命值
		addChild(_hero);
	}
	/****主角添加逻辑结束****/
	
	/****添加一个开火按钮****/
	{
		Button* fire = Button::create("BOOM.png");
		fire->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				_isAttack = true;
				break;
			case Widget::TouchEventType::MOVED:
				_isAttack = true;
				break;
			case Widget::TouchEventType::CANCELED:
				_isAttack = false;
				_layer->setVisible(false);
				break;
			case Widget::TouchEventType::ENDED:
				_isAttack = false;
				_layer->setVisible(false);
			}
		});
		fire->setPosition(Vec2(WIDTH - fire->getContentSize().width / 2, HEIGHT - fire->getContentSize().height / 2));
		addChild(fire, 103);
	}
	/****开火按钮逻辑添加结束****/

	/****添加boss以及假boss*****/
	{
		BOSS = Sprite::create("boss.jpg");
		BOSS->setPosition(0, 400);
		addChild(BOSS, 99);

		_falseBOSS = Sprite::create("boss.jpg");
		_falseBOSS->setPosition(-200, -200);//让这个假身boss一边凉快去(隐形）
		addChild(_falseBOSS, 99);
	}
	/****boss和它的分身添加逻辑结束*****/

	///****添加四周的海洋***/
	{
		_sea[0] = Sprite::create("button.png");
		_sea[0]->setContentSize(Size(1400, BOSS->getContentSize().width / 2+5));
		_sea[0]->setPosition(WID8 / 2, HEI8 - _sea[0]->getContentSize().height / 2+27);
		this->addChild(_sea[0]);//添加一个上边的海洋

		_sea[1] = Sprite::create("button.png");
		_sea[1]->setContentSize(_sea[0]->getContentSize());
		_sea[1]->setPosition(WID8 / 2, _sea[1]->getContentSize().height / 2 - 27);
		addChild(_sea[1]);//添加一个下边的海洋

		_sea[2] = Sprite::create("button.png");
		_sea[2]->setContentSize(Size(BOSS->getContentSize().width / 2+5,1400));
		_sea[2]->setPosition(_sea[2]->getContentSize().width / 2 - 27,HEI8/2);
		addChild(_sea[2]);

		_sea[3] = Sprite::create("button.png");
		_sea[3]->setContentSize(_sea[2]->getContentSize());
		_sea[3]->setPosition(WID8 - _sea[3]->getContentSize().width / 2 + 27, HEI8 / 2);
		addChild(_sea[3]);
	}
	///****添加海洋逻辑结束***/

	/****boss的血条还有蓝量框的添加*****/
	{
		_BOSSXueTiao = Sprite::create("HP.png");
		_BOSSXueTiao->setAnchorPoint(Vec2(0.5, 0));
		_BOSSXueTiao->setPosition(Vec2(1020, 400 - _BOSSXueTiao->getContentSize().height / 2));
		addChild(_BOSSXueTiao, 104);

		_BOSSPOWER = Sprite::create("button.png");
		_BOSSPOWER->setAnchorPoint(Vec2(0.5, 0));
		_BOSSPOWER->setContentSize(_BOSSXueTiao->getContentSize());//别忘了让蓝条和血量条大小尺寸一致
		_BOSSPOWER->setPosition(_BOSSXueTiao->getPosition());
		addChild(_BOSSPOWER, 105);
	}
	/****boss血条以及蓝条添加的逻辑结束*****/

	/****添加一个激光迸溅的火花效果***/
	{
		_spark = Sprite::create("BOOM.png");
		_spark->setVisible(false);
		addChild(_spark, 106);
	}
	/****火花效果添加的逻辑结束***/

	/*********/

	/*********/
	this->scheduleUpdate();

	return true;
}

void Level8::update(float delta)
{
	switch (_GameState)
	{
	case 0://游戏进行中
	{
		if (!_pause)//如果游戏没有暂停
		{
			/****先让火花的尺寸逐渐缩小****/
			_spark->setContentSize(Size(_spark->getContentSize().width - REDUCE, _spark->getContentSize().height - REDUCE));
			if (1 >= _spark->getContentSize().width)//如果火花的尺寸足够笑了
				_spark->setVisible(false);//那么让火花隐形
			/****关于火花的逻辑结束****/

			if(!_isSkill)//非大招情况下，让boss的假身隐形（这样boss在开始放大招时，如果boss不是在受伤状态，主角很难判定boss落脚点了）
				_falseBOSS->setVisible(false);
		
			if(0 <=_BOSSHP) _BOSSXueTiao->setScaleY((_BOSSHP+14) / BOSSHP);//更新boss生命值血量条
			_BOSSPOWER->setScaleY((_BOSSPowerVal+14) / BOSSHP);//更新boss的能量蓝条

			/****如果主角的生命值已经被打没了*****/
			if (_hero->getCool() <= 0)
			{
				auto* rotateTo = RotateBy::create(0.2, 360*10);
				_hero->runAction(rotateTo);
				_hero->setCool(10);
				_GameState = -1;
			}
			/****主角死亡的逻辑处理结束******/

			/*****主角移动的逻辑开始****/
			if (_hero->getDirection())
			{
				_hero->tankRun();//让主角移动
				//如果主角碰到了墙壁
				if (_hero->isInfringeBorder(WID8, HEI8, -_hero->getContentSize().width / 2))
					_hero->tankBack();

				/****如果主角进入海洋，让主角退回***/
				for (int i = 0;i < 4;i++)
				{
					if (_sea[i]->getBoundingBox().intersectsRect(_hero->getBoundingBox()))//如果主角碰到了四周的海洋
					{
						_hero->tankBack();//那么让主角退回
						break;
					}
				}
				/*****检测主角海洋退回的逻辑结束****/
			}
			/*****移动的逻辑结束*****/

			////下面这4行代码开启全屏攻击，实验用，一定要删除
			//if (!_isSkill)
			//{
			//	_BOSSHP -= AGGRESSIVITY;
			//}

			/****主角开火的逻辑开始****/
			if (_isAttack) //如果主角是开火状态
			{
				switch ((int)_hero->getRotation())
				{
				case 0://向上射的激光
					_layer->setContentSize(Size(LAYERWID, HEI8 - _hero->getPositionY()));
					_layer->setAnchorPoint(Vec2(0.5, 0));
					_layer->setPosition(_hero->getPosition());
					if (_layer->getBoundingBox().intersectsRect(BOSS->getBoundingBox()))//如果激光射到了boss
					{
						_layer->setContentSize(Size(LAYERWID, BOSS->getPositionY() - _hero->getPositionY() - BOSS->getContentSize().width / 2));//重新设置激光长度

						//产生对应的火花
						_spark->setPosition(_hero->getPositionX(), BOSS->getPositionY() - BOSS->getContentSize().width / 2);
						_spark->setContentSize(Size(SPARKSIZE, SPARKSIZE));
						_spark->setVisible(true);

						if (!_isSkill)//如果boss没有在放大招状态下
						{
							_BOSSHP -= AGGRESSIVITY;
							_falseBOSS->setVisible(true);//让假身是显形状态，这样boss放大招时，就可以留一个假身让主角判定落脚点了
						}
					}
					_layer->setVisible(true);//让激光显形
					break;
				case 90://向右射的激光
					_layer->setContentSize(Size(WID8 - _hero->getPositionX(), LAYERWID));
					_layer->setAnchorPoint(Vec2(0, 0.5));
					_layer->setPosition(_hero->getPosition());
					if (_layer->getBoundingBox().intersectsRect(BOSS->getBoundingBox()))//如果激光射到了boss
					{
						_layer->setContentSize(Size(BOSS->getPositionX() - _hero->getPositionX()-BOSS->getContentSize().width/2, LAYERWID));//重新设置激光长度

						//产生对应的火花
						_spark->setPosition(BOSS->getPositionX() - BOSS->getContentSize().width / 2, _hero->getPositionY());
						_spark->setContentSize(Size(SPARKSIZE, SPARKSIZE));
						_spark->setVisible(true);

						if (!_isSkill)//如果boss没有在放大招状态下
						{
							_BOSSHP -= AGGRESSIVITY;
							_falseBOSS->setVisible(true);//让假身是显形状态，这样boss放大招时，就可以留一个假身让主角判定落脚点了
						}
					}
					_layer->setVisible(true);//让激光显形
					break;
				case 180://向下射的激光
					_layer->setContentSize(Size(LAYERWID, _hero->getPositionY()));
					_layer->setAnchorPoint(Vec2(0.5, 1));
					_layer->setPosition(_hero->getPosition());
					if (_layer->getBoundingBox().intersectsRect(BOSS->getBoundingBox()))//如果激光射到了boss
					{
						_layer->setContentSize(Size(LAYERWID, _hero->getPositionY() - BOSS->getPositionY() - BOSS->getContentSize().width/2));//重新设置激光长度

						//产生对应的火花
						_spark->setPosition(_hero->getPositionX(), BOSS->getPositionY() + BOSS->getContentSize().width / 2);
						_spark->setContentSize(Size(SPARKSIZE, SPARKSIZE));
						_spark->setVisible(true);

						if (!_isSkill)//如果boss没有在放大招状态下
						{
							_BOSSHP -= AGGRESSIVITY;
							_falseBOSS->setVisible(true);//让假身是显形状态，这样boss放大招时，就可以留一个假身让主角判定落脚点了
						}
					}

					_layer->setVisible(true);//让激光显形
					break;
				case 270://向左射的激光
					_layer->setContentSize(Size(_hero->getPositionX(), LAYERWID));
					_layer->setAnchorPoint(Vec2(1, 0.5));
					_layer->setPosition(_hero->getPosition());
					if (_layer->getBoundingBox().intersectsRect(BOSS->getBoundingBox()))
					{
						_layer->setContentSize(Size(_hero->getPositionX() - BOSS->getPositionX()-BOSS->getContentSize().width/2, LAYERWID));//重新设置激光长度

						//产生对应的火花
						_spark->setPosition(BOSS->getPositionX() + BOSS->getContentSize().width / 2, _hero->getPositionY());
						_spark->setContentSize(Size(SPARKSIZE, SPARKSIZE));
						_spark->setVisible(true);

						if (!_isSkill)//如果boss没有在放大招状态下
						{
							_BOSSHP -= AGGRESSIVITY;
							_falseBOSS->setVisible(true);//让假身是显形状态，这样boss放大招时，就可以留一个假身让主角判定落脚点了
						}	
					}
					_layer->setVisible(true);//让激光显形
				}
			}
			/****开火的逻辑结束****/

			/***接下来该BOSS逻辑了***/
			if (0 < _BOSSHP)//如果BOSS还有血量存活着
			{
				if (_hero->getBoundingBox().intersectsRect(BOSS->getBoundingBox()))//如果boss碰到了主角
				{
					_hero->setCool(_hero->getCool() - NORMALHURT);//那么让主角受伤
				}

				if (!_isSkill)//如果boss不是在释放大招的状态
				{
					_BOSSPowerVal += POWERRESUME;//先让boss的蓝量增加

					/****随机让boss移动位置以及是否发射普攻***/
					{
						/***游戏完工时，记得把下面这些注释的代码恢复，因为它们决定boss正常状态下的位移和普攻***/
						int randFlag = CCRANDOM_0_1() * 1200;
						
						/*****如果boss没有在释放技能，那么让boss位移****/
						if ( 0 >= _stoneVector.size() && 0 >= _BOOMSmallVector.size() && _BOSSHP > 1000)
						{
							///****boss全屏移动*****/
							//{
							//	BOSS->stopAllActions();
							//	float posX, posY;
							//	posX = CCRANDOM_0_1() * WID8;
							//	posY = CCRANDOM_0_1() * HEI8;
							//	float timeSpeed = sqrt((BOSS->getPositionX() - posX) * (BOSS->getPositionX() - posX) +
							//		(BOSS->getPositionY() - posY) * (BOSS->getPositionY() - posY)) / 30;
							//	auto moveTo = MoveTo::create(timeSpeed, Vec2(posX, posY));
							//	BOSS->runAction(moveTo);
							//}
							///****boss全屏移动逻辑结束*****/
						
							/*****四海boss贴海移动****/
							{
								float posX = BOSS->getPositionX() + _speedX;
								float posY = BOSS->getPositionY() + _speedY;
								if (50 < posX && WID8 - 50 > posX && 50 < posY && HEI8 - 50 > posY
									|| 0>posX || WID8<posX || 0>posY || HEI8<posY)
								{
									_speedX = CCRANDOM_0_1() * 1 - 0.5;
									_speedY = CCRANDOM_0_1() * 1 - 0.5;
								}
								else  BOSS->setPosition(posX, posY);
							}
							/****贴海移动逻辑结束****/
						}
						/*****boss位移逻辑结束*****/

						/****随机让boss发射普攻子弹****/
						if ((_BOSSHP > 2000) >= randFlag &&
							_stoneVector.size()<=0 && _BOOMSmallVector.size()<= 0)//如果boss生命值低于2000，不会发射普攻子弹
						{
							float subX = _hero->getPositionX() - BOSS->getPositionX();//获取boss与主角的水平差
							float subY = _hero->getPositionY() - BOSS->getPositionY();//获取boss与主角的垂直差
							float distance = sqrt(subX*subX + subY*subY);//获取主角与boss之间的距离
							Sprite* solder = Sprite::create("bullet.png");
							solder->setContentSize(Size(40, 40));
							solder->setPosition(BOSS->getPosition());
							float speedX = subX*SOLDERSPEED / distance;
							float speedY = subY*SOLDERSPEED / distance;
							solder->setPositionZ(speedX);
							solder->setRotation(speedY);
							this->addChild(solder,100);
							_solderVector.pushBack(solder);

						}
						/****boss发射子弹的逻辑结束***/
					}
					/****boss的随机移动和普攻逻辑结束****/

					/*****先刷新普攻小兵的逻辑状态****/
					for (unsigned int i = 0;i < _solderVector.size();i++)
					{
						Sprite* solder = _solderVector.at(i);

						if (solder->getBoundingBox().intersectsRect(_hero->getBoundingBox()))//如果子弹小兵碰到了主角
						{
							_hero->setCool(_hero->getCool() - NORMALHURT);//那么让主角受伤
						}

						/****如果主角在攻击状态并且击中了boss的普攻小兵，那么小兵爆发天女散花****/
						if (_isAttack && solder->getBoundingBox().intersectsRect(_layer->getBoundingBox()))
						{
							float startX, startY;
							startX = solder->getPositionX();
							startY = solder->getPositionY();
							for (unsigned int angle = 0;angle < 360;angle += ANGLEADD)
							{
								float endX, endY;
								endX = startX + 2000 * cosf(angle*M_PI / 180);
								endY = startY + 2000 * sinf(angle*M_PI / 180);

								Sprite* flower = Sprite::create("bullet.png");
								/***如果需要，在这一行把天女散花的子弹尺寸调大***/
								flower->setPosition(solder->getPosition());//让这些花子弹的初始位置和普攻小兵死亡时的位置重合
								this->addChild(flower);
								MoveTo* moveTo = MoveTo::create(3.0f, Vec2(endX, endY));
								flower->runAction(moveTo);

								_flowerVector.pushBack(flower);
							}

							this->removeChild(solder, true);
							_solderVector.eraseObject(solder);
							continue;
						}
						/****小兵天女散花的逻辑结束****/

						/****这里的代码让普攻小兵位移****/
						{
							//以下下代码让普攻小兵位移
							float speedX, speedY;
							speedX = solder->getPositionZ();
							speedY = solder->getRotation();
							solder->setPositionX(solder->getPositionX() + speedX);
							solder->setPositionY(solder->getPositionY() + speedY);

							if (solder->getPositionX() < 0 || WID8 < solder->getPositionX())//如果该子弹兵在横坐标上超出屏幕左或右
								solder->setPositionZ(-speedX); //那么设置一个弹回
							if (solder->getPositionY() < 0 || HEI8 < solder->getPositionY()) //如果该子弹兵在纵坐标上超出屏幕上或下
								solder->setRotation(-speedY); //那么设置一个弹回
						}
						/****普攻小兵位移的逻辑结束****/
					}
					/******普攻小兵的逻辑结束*******/

					/******刷新天女散花子弹的逻辑*******/
					for (unsigned int i = 0;i < _flowerVector.size();i++)
					{
						Sprite* flower = _flowerVector.at(i);

						if (flower->getBoundingBox().intersectsRect(_hero->getBoundingBox()))//如果该天女散花子弹与主角重叠
						{
							_hero->setCool(_hero->getCool() - NORMALHURT);//那么让主角受伤
						}

						/****如果花子弹飞出了屏幕****/
						if (0 > flower->getPositionX() || WID8<flower->getPositionX() || 0>flower->getPositionY() || HEI8 < flower->getPositionY())
						{
							this->removeChild(flower, true);
							_flowerVector.eraseObject(flower);
							continue;
						}
						/****飞出屏幕的花子弹逻辑结束****/

						/***检测该天女散花的子弹是否撞到了别的小兵子弹***/
						for (unsigned int j = 0;j < _solderVector.size();j++)
						{
							auto solder = _solderVector.at(j);
							/****如果天女散花的这个子弹撞到了某个普攻小兵，那么在该位置爆出天女花****/
							if (flower->getBoundingBox().intersectsRect(solder->getBoundingBox()))
							{
								{
									float startX, startY;
									startX = solder->getPositionX();
									startY = solder->getPositionY();
									for (unsigned int angle = 0;angle < 360;angle += ANGLEADD)
									{
										float endX, endY;
										endX = startX + 2000 * cosf(angle*M_PI / 180);
										endY = startY + 2000 * sinf(angle*M_PI / 180);
										auto flo = Sprite::create("icebullet.png");
										
										/***如果需要，在这一行把天女散花的子弹尺寸调大***/
										
										flo->setPosition(solder->getPosition());
										this->addChild(flo);
										MoveTo* moveTo = MoveTo::create(3.0, Vec2(endX, endY));
										flo->runAction(moveTo);

										_flowerVector.pushBack(flo);
									}
								}

								_flowerVector.eraseObject(flower);
								this->removeChild(flower, true);//先清除掉互撞的花子弹
								_solderVector.eraseObject(solder);
								this->removeChild(solder, true);//再清除互撞的普攻子弹
								break;
							}
							/****爆出天女花的逻辑结束****/
						}
						/****天女散花子弹撞到别的普攻子弹的逻辑结束****/
					}
					/******天女散花的逻辑状态结束****/

					/***天外飞仙的状态逻辑开始***/
					for (unsigned int i = 0;i < _stoneVector.size();i++)
					{
						auto stone = _stoneVector.at(i);
						if (stone->getBoundingBox().intersectsRect(_hero->getBoundingBox()))//如果某颗陨石与主角相撞
						{
							_hero->setCool(_hero->getCool() - NORMALHURT);//那么让主角受伤
						}

						/****如果主角主角的激光扫到了某颗陨石****/
						if (_isAttack && _layer->getBoundingBox().intersectsRect(stone->getBoundingBox()))
						{
							stone->setColor(Color3B::RED);//让被激光射到的陨石变成流血变成红色
							stone->stopAllActions();//让被激光射到的陨石暂停移动
							/****如果被射爆的母石，那么在该位置爆出天女花*****/
							if(0<stone->getPositionZ())
							{
								switch (int(_hero->getRotation()))//让激光黏住被伤到的母石
								{
								case 0:case 180:
									stone->setPositionX(_hero->getPositionX());
									break;
								case 90:case 270:
									stone->setPositionY(_hero->getPositionY());
									break;
								}
								stone->setPositionZ(stone->getPositionZ() - REDSTONELETHSLITY);//让母石受伤
								if (0 >= stone->getPositionZ()) //表达式为母陨石该爆了
								{
									float startX, startY;
									startX = stone->getPositionX();
									startY = stone->getPositionY();
									for (unsigned int angle = 0;angle < 360;angle += ANGLEADD)
									{
										float endX, endY;
										endX = startX + 2000 * cosf(angle*M_PI / 180);
										endY = startY + 2000 * sinf(angle*M_PI / 180);
										Sprite* flower = Sprite::create("bullet.png");
										flower->setPosition(stone->getPosition());
										addChild(flower);
										MoveTo* moveTo = MoveTo::create(3.0, Vec2(endX, endY));
										flower->runAction(moveTo);
										_flowerVector.pushBack(flower);
									}
									this->removeChild(stone, true);
									_stoneVector.eraseObject(stone);
								}
								continue;
							}
							/****母石爆出天女花逻辑结束*****/

							this->removeChild(stone, true);
							_stoneVector.eraseObject(stone);
							continue;
						}
						
						if (WID8 < stone->getPositionX())//如果流星雨飞出了屏幕（飞出了屏幕的右边）
						{
							_stoneVector.eraseObject(stone);
							this->removeChild(stone, true);
							continue;
						}

						/****如果这颗陨石被天女散花的花子弹击中，那么该陨石爆开天女散花****/
						{
							for (unsigned int j = 0;j < _flowerVector.size();j++)
							{
								auto flower = _flowerVector.at(j);
								if (stone->getBoundingBox().intersectsRect(flower->getBoundingBox()))
								{
									float startX, startY;
									startX = stone->getPositionX();
									startY = stone->getPositionY();
									for (int angle = 0;angle < 360;angle += ANGLEADD/6)
									{
										float endX, endY;
										endX = startX + 2000 * cosf(angle*M_PI / 180);
										endY = startY + 2000 * sinf(angle*M_PI / 180);
										Sprite* flow = Sprite::create("icebullet.png");
										flow->setPosition(startX, startY);
										addChild(flow);
										MoveTo* moveTo = MoveTo::create(3.0, Vec2(endX, endY));
										flow->runAction(moveTo);
										_flowerVector.pushBack(flow);
									}

									this->removeChild(flower, true);
									_flowerVector.eraseObject(flower);//先清除掉撞到陨石上的花子弹
									this->removeChild(stone, true);
									_stoneVector.eraseObject(stone);//最后别忘了清除掉这个被花子弹撞到的陨石
									break;
								}
							}
						}
						/****陨石爆开天女散花的逻辑结束*****/
					}
					/***天外飞仙状态的逻辑更新结束***/
					
					/*****狂轰滥炸炸弹逻辑开始****/
					for (unsigned int i = 0;i < _BOOMSmallVector.size();i++)//更新狂轰滥炸前身的逻辑状态
					{
						auto boom = _BOOMSmallVector.at(i);
						//下面这句代码让狂轰滥炸炸弹逐渐变小
						boom->setContentSize(Size(boom->getContentSize().width - BOOMSMALLSUB, boom->getContentSize().height - BOOMSMALLSUB));

						/****如果狂轰滥炸炸弹可以显形了****/
						if (BOOMSMALLMINSIZE > boom->getContentSize().width)//如果狂轰滥炸炸弹缩小的足够了
						{
							boom->setVisible(true);//那么让它显形
							if (_isAttack && _layer->getBoundingBox().intersectsRect(boom->getBoundingBox()))//如果在开火状态并且激光射到了这个未引爆炸弹
							{
								this->removeChild(boom, true);//那么消除掉这个未引爆炸弹
								_BOOMSmallVector.eraseObject(boom);
								break;
							}
						}
						/****让狂轰滥炸炸弹露出来的逻辑结束*****/
						
						/*****如果狂轰滥炸炸弹缩小的快没有了（掉地上），在该位置创建一个冲击波爆炸圈****/
						if (1 > boom->getContentSize().width)
						{
							if ( !BOSS->getBoundingBox().intersectsRect(boom->getBoundingBox()))//如果未引爆炸弹没有落在boss身上（海绵上）
							{
								Sprite* BOOM = Sprite::create("BOOM.png");
								BOOM->setPosition(boom->getPosition());
								BOOM->setContentSize(Size(1, 1));
								this->addChild(BOOM);
								_BOOMVector.pushBack(BOOM); //以上几行代码是创建爆炸圈
							}

							_BOOMSmallVector.eraseObject(boom);//以下两行代码是删除掉这个狂轰滥炸前身
							this->removeChild(boom, true);
						}
						/*****冲击波爆炸圈创建逻辑完毕*****/
					}
					/***狂轰滥炸前身（是前身！）逻辑状态更新完毕***/

					/*****狂轰滥炸冲击波的逻辑状态更新开始****/
					for (unsigned int i = 0;i < _BOOMVector.size();i++)
					{
						auto BOOM = _BOOMVector.at(i);
						BOOM->setContentSize(Size(BOOM->getContentSize().width + BOOMRADIUSADD, BOOM->getContentSize().height + BOOMRADIUSADD));
					
						if (_hero->getBoundingBox().intersectsCircle(BOOM->getPosition(),BOOM->getContentSize().width/2))//如果狂轰滥炸炸到了主角
						{
							_hero->setCool(_hero->getCool() - NORMALHURT);//那么让主角受伤
						}

						/****挨个测试所有存在普攻小兵，看是否有小兵被该狂轰滥炸炸到了****/
						for (unsigned int j = 0;j < _solderVector.size();j++)
						{
							auto solder = _solderVector.at(j);
						}
						/*****测试普攻小兵是否被炸到的逻辑结束***/
						
						/*****以下这一段代码是让冲击波双双消除，因为是偶数，不存在最后一个无限扩张的情况****/
						for (unsigned int j = 0;j < _BOOMVector.size();j++)
						{
							auto BOOM2 = _BOOMVector.at(j);
							if (BOOM2 == BOOM) break;
							float changdu;
							float wid = BOOM->getPositionX() - BOOM2->getPositionX();
							float hei = BOOM->getPositionY() - BOOM2->getPositionY();
							changdu = sqrt(wid*wid+hei*hei);
							if(changdu<BOOM->getContentSize().width/2+BOOM2->getContentSize().width/2)//如果长度小于两个冲击波的半径之和
							{
								_BOOMVector.eraseObject(BOOM2);
								this->removeChild(BOOM2, true);
								_BOOMVector.eraseObject(BOOM);
								this->removeChild(BOOM, true);
								break;
							}
						}
						/****冲击波鸳鸯式消除法逻辑结束****/

						///****冲击波单个消除法（两两相撞大的死亡）逻辑开始***/
						//for (unsigned int a = 0;a < _BOOMVector.size();a++)
						//{
						//	auto BOOM2 = _BOOMVector.at(a);
						//	if (BOOM2 == BOOM) break;
						//	float changdu;
						//	float wid = BOOM->getPositionX() - BOOM2->getPositionX();
						//	float hei = BOOM->getPositionY() - BOOM2->getPositionY();
						//	changdu = sqrt(wid*wid + hei*hei);
						//	if (changdu < BOOM->getContentSize().width / 2 + BOOM2->getContentSize().width / 2)
						//	{
						//		if (2 == _BOOMVector.size() && 0 == _BOOMSmallVector.size())//如果冲击波就剩下最后两个了
						//		{
						//			_BOOMVector.eraseObject(BOOM);
						//			this->removeChild(BOOM, true);
						//			_BOOMVector.eraseObject(BOOM2);
						//			this->removeChild(BOOM2, true);
						//			break;
						//		}
						//		else //不然就是冲击波还剩了好几个（大于2），那么可以逐个消除
						//		{
						//			if (BOOM->getContentSize().width < BOOM2->getContentSize().width)//如果boom2冲击波比boom大
						//			{
						//				_BOOMVector.eraseObject(BOOM2); //boom2死
						//				this->removeChild(BOOM2, true);
						//				break;
						//			}
						//			else//不然就是boom冲击波比较大，那么boom死
						//			{
						//				_BOOMVector.eraseObject(BOOM);
						//				this->removeChild(BOOM, true);
						//				break;
						//			}
						//		}
						//	}
						//}
						///****冲击波单个消除法逻辑结束****/
					}
					/******狂轰滥炸真身的逻辑更新结束*****/

					/*****检测boss是否满蓝了，是就释放技能****/
					if (_BOSSPowerVal >= _BOSSHP)//如果满蓝了
					{
						_BOSSPowerVal = 0;//先把boss的蓝量清零
						/*****下面两个for循环是为了把普攻子弹和天女散花清空****/
						for (int i = 0;i < _solderVector.size();i++) {
							auto solder = _solderVector.at(i);
							_solderVector.eraseObject(solder);
							this->removeChild(solder, true);
						}
						for (int i = 0;i > _flowerVector.size();i++)
						{
							auto flower = _flowerVector.at(i);
							_flowerVector.eraseObject(flower);
							this->removeChild(flower, true);
						}
						int flag;
						//if (_BOSSHP <= BOSSHP / 4)//如果boss生命值太低了
						//	flag = 3; //那么flag为3，意味着boss一直释放终极技能
						//else  //不然boss随机释放技能把
							flag = CCRANDOM_0_1() * 4;
							if (1000 >= _BOSSHP) flag = 2;

						switch (flag)
						{
						case 0://释放流星雨技能
						{
							for (unsigned int i = 0;i < METEORSHOWERNUM;i++)
							{
								float startPosX, startPosY, endPosX, endPosY;
								startPosX = CCRANDOM_0_1()*WID8 - WID8;//以下两行代码决定陨石的初始位置
								startPosY = CCRANDOM_0_1()*HEI8 * 3-HEI8;
								endPosX = CCRANDOM_0_1()*WID8 + WID8;//决定陨石的目标位置
								endPosY = CCRANDOM_0_1()*HEI8 * 3-HEI8;
								Sprite* stone = Sprite::create("obstacle.png");
								stone->setPosition(startPosX, startPosY);
								int randFlag = CCRANDOM_0_1()*(METEORSHOWERNUM + STONERAND);
								if (1 >= randFlag)//这两行代码将这颗陨石设置成危险陨石（被射爆后会散发天女散花）
								{
									stone->setPositionZ(1);
								}
								auto moveTo = MoveTo::create(5.0, Vec2(endPosX, endPosY));
								stone->runAction(moveTo);
								addChild(stone,99);
								_stoneVector.pushBack(stone);
							}
						}
							break;
						case 1://释放狂轰滥炸技能
						{
							for (unsigned int i = 0;i < BOOMNUM;i++)
							{
								float randSize = CCRANDOM_0_1() * (BOOMSMALLMAXSIZE-BOOMSMALLMINSIZE);//这个决定爆炸圈前身的隐形时间
								randSize += BOOMSMALLMINSIZE;//这个和上面代码，决定爆炸圈前身的总大小
								float posX = CCRANDOM_0_1()*WID8;//以下两行代码决定该爆炸圈前身的随机位置
								float posY = CCRANDOM_0_1()*HEI8;
								Sprite* boom = Sprite::create("zhadan.png");
								boom->setColor(Color3B::WHITE);
								boom->setContentSize(Size(randSize,randSize));
								boom->setPosition(posX, posY);
								boom->setVisible(false);
								addChild(boom,99);
								_BOOMSmallVector.pushBack(boom);
							}
						}
							break;
						case 3: //释放回光返照技能
						{
							_BOSSHP = int((HPRECOVER + _BOSSHP) / BOSSHP)*BOSSHP;
							if (_BOSSHP < BOSSHP - HPRECOVER)
								_BOSSHP += HPRECOVER;
							else _BOSSHP = BOSSHP;
							_BOSSPowerVal += POWERRECOVER;
						}
							break;
						case 2://释放终极技能
						{
							BOSS->stopAllActions();
							/****boss释放大招了，清除其他技能或杂物*****/
							{
								for (int i = 0;i < _solderVector.size();i++)
								{
									auto solder = _solderVector.at(i);
									this->removeChild(solder, true);
									_solderVector.eraseObject(solder);
								}
								for (int i = 0;i < _flowerVector.size();i++)
								{
									auto flower = _flowerVector.at(i);
									this->removeChild(flower, true);
									_flowerVector.eraseObject(flower);
								}
								for (int i = 0;i < _stoneVector.size();i++)
								{
									auto stone = _stoneVector.at(i);
									this->removeChild(stone, true);
									_stoneVector.eraseObject(stone);
								}
								for (int i = 0;i < _BOOMSmallVector.size();i++)
								{
									auto boom = _BOOMSmallVector.at(i);
									this->removeChild(boom, true);
									_BOOMSmallVector.eraseObject(boom);
								}
								for (int i = 0;i < _BOOMVector.size();i++)
								{
									auto BOOM = _BOOMVector.at(i);
									this->removeChild(BOOM, true);
									_BOOMVector.eraseObject(BOOM);
								}
							}
							/*****杂物清除完毕****/
							_speedX = CCRANDOM_0_1();
							_speedY = SKILLSTARTSPEED - _speedX;
							if (WID8 / 2 <= BOSS->getPositionX())//如果boss在屏幕的右半边
								_speedX = -_speedX; //那么boss终极技能的初始方向向左移动
							if (HEI8 / 2 <= BOSS->getPositionY())//如果boss在屏幕的上面
								_speedY = -_speedY;//那么boss终极技能的初始方向向下移动
							_skillFlagNum = 3; //设置boss碰壁的最大次数（碰完了大招结束）
							_isSkill = true; //设置boss为开大招状态
							
							_falseBOSS->setPosition(BOSS->getPosition());//让boss假身停留在boss真身上面来迷惑敌人
							BOSS->setOpacity(int(_BOSSHP / 1000)*11);
						}
							break;
						}
					}
					/****测蓝与释放技能的逻辑结束****/
				}
				else//boss在释放终极技能
				{
					BOSS->setPositionX(BOSS->getPositionX() + _speedX);
					BOSS->setPositionY(BOSS->getPositionY() + _speedY);

					/****碰壁精致的返回逻辑开始****/
					{
						bool flagX = false;
						bool flagY = false;//这两个用于让boss回头
						auto posX = BOSS->getPositionX();//先获取超出屏幕的boss的X轴位置
						if (0 > posX)//如果boss超出了屏幕左边
						{
							SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
							auto indexY = _speedY*posX / _speedX; //获取boss应该退回的Y轴量
							BOSS->setPosition(0, BOSS->getPositionY() - indexY); //重新设置boss在最左边以及退回相应的Y轴量
							flagX = true;
							--_skillFlagNum;
							if (0 >= _skillFlagNum)//如果碰壁次数达到3次
							{
								_isSkill = false;//boss的终极技能释放完毕
								_falseBOSS->setPosition(-200, -200);
								BOSS->setOpacity(255);
							}
						/*	else
							{
								auto* rotateTo = RotateBy::create(0.1, 360 * 10);
								_hero->runAction(rotateTo);
							}*/
						}
						else if (WID8 < posX)//如果boss超出了屏幕右边
						{
							SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
							auto indexY = _speedY*(posX - WID8) / _speedX;
							BOSS->setPosition(WID8, BOSS->getPositionY() - indexY);
							flagX = true;
							--_skillFlagNum;
							if (0 >= _skillFlagNum)//如果碰壁次数达到3次
							{
								_isSkill = false;//boss的终极技能释放完毕
								_falseBOSS->setPosition(-200, -200);
								BOSS->setOpacity(255);
							}
							/*else
							{
								auto* rotateTo = RotateBy::create(0.1, 360 * 10);
								_hero->runAction(rotateTo);
							}*/
						}
						auto posY = BOSS->getPositionY();
						if (0 > posY)//如果boss超出了屏幕下面
						{
							SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
							auto indexX = _speedX*posY / _speedY;
							BOSS->setPosition(BOSS->getPositionX() - indexX, 0);
							flagY = true;
							--_skillFlagNum;
							if (0 >= _skillFlagNum)//如果碰壁次数达到3次
							{
								_isSkill = false;//boss的终极技能释放完毕
								_falseBOSS->setPosition(-200, -200);
								BOSS->setOpacity(255);
							}
							/*else
							{
								auto* rotateTo = RotateBy::create(0.1, 360 * 10);
								_hero->runAction(rotateTo);
							}*/
						}
						else if (HEI8 < posY)//如果boss超出了屏幕上面
						{
							SimpleAudioEngine::getInstance()->playEffect("BOOM2.mp3");
							auto indexX = _speedX*(posY - HEI8) / _speedY;
							BOSS->setPosition(BOSS->getPositionX() - indexX, HEI8);
							flagY = true;
							--_skillFlagNum;
							if (0 >= _skillFlagNum)//如果碰壁次数达到3次
							{
								_isSkill = false;//boss的终极技能释放完毕
								_falseBOSS->setPosition(-200, -200);
								BOSS->setOpacity(255);
							}
						/*	else
							{
								auto* rotateTo = RotateBy::create(0.1, 360 * 10);
								_hero->runAction(rotateTo);
							}*/
						}
						if (flagX) _speedX = -_speedX;
						if (flagY) _speedY = -_speedY;
					}
					/****碰壁精致的返回逻辑结束****/
					
					_speedX *= SPEEDADD;
					_speedY *= SPEEDADD;
				}
				/****boss释放终极技能的代码段结束****/
			}
			else /****boss被打死，开始自爆****/
			{
				if (0 >= _deadBOOM.size())//如果还没有自爆开花过
				{
					for (int i = 0;i < 50;i++)
					{
						Sprite* deadBOOM = Sprite::create("BOOM.png");
						deadBOOM->setPosition(BOSS->getPositionX() + BOSS->getContentSize().width*CCRANDOM_0_1() - BOSS->getContentSize().width / 2,
							BOSS->getPositionY() + BOSS->getContentSize().width*CCRANDOM_0_1() - BOSS->getContentSize().width / 2);
						deadBOOM->setContentSize(Size(1, 1));
						DelayTime* delayTime = DelayTime::create(CCRANDOM_0_1()*5.0F);
						auto scaleTo = ScaleTo::create(0.4, 50);
						Sequence* sequence = Sequence::create(delayTime, scaleTo, NULL);
						deadBOOM->runAction(sequence);
						this->addChild(deadBOOM, 108);
						_deadBOOM.pushBack(deadBOOM);
					}
				}
				else if (1 <= _deadBOOM.size() && 9 >= _deadBOOM.size()) //自爆开花的向量表还剩几朵花，可以不用自爆了，让boss上路，游戏获得最终胜利！
				{
					for (int i = 0;i < _deadBOOM.size();i++)
						_deadBOOM.at(i)->setVisible(false);
					for (int i = 0;i < 4;i++)
						_sea[i]->setVisible(false);
					BOSS->setVisible(false);
					_hero->setVisible(false);

					_text->setVisible(true);
					_text->setPositionX(_hero->getPositionX());
				}
				else for (unsigned int i = 0;i < _deadBOOM.size();i++) //循环自爆开花向量表，消除若干爆开的花
					{
						auto deadBOOM = _deadBOOM.at(i);
						if (50 <= deadBOOM->getScaleX())
						{
							this->removeChild(deadBOOM, true);
							_deadBOOM.eraseObject(deadBOOM);
						}
					}
			}
			/****boss自爆的代码段结束*****/
			/****boss死亡自爆开花的逻辑处理完毕****/
				;
		}
		else//不然就是在暂停时，因为流星雨等还能活动，所以要测试是否撞死了主角
		{
			for (int i = 0;i < _flowerVector.size();i++) {
				auto flower = _flowerVector.at(i);
				if (_hero->getBoundingBox().intersectsRect(flower->getBoundingBox())) {
					_GameState = -1;
				}
			}

			for (int i = 0;i < _stoneVector.size();i++) {
				auto stone = _stoneVector.at(i);
				if (_hero->getBoundingBox().intersectsRect(stone->getBoundingBox())) {
					_GameState = -1;
				}
			}
		}
		

	}
		break;
	case 1://游戏获得最终胜利
	{
	}
		break;
	case -1:
	{//创建一个按钮，可以返回调用开始游戏
		auto next = Button::create("button.png");
		next->setTitleText("NEW");
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
	case -2:
		break;
	case -3:
		break;

	}
}