#include "MainMenuScene.h"
#include "NetworkChooseScene.h"
#include "GameScene.h"
#include "Definitions.h"
#include "AboutScene.h"
//#include "SonarFrameworks.h"

#include "GamePrepareScene.h"

#include <cstdlib>

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);

    return scene;
}

bool MainMenuScene::init()
{
    if (!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    backgroundImage = Sprite::create("backgroundw.png");
    backgroundImage->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundImage);

    currentColor = (rand()%5 + 1) - 1;
    setColors(currentColor);

   	this->circleInitiate();

    if(UserDefault::sharedUserDefault()->getStringForKey("firstTimeLaunch") == "")
    {
    	UserDefault::sharedUserDefault()->setStringForKey("firstTimeLaunch", "false");
    	Sprite* aboutSprite = Sprite::create("about.png");
    	aboutSprite->setTag(23);
    	aboutSprite->setPosition(Vec2(visibleSize.width + origin.x - 50, cocos2d::Director::getInstance()->getVisibleSize().height - 50));
    	aboutSprite->setScale(0.27f);
    	this->addChild(aboutSprite);
    }

   	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if(this->getChildByTag(23) != NULL)
		{
			Sprite* aboutSprite = static_cast<Sprite*>(this->getChildByTag(23));

			if(aboutSprite->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
			{
				aboutSprite->runAction(Sequence::create(FadeTo::create(0.5f, 0), RemoveSelf::create(false), nullptr));
				std::string txt2 = "TO PLAY, YOU HAVE TO BE IN A LAN CONNECTION WITH YOUR FRIENDS\nFOR NAVIGATION USE SWIPES\nFOR SELECTION JUST CLICK AND HOLD";
				Label* guideLabel = Label::create(txt2,"arial",GameManager::getInstance()->getLabelPixels() * 30 / 100);
				guideLabel->setAlignment(TextHAlignment::CENTER);
				guideLabel->setPosition(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + visibleSize.height/4);
				guideLabel->setOpacity(0);
				this->addChild(guideLabel);
				guideLabel->runAction(Sequence::create(FadeTo::create(0.5f, 1000), TintBy::create(10, 0, 0, 0), FadeTo::create(0.5f, 0), RemoveSelf::create(false), nullptr));
				aboutSprite->setTag(aboutSprite->getTag() + 1);

				return true;
			}
		}

		initialTouchPos[0] = touch->getLocation().x;
		currentTouchPos[0] = touch->getLocation().x;
		initialTouchPos[1] = touch->getLocation().y;
		currentTouchPos[1] = touch->getLocation().y;

		isTouchDown = true;

		return true;
	};

	listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		currentTouchPos[0] = touch->getLocation().x;
		currentTouchPos[1] = touch->getLocation().y;
	};

	listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		isTouchDown = false;
	};
	listener->onTouchCancelled = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		isTouchDown = false;
	};
   	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    currentMenuSelection = 0;

    playSprite = Sprite::create();
    playSprite->setTag(1);
	Label* playSpriteLabel = Label::create("PLAY", "arial", GameManager::getInstance()->getLabelPixels());
	playSprite->addChild(playSpriteLabel);
	playSprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(playSprite);

	infoSprite = Sprite::create();
	infoSprite->setTag(3);
	Label*infoSpriteLabel = Label::create("ABOUT", "arial", GameManager::getInstance()->getLabelPixels());
	infoSprite->addChild(infoSpriteLabel);
	infoSprite->setPosition(Vec2(visibleSize.width/2 + origin.x, -100));
	this->addChild(infoSprite);

	exitSprite = Sprite::create();
	exitSprite->setTag(2);
	Label*exitSpriteLabel = Label::create("EXIT", "arial", GameManager::getInstance()->getLabelPixels());
	exitSprite->addChild(exitSpriteLabel);
	exitSprite->setPosition(Vec2(visibleSize.width/2 + origin.x, -100));
	this->addChild(exitSprite);

	menuSprites.pushBack(playSprite);
	menuSprites.pushBack(infoSprite);
	menuSprites.pushBack(exitSprite);

    MenuItemImage* m3 = MenuItemImage::create("rectFull.png", "rectFull.png");
    m3->setPosition(Point(0, 0));
    m3->setScale(0.2f);
    MenuItemImage* m2 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
    m2->setPosition(Point(-50, 0));
    m2->setScale(0.2f);
    MenuItemImage* m4 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
    m4->setPosition(Point(50, 0));
    m4->setScale(0.2f);
    MenuItemImage* m1 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	m1->setPosition(Point(-100, 0));
	m1->setScale(0.2f);
	MenuItemImage* m5 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	m5->setPosition(Point(100, 0));
	m5->setScale(0.2f);

    Menu* menu1 = Menu::create(m1, m2, m3, m4, m5, NULL);
    menu1->setPosition(Point(visibleSize.width / 2, m1->getContentSize().height * 0.4));
    this->addChild(menu1);

    this->menuBottom.pushBack(m3);
    this->menuBottom.pushBack(m4);
    this->menuBottom.pushBack(m5);
    this->menuBottom.pushBack(m1);
    this->menuBottom.pushBack(m2);

    menuBottomI = 0;

    MenuItemImage* r1 = MenuItemImage::create("rectFull.png", "rectFull.png");
    r1->setPosition(Point(0, 50));
    r1->setScale(0.2f);
    MenuItemImage* r2 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
    r2->setPosition(Point(0, 0));
    r2->setScale(0.2f);
    MenuItemImage* r3 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
    r3->setPosition(Point(0, -50));
    r3->setScale(0.2f);

	Menu* menu2 = Menu::create(r1, r2, r3, NULL);
    menu2->setPosition(Point(visibleSize.width - r1->getContentSize().width * 0.4, visibleSize.height / 2));
    this->addChild(menu2);

    this->menuRight.pushBack(r1);
    this->menuRight.pushBack(r2);
    this->menuRight.pushBack(r3);

    menuRightI = 0;

    this->scheduleUpdate();
    this->resume();

    //SonarCocosHelper::AdMob::showFullscreenAd();

#ifndef NDEBUG
    debugPlaySprite = Sprite::create();
    debugPlaySprite->setTag(0);
    Label* debugPlayLabel = Label::create("DEBUG", "arial", GameManager::getInstance()->getLabelPixels());
    debugPlaySprite->addChild(debugPlayLabel);
    debugPlaySprite->setPosition(Vec2(visibleSize.width/2 + origin.x, -100));
    this->addChild(debugPlaySprite);
	menuSprites.pushBack(debugPlaySprite);

    MenuItemImage* r4 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	r4->setPosition(Point(0, -100));
	r4->setScale(0.2f);
	menu2->addChild(r4);
	menu2->setPosition(menu2->getPosition() + Vec2(0, 25));

    this->menuRight.pushBack(r4);
#endif

    return true;
}

void MainMenuScene::update(float deltaTime)
{
	if(isTouchDown)
	{
		circleScaler += 1 * deltaTime;
		if(circleScaler >= 0.6f)
		{
			circleScaler = 0.6f;

			switch(menuSprites.at(currentMenuSelection)->getTag())
			{
			case 0:
				GameManager::getInstance()->setOpponentScreenDetails(Director::getInstance()->getVisibleSize().width / 1.2f, Director::getInstance()->getVisibleSize().height / 1.2f, GameManager::getInstance()->getDPI());
				GameScene::debugMode = true;
				Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, GameScene::createScene()));
				break;
			case 1:
				Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, NetworkChooseScene::createScene()));
				break;
			case 2:
				Director::getInstance()->end();
				break;
			case 3:
				Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, AboutScene::createScene()));
			}
		}

		fullCircle->setScale(circleScaler);

		if(initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			currentColor += 1;
			if(currentColor > 4)
				currentColor = 0;

			setColors(currentColor);
			menuHandler(true, &menuBottom, &menuBottomI);
		}

		else if(initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			currentColor -= 1;
			if(currentColor < 0)
				currentColor = 4;

			setColors(currentColor);
			menuHandler(false, &menuBottom, &menuBottomI);
		}

		else if(initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			menuHandler(false, &menuRight, &menuRightI);
			mainMenuHandler(false, &menuSprites, &currentMenuSelection, menuSprites.size());
		}

		else if(initialTouchPos[1] - currentTouchPos[1] < -visibleSize.width * 0.05f) //meniuto sliza nadolo
		{
			isTouchDown = false;

			menuHandler(true, &menuRight, &menuRightI);
			mainMenuHandler(true, &menuSprites, &currentMenuSelection, menuSprites.size());
		}
	}
	else
	{
		circleScaler -= 1 * deltaTime;
		if(circleScaler <= 0)
			circleScaler = 0;
		fullCircle->setScale(circleScaler);
	}
}

void MainMenuScene::setColors(int colorNumber)
{
	switch(colorNumber)
	{
	case 0:
		changeShipColor("blue", 78, 191, 207); break;
	case 1:
		changeShipColor("red", 190, 18, 18); break;
	case 2:
		changeShipColor("yellow", 235, 226, 78); break;
	case 3:
		changeShipColor("purple", 208, 100, 180); break;
	case 4:
		changeShipColor("green", 62, 199, 156); break;
	}
}

void MainMenuScene::changeShipColor(std::string shipColorString, int red, int green, int blue)
{
	GameManager::getInstance()->setMyShipTypeWithString(shipColorString);
	backgroundImage->runAction(TintTo::create(0.5f, red, green, blue));
}

void MainMenuScene::GoToGameScene(Ref* sender)
{
	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, gameScene));
}

void MainMenuScene::GoToMultiplayerScene(cocos2d::Ref* sender)
{
	auto networkScene = NetworkChooseScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, networkScene));
}

void MainMenuScene::GoToInfoScene(Ref* sender)
{

}

void MainMenuScene::ExitGame(Ref* sender)
{
	Director::getInstance()->end();
}
