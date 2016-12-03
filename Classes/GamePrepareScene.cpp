#include "GamePrepareScene.h"
#include "Definitions.h"
#include "GameScene.h"

USING_NS_CC;

Scene* GamePrepareScene::createScene()
{
	auto scene = Scene::create();
    auto layer = GamePrepareScene::create();
    scene->addChild(layer);

    return scene;
}

bool GamePrepareScene::init()
{
    if (!Layer::init())
        return false;
    
    glClearColor(0,0,0,1.0);

    std::string funcParameters01 = std::string("SendMyScreenDetails,") + GameManager::getInstance()->intToString(Director::getInstance()->getVisibleSize().width) + "," + GameManager::getInstance()->intToString(Director::getInstance()->getVisibleSize().height) + "," + GameManager::getInstance()->intToString(GameManager::getInstance()->getDPI());
	GameManager::getInstance()->networkFunctionOnOpponent(funcParameters01);

	std::string funcParameters02 = std::string("SendMyShipType,") + GameManager::getInstance()->getMyShipTypeString();
	GameManager::getInstance()->networkFunctionOnOpponent(funcParameters02);

	std::string funcParameters03 = std::string("SendMyName,") + GameManager::getInstance()->getMyName();
	GameManager::getInstance()->networkFunctionOnOpponent(funcParameters03);

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GamePrepareScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GamePrepareScene::onTouchEnded, this);
	listener->onTouchCancelled = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		isTouchDown = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	isTouchDown = false;

	runOnce = false;

	Sprite* emptyCircle = Sprite::createWithSpriteFrameName("circleEmpty.png");
	emptyCircle->setTag(15);
	fullCircle = Sprite::createWithSpriteFrameName("circleFull.png");
	emptyCircle->setScale(0.6f);
	fullCircle->setScale(0);
	emptyCircle->setPosition(Vec2(50, visibleSize.height - 50));
	fullCircle->setPosition(Vec2(50, visibleSize.height - 50));
	this->addChild(fullCircle);
	this->addChild(emptyCircle);
	circleScaler = 0;

	readyToStart = false;

	statusLabelDots = 0;
	statusLabelText = "Hold if you're ready ";
	statusLabel = Label::createWithTTF(statusLabelText, "fonts/arial.ttf", GameManager::getInstance()->getLabelPixels() * 40 / 100);
	statusLabel->setPosition(Vec2(visibleSize.width/2 + origin.x - statusLabel->getContentSize().width/2, visibleSize.height/2 + origin.y - statusLabel->getContentSize().height/2));
	statusLabel->setAnchorPoint(Vec2(0, 0));
	this->addChild(statusLabel);

	opponentNameLabel = Label::createWithTTF(std::string("Your opponent: ") + GameManager::getInstance()->getOpponentName(), "fonts/arial.ttf", GameManager::getInstance()->getLabelPixels() * 20 / 100);
	opponentNameLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - opponentNameLabel->getContentSize().height * 2));
	opponentNameLabel->setColor(Color3B(179,179,179));
	this->addChild(opponentNameLabel);

	this->scheduleUpdate();
	this->schedule(schedule_selector(GamePrepareScene::statusLabelHandler), 1);
	this->resume();

    return true;
}

void GamePrepareScene::statusLabelHandler(float deltaTime)
{
	opponentNameLabel->setString(std::string("Your opponent: ") + GameManager::getInstance()->getOpponentName());

	switch(statusLabelDots)
	{
	case 0:
		statusLabel->setString(statusLabelText);
		statusLabelDots += 1;
		break;
	case 1:
		statusLabel->setString(statusLabelText + std::string("."));
		statusLabelDots += 1;
		break;
	case 2:
		statusLabel->setString(statusLabelText + std::string(".."));
		statusLabelDots += 1;
		break;
	case 3:
		statusLabel->setString(statusLabelText + std::string("..."));
		statusLabelDots = 0;
		break;
	}
}

void GamePrepareScene::update(float deltaTime)
{
	if(readyToStart || GameManager::getInstance()->getOpponentIsReady())
	{
		if(readyToStart && GameManager::getInstance()->getOpponentIsReady())
		{
			GameManager::getInstance()->setOpponentIsReady(false);
			Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, GameScene::createScene()));
			this->unscheduleUpdate();
		}
		else if(readyToStart)
		{
			return;
		}
	}

	if(isTouchDown)
	{
		circleScaler += 1 * deltaTime;
		if(circleScaler >= 0.6)
		{
			circleScaler = 0.6;
			if(runOnce)
				return;

			statusLabelText = "Waiting for your opponent ";
			statusLabel->setString(statusLabelText);
			statusLabelDots = 0;
			statusLabel->setPosition(Vec2(visibleSize.width/2 + origin.x - statusLabel->getContentSize().width/2, visibleSize.height/2 + origin.y - statusLabel->getContentSize().height/2));
			this->getChildByTag(15)->setVisible(false);
			fullCircle->setVisible(false);
			readyToStart = true;

			std::string funcParameters04 = std::string("SetOpponentIsReady,") + std::string("true");
			GameManager::getInstance()->networkFunctionOnOpponent(funcParameters04);

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

bool GamePrepareScene::onTouchBegan(Touch* touch, Event *event)
{
	isTouchDown = true;

	return true;
}

void GamePrepareScene::onTouchEnded(Touch* touch, Event *event)
{
	isTouchDown = false;

	runOnce = false;
}
