#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "BOSS.h"
using namespace CocosDenshion;
using namespace ui;
using namespace std;
USING_NS_CC;

int HelloWorld::level = 7;

cocos2d::Scene* (*pCreateGame[8])();//函数指针数组，用于创建对应的关卡游戏
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	level = UserDefault::getInstance()->getIntegerForKey("level");
	//level = 7;//这一句是选关用的，成品一定要删除这句代码


	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...


	/****增加一个提取故事文本的代码段****/
	{
		auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");

		string storyStr = "story";
		storyStr.append(StringUtils::format("%d", level+1)); 

		auto storyText = ((CCString*)chnStrings->objectForKey(storyStr))->getCString();

		_bgStory = Text::create(storyText, "", 30);
		_bgStory->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		_bgStory->setRotation(270);
		addChild(_bgStory);
	}
	/****提取故事文本的代码段结束****/

	/****增加一个显示第几关的文本代码段*****/
	{
		_levelTxt = Text::create();
		_levelTxt->setText(StringUtils::format("%d", level+1));
		_levelTxt->setPosition(Vec2(1350, HEI*0.225));
		_levelTxt->setRotation(270);
		_levelTxt->setScale(2.55);
		addChild(_levelTxt);
	}
	/****显示第几关的代码段结束*****/

	/****增加一个控制故事文本上与下的按钮的代码段****/
	{
		Button* upBtn = Button::create("up.png");
		upBtn->setRotation(315);
		upBtn->setScale(0.4);
		upBtn->setPosition(Vec2(1350, HEIGHT*0.3));
		upBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::ENDED == type)
			{
				_bgStory->setPositionX(_bgStory->getPositionX() - 100);
				int num = UserDefault::getInstance()->getIntegerForKey("level");
				if (level + 1 <= num)
				{
					level++;
				}		
				_levelTxt->setText(StringUtils::format("%d", level + 1));
				auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
				string storyStr = "story";
				storyStr.append(StringUtils::format("%d", level + 1));
				auto storyText = ((CCString*)chnStrings->objectForKey(storyStr))->getCString();
				_bgStory->setText(storyText);
			}
		});
		addChild(upBtn, 2);

		Button* downBtn = Button::create("up.png");
		downBtn->setRotation(135);
		downBtn->setScale(0.4);
		downBtn->setPosition(Vec2(1350, HEIGHT*0.15));
		downBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::ENDED == type) 
			{
				_bgStory->setPositionX(_bgStory->getPositionX() + 100);
				if (level - 1 >= 0)
				{
					level--;
				}
				_levelTxt->setText(StringUtils::format("%d", level + 1));

				auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
				string storyStr = "story";
				storyStr.append(StringUtils::format("%d", level + 1));
				auto storyText = ((CCString*)chnStrings->objectForKey(storyStr))->getCString();
				_bgStory->setText(storyText);
			}
		});
		addChild(downBtn);
	}
	/****控制故事文本上与下的按钮的代码段结束****/
	


	/*	auto downBtn = Sprite::create("down.png");
		downBtn->setPosition(Vec2(1300, HEIGHT*0.4));
		addChild(downBtn);

		auto downListener = EventListenerTouchOneByOne::create();
		downListener->onTouchBegan = [](Touch* touch, Event* event) {

			return true;
		};
		downListener->onTouchMoved = [&](Touch* touch, Event* event) {
			auto target = event->getCurrentTarget();
			auto location = touch->getLocation();
			auto locationInNode = target->convertToNodeSpace(location);
			Size s = target->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(locationInNode))
			{
				_sprite->setPositionY(_sprite->getPositionY() + 1);
			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(downListener, downBtn);*/

	/*****增加一个介绍作者的文本的代码段****/
	{
		_authorSprite = Sprite::create("author.png");//添加一个介绍作者的图片，先隐藏
		_authorSprite->setPosition(600, _authorSprite->getContentSize().width / 2);
		_authorFlag = false;
		_authorSprite->setVisible(_authorFlag);
		addChild(_authorSprite, 99);
	}
	/****介绍作者的文本的代码段结束*****/

     /*****增加一个开始按钮的代码段****/
	{
		auto start = Button::create("button.png");
		start->setTitleText("start");
		start->setTitleFontSize(32);
		start->setRotation(270);
		start->setPosition(Vec2(1350,400));
		start->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::ENDED == type) {
				Director::getInstance()->replaceScene(HelloWorld::createLevelGame());
			}
		});
		addChild(start);
	}
	/****开始按钮的代码段结束****/

	/****增加一个隐藏作者的按钮的代码段*****/
	{
		auto  author = Button::create("button.png");
		author->setTitleText("author");
		author->setTitleFontSize(32);
		author->setRotation(270);
		author->setPosition(Vec2(1350, 700));
		author->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			if (Widget::TouchEventType::ENDED == type) {
				_authorFlag = !_authorFlag;
				_authorSprite->setVisible(_authorFlag);
			}
		});
		addChild(author);
	}
	/*****隐藏作者的按钮的逻辑结束****/

		/*SimpleAudioEngine::getInstance()->playBackgroundMusic("1.mp3");*/
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

cocos2d::Scene* HelloWorld::createLevelGame()
{
	switch (level) 
	{
	case 0://第一关的创建
		return Level1::createScene();
		break;

	case 1://第二关的创建
		return Level2::createScene();
		break;

	case 2://第三关的创建
		return Level3::createScene();
		break;

	case 3://第四关的创建
		return Level4::createScene();
		break;

	case 4://第五关的创建
		return Level5::createScene();
		break;

	case 5://第六关的创建
		return Level6::createScene();
		break;

	case 6://第七关的创建
		return Level7::createScene();
		break;

	case 7://第八关的创建
		return Level8::createScene();
		break;
	}
	return nullptr;
}
void HelloWorld::update(float delta)
{
	
}
