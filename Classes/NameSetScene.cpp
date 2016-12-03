#include "NameSetScene.h"
#include "Definitions.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* NameSetScene::createScene()
{
	auto scene = Scene::create();
    auto layer = NameSetScene::create();
    scene->addChild(layer);

    return scene;
}

bool NameSetScene::init()
{
    if (!Layer::init())
        return false;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    textField = ui::TextField::create("ENTER YOUR NAME","arial", GameManager::getInstance()->getLabelPixels());
    textField->setMaxLength(15);
    textField->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height / 2));
    this->addChild(textField);

    auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(NameSetScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(NameSetScene::onTouchEnded, this);
	listener->onTouchCancelled = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		isTouchDown = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	isTouchDown = false;

	runOnce = false;

	Sprite* emptyCircle = Sprite::createWithSpriteFrameName("circleEmpty.png");
	fullCircle = Sprite::createWithSpriteFrameName("circleFull.png");
	emptyCircle->setScale(0.6f);
	fullCircle->setScale(0);
	emptyCircle->setPosition(Vec2(50, visibleSize.height - 50));
	fullCircle->setPosition(Vec2(50, visibleSize.height - 50));
	this->addChild(fullCircle);
	this->addChild(emptyCircle);
	circleScaler = 0;

	this->scheduleUpdate();
	this->resume();

    return true;
}

void NameSetScene::update(float deltaTime)
{
	if(isTouchDown)
	{
		circleScaler += 1 * deltaTime;
		if(circleScaler >= 0.6)
		{
			circleScaler = 0.6;

			if(runOnce || textField->getString() == "")
				return;

			UserDefault::sharedUserDefault()->setStringForKey("name", textField->getString());
			Director::getInstance()->replaceScene(MainMenuScene::createScene());

			runOnce = true;
		}
		else if(circleScaler >= 0.1f)
		{
			HideSystemUI_CPP();
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

bool NameSetScene::onTouchBegan(Touch* touch, Event *event)
{
	isTouchDown = true;

	return true;
}

void NameSetScene::onTouchEnded(Touch* touch, Event *event)
{
	isTouchDown = false;

	runOnce = false;
}
