#include "NetworkChooseScene.h"
//#include "ClientConnectScene.h"
#include "Definitions.h"
#include "ServerWaitingScene.h"
#include "ClientWaitingScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* NetworkChooseScene::createScene()
{
	auto scene = Scene::create();
    auto layer = NetworkChooseScene::create();
    scene->addChild(layer);

    return scene;
}

bool NetworkChooseScene::init()
{
    if (!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	this->setKeypadEnabled(true);
	EventListenerKeyboard* elk = EventListenerKeyboard::create();
	elk->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(elk, this);

    if(!IsNetworkConnected_CPP())
    {
    	Label* infoLabel = Label::create("CONNECT TO A NETWORK \nAND \nTRY AGAIN", "arial", GameManager::getInstance()->getLabelPixels() * 80 / 100);
    	infoLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    	infoLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    	this->addChild(infoLabel);

    	return true;
    }

	serverStarted = false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
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

	circleInitiate();

	currentMenuSelection = 0;

	startServer = Sprite::create();
	startServer->setTag(1);
	Label* startServerSpriteLabel = Label::create("START GAME", "arial", GameManager::getInstance()->getLabelPixels());
	startServer->addChild(startServerSpriteLabel);
	startServer->setPosition(Vec2(visibleSize.width/2 + origin.x, -100));
	this->addChild(startServer);

	joinServer = Sprite::create();
	joinServer->setTag(0);
	Label* joinServerSpriteLabel = Label::create("JOIN GAME", "arial", GameManager::getInstance()->getLabelPixels());
	joinServer->addChild(joinServerSpriteLabel);
	joinServer->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(joinServer);

	menuSprites.pushBack(joinServer);
	menuSprites.pushBack(startServer);

	MenuItemImage* r1 = MenuItemImage::create("rectFull.png", "rectFull.png");
	r1->setPosition(Point(0, 0));
	r1->setScale(0.2f);
	MenuItemImage* r2 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	r2->setPosition(Point(0, -50));
	r2->setScale(0.2f);

	Menu* menu2 = Menu::create(r1, r2, NULL);
	menu2->setPosition(Point(visibleSize.width - r1->getContentSize().width * 0.4, visibleSize.height / 2 + 100 * 0.2f));
	this->addChild(menu2);

	this->menuRight.pushBack(r1);
	this->menuRight.pushBack(r2);

	menuRightI = 0;

	this->scheduleUpdate();
	this->resume();

	swipeBackStacker = 0;
	swipeBackTimer = 0;

    return true;
}

void NetworkChooseScene::update(float deltaTime)
{
	if(isTouchDown)
	{
		circleScaler += 1 * deltaTime;
		if(circleScaler >= 0.6)
		{
			circleScaler = 0.6;

			if(!IsNetworkConnected_CPP())
			{
				Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));
				GameManager::getInstance()->makeToast("Don't turn off your internet connection!");

				this->unscheduleUpdate();
				return;
			}

			switch(menuSprites.at(currentMenuSelection)->getTag())
			{
			case 0:
			    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, ClientWaitingScene::createScene()));
				break;
			case 1:
				if(serverStarted)
					break;

				StartServer_CPP(7777);
				serverStarted = true;
				Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, ServerWaitingScene::createScene()));
				break;
			}
		}
		fullCircle->setScale(circleScaler);

		if(initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			menuHandler(true, &menuRight, &menuRightI);
			mainMenuHandler(false, &menuSprites, &currentMenuSelection, menuSprites.size());
		}

		else if(initialTouchPos[1] - currentTouchPos[1] < -visibleSize.width * 0.05f) //meniuto sliza nadolo
		{
			isTouchDown = false;

			menuHandler(false, &menuRight, &menuRightI);
			mainMenuHandler(true, &menuSprites, &currentMenuSelection, menuSprites.size());
		}

		if(initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			swipeBackStacker += 1;

			if(swipeBackStacker >= 2)
			{
				if(swipeBackTimer <= 1.5f)
					Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));
				else
					swipeBackTimer = 0;
			}
		}
	}
	else
	{
		circleScaler -= 1 * deltaTime;
		if(circleScaler <= 0)
			circleScaler = 0;
		fullCircle->setScale(circleScaler);

		swipeBackTimer += 1 * deltaTime;
	}
}

void NetworkChooseScene::GoToClientConnectScene(Ref* sender)
{
//	auto clientConnectScene = ClientConnectScene::createScene();
//	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, clientConnectScene));
}
