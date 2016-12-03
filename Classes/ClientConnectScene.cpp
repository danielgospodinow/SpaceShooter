#include "ClientConnectScene.h"
#include "Definitions.h"
#include "NetworkChooseScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* ClientConnectScene::createScene()
{
	auto scene = Scene::create();
    auto layer = ClientConnectScene::create();
    scene->addChild(layer);

    return scene;
}

bool ClientConnectScene::init()
{
    if (!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    port = 7777;
    ipAdress = "";

    textField = ui::TextField::create("ENTER IP ADDRESS","arial", GameManager::getInstance()->getLabelPixels());
    textField->setMaxLength(15);
    textField->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height / 2));
    this->addChild(textField);

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
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setKeypadEnabled(true);
	EventListenerKeyboard* elk = EventListenerKeyboard::create();
	elk->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			HideSystemUI_CPP();
			Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(elk, this);

	runOnce = false;

	this->circleInitiate();

	this->scheduleUpdate();
	this->resume();

    return true;
}

void ClientConnectScene::update(float deltaTime)
{
	if(isTouchDown)
	{
		circleScaler += 1 * deltaTime;
		if(circleScaler >= 0.6)
		{
			circleScaler = 0.6;

			if(runOnce)
				return;

			HideSystemUI_CPP();
			ipAdress = textField->getString();

			if(ipAdress != "")
				ConnectToServer_CPP(ipAdress.c_str(), port);

			runOnce = true;
		}
		fullCircle->setScale(circleScaler);
	}
	else
	{
		circleScaler -= 1 * deltaTime;
		if(circleScaler <= 0)
			circleScaler = 0;
		fullCircle->setScale(circleScaler);
	}
}
