#include "ClientWaitingScene.h"
#include "Definitions.h"
#include "NetworkChooseScene.h"
#include "MainMenuScene.h"
#include <algorithm>
#include <vector>
#include "ui/UIButton.h"

#include "ClientConnectScene.h"

USING_NS_CC;

Scene* ClientWaitingScene::createScene()
{
	auto scene = Scene::create();
    auto layer = ClientWaitingScene::create();
    layer->setTag(69);
    scene->addChild(layer);

    return scene;
}

bool ClientWaitingScene::init()
{
    if (!Layer::init())
        return false;

    this->setName("cwsLayer");

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

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

	initStatusLabel();

	totalHostGames = 0;

	currentMenuSelection = -1;

	centerScreenPos = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
	offScreenPos = Vec2(visibleSize.width/2 + origin.x, -100);

	menu = NULL;
	menuI = 0;

	runOnce = false;

	this->resume();

	this->runAction(
	  Sequence::create(
		DelayTime::create(1),
		CallFunc::create([this]() {
			StartHostGamesDiscovery_CPP();
		}),
		nullptr
	  )
	);

	this->setKeypadEnabled(true);
	EventListenerKeyboard* elk = EventListenerKeyboard::create();
	elk->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			StopHostGamesDiscovery_CPP();
			Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(elk, this);

	ui::Button* button = ui::Button::create();
	button->setTitleFontSize(GameManager::getInstance()->getLabelPixels() * 6 / 10.0f);
	button->setTitleFontName("arial");
	button->setTitleText("+");
	button->addTouchEventListener([this] (Ref* sender, ui::Widget::TouchEventType type)
	{
		if(ui::Widget::TouchEventType::BEGAN == type)
		{
			StopHostGamesDiscovery_CPP();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, ClientConnectScene::createScene()));
		}
	});
	Label* tipLabel = Label::create("MANUAL FIND", "arial", GameManager::getInstance()->getLabelPixels() * 3 / 10.0f);
	button->setPosition(Vec2(visibleSize.width + origin.x - button->getContentSize().width * 1.2f, visibleSize.height + origin.y - button->getContentSize().height / 2));
	tipLabel->setPosition(button->getPosition() + Vec2(-(tipLabel->getContentSize().width / 1.6f), -2));
	this->addChild(button);
	this->addChild(tipLabel);

    return true;
}

void ClientWaitingScene::update(float deltaTime)
{
	if(isTouchDown)
	{
		circleScaler += 1 * deltaTime;
		if(circleScaler >= 0.6)
		{
			circleScaler = 0.6;

			if(runOnce)
				return;

			std::string ip = GameManager::getInstance()->split(allHostGames.at(currentMenuSelection), ',').at(0);

			//if(IsIPAvaivable_CPP(ip))
				ConnectToServer_CPP(ip.c_str(), 7777);
//			else
//			{
//				GameManager::getInstance()->makeToast("Sorry, this game wasn't available!");
//				removeHostGame(allHostGames.at(currentMenuSelection));
//			}

			runOnce = true;
		}

		fullCircle->setScale(circleScaler);

		if(initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05f)
		{
			isTouchDown = false;
		}

		else if(initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05f)
		{
			isTouchDown = false;
		}

		else if(initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			if(totalHostGames == 0 || totalHostGames == 1)
				return;

			menuHandler(false, &menuImages, &menuI);
			mainMenuHandler(false, &vectorLabels, &currentMenuSelection, vectorLabels.size());
		}

		else if(initialTouchPos[1] - currentTouchPos[1] < -visibleSize.width * 0.05f)
		{
			isTouchDown = false;

			if(totalHostGames == 0 || totalHostGames == 1)
				return;

			menuHandler(true, &menuImages, &menuI);
			mainMenuHandler(true, &vectorLabels, &currentMenuSelection, vectorLabels.size());
		}
	}
	else
	{
		circleScaler -= 1 * deltaTime;
		if(circleScaler <= 0)
			circleScaler = 0;
		fullCircle->setScale(circleScaler);

		runOnce = false;
	}
}

void ClientWaitingScene::initStatusLabel()
{
	statusLabelDots = 0;
	statusLabelText = "SEARCHING FOR A GAME ";
	statusLabel = Label::createWithTTF(statusLabelText, "fonts/arial.ttf", GameManager::getInstance()->getLabelPixels() * 75 / 100);
	statusLabel->setPosition(Vec2(visibleSize.width/2 + origin.x - statusLabel->getContentSize().width/2, visibleSize.height/2 + origin.y - statusLabel->getContentSize().height/2));
	statusLabel->setAnchorPoint(Vec2(0, 0));
	this->addChild(statusLabel);
	this->schedule(schedule_selector(ClientWaitingScene::statusLabelHandler), 0.5f);
}

void ClientWaitingScene::removeHostGame(std::string args) //Iska pipane
{
	//StopHostGamesDiscovery_CPP();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));

//	if(!(std::find(allHostGames.begin(), allHostGames.end(), args) != allHostGames.end()))
//		return;

//	allHostGames.erase(std::remove(allHostGames.begin(), allHostGames.end(), args), allHostGames.end());
//
//	Label* label = static_cast<Label*>(this->getChildByName(args + "label"));
//	MenuItemImage* menuItemImage = static_cast<MenuItemImage*>(this->getChildByName(args + "itemImage"));
//
//	vectorLabels.eraseObject(label, false);
//	menuImages.eraseObject(menuItemImage, false);
//
//	label->removeFromParent();
//
//	menu->removeChild(menuItemImage, true);
//
//	totalHostGames -= 1;
//
//	if(allHostGames.size() == 0)
//	{
//		if(statusLabel == NULL)
//		{
//			initStatusLabel();
//			currentMenuSelection = -1;
//
//			this->unscheduleUpdate();
//			this->schedule(schedule_selector(ClientWaitingScene::statusLabelHandler), 0.5f);
//
//			fullCircle->setOpacity(0);
//			fullCircle = NULL;
//			this->removeChildByName("emptyCircle");
//		}
//
//		menu = NULL;
//	}
//	else
//	{
//		currentMenuSelection = 0;
//
//		menuHandler(true, &menuImages, &menuI);
//		mainMenuHandler(true, &vectorLabels, &currentMenuSelection, vectorLabels.size());
//	}
}

void ClientWaitingScene::addHostGame(std::string args)
{
	gotAGame = true;

	if(std::find(allHostGames.begin(), allHostGames.end(), args) != allHostGames.end())
		return;

	if(statusLabel != NULL)
	{
		this->unschedule(schedule_selector(ClientWaitingScene::statusLabelHandler));
		statusLabel->removeFromParent();
		statusLabel = NULL;
		currentMenuSelection = 0;
	    this->circleInitiate();
	    this->scheduleUpdate();
	}

	allHostGames.push_back(args);
	std::vector<std::string> strArgs = GameManager::getInstance()->split(args, ',');

	Label* tempLabel = Label::create(strArgs[1], "arial", GameManager::getInstance()->getLabelPixels());
	tempLabel->setName(args + "label");

	MenuItemImage* tempItemImage = NULL;

	if(totalHostGames == 0)
	{
		tempLabel->setPosition(centerScreenPos);
		tempItemImage = MenuItemImage::create("rectFull.png", "rectFull.png");
	}
	else
	{
		tempLabel->setPosition(offScreenPos);
		tempItemImage = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	}

	tempItemImage->setName(args + "itemImage");

	vectorLabels.pushBack(tempLabel);

	tempItemImage->setPosition(Vec2(0, -totalHostGames * 100 * 0.2f * 2));
	tempItemImage->setScale(0.2f);

	menuImages.pushBack(tempItemImage);

	if(menu == NULL)
	{
		menu = Menu::create(tempItemImage, NULL);
		menu->setPosition(Vec2(visibleSize.width - tempItemImage->getContentSize().width * 0.2,
						   (visibleSize.height / 2 + Director::getInstance()->getVisibleOrigin().y - 100 * 0.2 / 2)));

		this->addChild(menu);
	}
	else
	{
		menu->addChild(tempItemImage);
		menu->setPosition(menu->getPosition().x, menu->getPosition().y + 100 * 0.2f);
	}

	this->addChild(tempLabel);

	totalHostGames += 1;

	gotAGame = false;
}

void ClientWaitingScene::statusLabelHandler(float deltaTime)
{
	if(gotAGame)
		return;

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
