#include "Level7.h"
#include "cmath"
#include <iostream>
USING_NS_CC;

cocos2d::Scene* Level7::createScene()
{
	return Level7::create();
}

bool Level7::init()
{
	{
		if (!Scene::init()) return false;

		_taskCount = 0;
		_GameState = 0;
		_isStopRota = false;
		_rotaTime = ROTATIME;
		_rotaAdd = 1;
		
		auto background = Sprite::create("BG.png");//首先添加一个红色背景图片
		background->setContentSize(Size(400, 800));
		background->setPosition(WID7 + background->getContentSize().width / 2, background->getContentSize().height / 2);
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
				Director::getInstance()->replaceScene(Level7::createScene());
			}
		});
		refurbish->setPosition(Vec2(1050, 800 - refurbish->getContentSize().width / 2));
		addChild(refurbish);



		SimpleAudioEngine::getInstance()->playBackgroundMusic("7.mp3", true);//播放背景音乐

		_hero = Hero::createWithSpriteName(HEROSPEED7, 0);//初始化主角
		_hero->setPosition(WID7/2-200, HEI7-25);
		addChild(_hero);
	/*	_bullet = Bullet::createWithSpriteName(16, _hero->getRotation(), "bullet.png");
		_bullet->setPosition(-100, -100);
		addChild(_bullet);

		_bullet = Bullet::createWithSpriteName(BUSP7, _hero->getRotation(), "bullet.png");
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
				_bullet = Bullet::createWithSpriteName(BUSP7, _hero->getRotation(), "icebullet.png");
				_bullet->setPosition(_hero->getPosition());
				addChild(_bullet);

				_hero->setFire(true);
			}
		}
	});
	fire->setPosition(Vec2(WIDTH - fire->getContentSize().width / 2, HEIGHT - fire->getContentSize().height / 2));
	addChild(fire);

	_realEnemy = Sprite::create("ppp.png");//添加一个真敌人
	_realEnemy->setPosition(400,400);
	addChild(_realEnemy);//真敌人添加结束

	_falseEnemy = Sprite::create("snowmonster.png");//添加一个假敌人
	_falseEnemy->setPosition(900,700);
	setFlasePos();
	addChild(_falseEnemy);//假敌人添加结束

	_layer = Sprite::create("BG.png");//添加一束激光
	_layer->setContentSize(Size(10, 1300));
	_layer->setAnchorPoint(Vec2(0.5, 0));
	_layer->setPosition(_realEnemy->getPosition());
	addChild(_layer);//激光添加结束

	//auto rota = RotateBy::create(_rotaTime, 360);
	//CallFunc* callFunc = CallFunc::create([&] {
	//	_isStopRota = true;
	//});
	//Sequence* sequence = Sequence::create(rota, callFunc, NULL);
	//_layer->runAction(sequence);

	////调试用的代码，用来显示坦克的角度，完工删除
	//float XX = _hero->getPositionX() - _realEnemy->getPositionX();
	//float YY = _hero->getPositionY() - _realEnemy->getPositionY();
	//float pp = tan(XX / YY) * 180;
	//std::string s = "()";
	//s.append(StringUtils::format("%f", pp));

	//ext = Text::create(s, "Arial", 64);
	//ext->setPosition(Vec2(800, 400));
	//addChild(ext,99999);
	////完工把以上代码删除

	return true;
}

void Level7::update(float delta)
{
	switch (_GameState) //检测游戏是否进入暂停状态
	{
	case 0:
		if (!_pause)//如果还没有进入暂停状态
		{
			removeChild(ext);
			float XX = _hero->getPositionX() - _realEnemy->getPositionX();
			float YY = _hero->getPositionY() - _realEnemy->getPositionY();
			float pp;

			/*设置一个文本，用来显示角度，调试用，完工了删除这些代码*/
		/*	pp = transAngle(_hero->getPosition(),_realEnemy->getPosition());
			std::string s = "()";
			s.append(StringUtils::format("hero:%f", pp));
			s.append(StringUtils::format("layer:%f", _layer->getRotation()));

			 ext = Text::create(s, "Arial", 64);
			ext->setPosition(Vec2(800, 400));
			addChild(ext);*/
			//完工了把以上代码删除

			_layer->setRotation(_layer->getRotation() + _rotaAdd); //激光会越转越快
			if (360 < _layer->getRotation()) _layer->setRotation(_layer->getRotation() - 360);//因为转过一圈后是360+，所以这里要调整小于360
			_rotaAdd *= ROTAADD; //下一次激光旋转要增加的幅度稍微变大了一点

			if (isHurt(XX,YY)) //如果激光扫到了主角
			{
				/*_GameState = -1;
				_layer->stopAllActions();*/

				////以下这段代码用于转换真假长老
				//_rotaTime = ROTATIME;
				//Vec2 temp = _realEnemy->getPosition();
				//_realEnemy->setPosition(_falseEnemy->getPosition());
				//_layer->setPosition(_realEnemy->getPosition());
				//_falseEnemy->setPosition(temp);
				////转换真假长老结束
				_hero->setContentSize(Size(80, 80)); //让主角变大一下，表示被激光射到了
				_GameState = -1;
			}

				if (_hero->getDirection()) //如果英雄处于前进状态
				{
					_hero->tankRun(); //让主角前进一下
					if (_hero->isInfringeBorder(WID7, HEI7, -_hero->getContentSize().width / 2)) //如果主角前进后越界了
						_hero->tankBack(); //把主角重新拉回来
					if (_hero->getBoundingBox().intersectsRect(_realEnemy->getBoundingBox()))//如果英雄撞到了真敌人
					{
						_hero->tankBack(); //把主角拉回来
						switch (_hero->getDirection()) //按照英雄的行走方向让英雄漂移，漂移的速度跟主角前进速度一致
						{
						case 1:
						case 3:
							if (_hero->getPositionX() >= _realEnemy->getPositionX())
								_hero->setPositionX(_hero->getPositionX() + HEROSPEED7);
							else _hero->setPositionX(_hero->getPositionX() - HEROSPEED7);
							break;
						case 2:
						case 4:
							if (_hero->getPositionY() >= _realEnemy->getPositionY())
								_hero->setPositionY(_hero->getPositionY() + HEROSPEED7);
							else _hero->setPositionY(_hero->getPositionY() - HEROSPEED7);
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
						if (_bullet->isInfringeBorder(WID7, HEI7, _bullet->getContentSize().height / 2)) //如果子弹超越了边界
						{
							removeFlag1 = true; //把是否应该删除子弹的标记设为是
							goto removeBullet1; //然后去删除子弹
						}//如果子弹越界了，子弹消失
						if (_bullet->getBoundingBox().intersectsRect(_realEnemy->getBoundingBox()))//如果子弹击中了真敌人
						{
							if (++_taskCount >= TASKNUM7)//如果击毙敌人达到关卡任务量
							{
								_GameState = 1;
							}
							else
							{
								_realEnemy->setPosition(_falseEnemy->getPosition());//让假身敌人变成真敌人
								_layer->setPosition(_falseEnemy->getPosition());//以上两行代码调整敌人和激光的位置
								//_rotaTime = ROTATIME;//让激光旋转的时间重新归位，免得接上一个更快的时间来转
								_rotaAdd = 1.0; //重新恢复激光旋速要增加的幅度
								setFlasePos();//设置一个新假身敌人的位置
								removeFlag1 = true; //把是否应该删除子弹的标记设为是
								goto removeBullet1; //然后去删除子弹
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

		}
		break;
	case 1: //这个分支代表玩家达到了过关条件
	{ //这里必须要加上花括号，不然在某些编译器中，会因为局部变量作用域的问题编译报错
		//把关卡数设置为1，表示该创建进入第二关了
		{
			if (7 > UserDefault::getInstance()->getIntegerForKey("level"))
			{
				UserDefault::getInstance()->setIntegerForKey("level", 7);
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
		Text* failText = Text::create("hero be killed by layer", "Arial", 64);
		failText->setRotation(270);
		failText->setPosition(Vec2(400, 400));
		addChild(failText);
		_GameState = 2;//这个是防止一直停止播放背景音乐，如果一直停止的话，再次重新加载播放BGM的话，会静音的

		float XX = _hero->getPositionX() - _realEnemy->getPositionX();
		float YY = _hero->getPositionY() - _realEnemy->getPositionY();
		float pp = tan(XX / YY) * 180;
		std::string s = "()";
		s.append(StringUtils::format("%f", pp));
		
		Text* ext = Text::create(s, "Arial", 64);
		ext->setPosition(Vec2(800, 400));
		addChild(ext);
	}
	break;
	case -2:
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Text* failText2 = Text::create("enemy be killed by bullet", "Arial", 64);
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

bool Level7::isHurt(float XX, float YY) //用来检测激光是否扫射到了坦克
{
	float layerAngel = _layer->getRotation();//获取激光的角度
	float heroAngle = transAngle(_hero->getPosition(), _realEnemy->getPosition());//获取坦克与敌人的角度
	return (layerAngel > heroAngle-3.1f && layerAngel < heroAngle+3.1f);
}

float Level7::transAngle(Vec2 origin, Vec2 target)//转换角度用，第一个是激光源，第二个参数是主角
{
	float x = origin.x - target.x;
	float y = origin.y - target.y;
	if (x >= 0.0) //坦克相对激光敌人而言，是在它的右半区域
	{
		if (y >= 0.0) //坦克如果是在敌人右上角（相对target而言）
		{
			return atan(x / y) * 180 / PI;
		}
		else // 不然坦克就是在右下角（相对target而言）
		{
			return 90 - atan(y / x) * 180 / PI;
		}
	}
	else //坦克相对激光敌人而言，是在它的左半区域
	{
		if (y >= 0.0)//坦克如果在敌人左上角（相对target而言）
		{
			return 270.0 - atan(y / x) * 180 / PI;
		}
		else //不然坦克就是在左下角（相对target而言）
		{		
			return 180 + atan(x / y) * 180 / PI;
		}
	}
}

void Level7::setFlasePos() //设置假身敌人的位置
{
start:
	float distance; //这个代表真假敌人相距的最远距离，水平线与垂直线的叠加量
	distance = MAXDIS - CCRANDOM_0_1()*OFFSET;  //然后适当给玩家降低一点难度，缩短一点点距离
	float horizontal; //这个是真假敌人水平线的距离
	float vertical;  //这个是真假敌人垂直线的距离
	horizontal = CCRANDOM_0_1()*distance;  //随机设置一个真假敌人水平线的距离
	vertical = distance - horizontal; //然后总距离减去水平线距离就是真假敌人的垂直线距离
	int random; //用一个随机值，来表示水平或垂直距离是正还是负数
	random = CCRANDOM_0_1() * 2;
	if (random ) horizontal = -horizontal;
	random = CCRANDOM_0_1() * 2;
	if (random) vertical = -vertical;
	horizontal = _falseEnemy->getPositionX() + horizontal; //设置新假人方位的水平值
	vertical += _falseEnemy->getPositionY(); //设置新假人方位的垂线值
	if (horizontal<125 || horizontal>WID7 - 125 || vertical<125 || vertical>HEI7 - 125) //如果新假人的新方位不合理，比如越界或者靠近四个边
		goto start; //那么重新弄一个新的方位值
	_falseEnemy->setPosition(Vec2(horizontal, vertical));
	return;
}