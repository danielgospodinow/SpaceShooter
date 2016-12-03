#include "AboutScene.h"
#include "Definitions.h"
#include "MyJNIHelper.h"
#include "MainMenuScene.h"
USING_NS_CC;

Scene* AboutScene::createScene()
{
	auto scene = Scene::create();
    auto layer = AboutScene::create();
    scene->addChild(layer);

    return scene;
}

bool AboutScene::init()
{
    if (!Layer::init())
        return false;
    
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
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	currentMenuSelection = 0;

	developer = Sprite::create();
	developer->setTag(1);
	Label* developerSpriteLabel = Label::create("DEVELOPER", "arial", GameManager::getInstance()->getLabelPixels());
	developer->addChild(developerSpriteLabel);
	Label* developerInfo = Label::create("DANIEL GOSPODINOW", "arial", GameManager::getInstance()->getLabelPixels() / 4);
	developerInfo->setColor(Color3B(179,179,179));
	developerInfo->setPosition(Vec2(0, -developerSpriteLabel->getContentSize().height * 0.5f));
	developer->addChild(developerInfo);
	developer->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(developer);

	dpi = Sprite::create();
	dpi->setTag(0);
	Label* dpiSpriteLabel = Label::create("DPI", "arial", GameManager::getInstance()->getLabelPixels());
	dpi->addChild(dpiSpriteLabel);
	Label* dpiStatusLabel = Label::create(GameManager::getInstance()->intToString(GameManager::getInstance()->getDPI()).c_str(), "arial", GameManager::getInstance()->getLabelPixels() / 4);
	dpiStatusLabel->setColor(Color3B(179,179,179));
	dpiStatusLabel->setPosition(Vec2(0, -dpiSpriteLabel->getContentSize().height * 0.5f));
	dpiStatusLabel->setName("dpiStatusLabel");
	dpi->addChild(dpiStatusLabel);

	dpi->setPosition(Vec2(visibleSize.width/2 + origin.x, -100));
	this->addChild(dpi);

	ui::Button* buttonPlus = ui::Button::create();
	dpi->addChild(buttonPlus);
	buttonPlus->setPosition(Vec2(200, 0));
	buttonPlus->setTitleText("+");
	buttonPlus->setTitleFontSize(GameManager::getInstance()->getLabelPixels());
	buttonPlus->setTitleFontName("arial");
	buttonPlus->addTouchEventListener([this] (Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			GameManager::getInstance()->setDpi(GameManager::getInstance()->getDPI() + 1);
			refreshLabels();
		}
	});

	ui::Button* buttonMinus = ui::Button::create();
	dpi->addChild(buttonMinus);
	buttonMinus->setPosition(Vec2(-200, 0));
	buttonMinus->setTitleText("-");
	buttonMinus->setTitleFontSize(GameManager::getInstance()->getLabelPixels());
	buttonMinus->setTitleFontName("arial");
	buttonMinus->addTouchEventListener([this] (Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			GameManager::getInstance()->setDpi(GameManager::getInstance()->getDPI() - 1);
			refreshLabels();
		}
	});

	ui::Button* buttonReset = ui::Button::create();
	dpi->addChild(buttonReset);
	buttonReset->setPosition(Vec2(0, dpiSpriteLabel->getContentSize().height * 0.5f));
	buttonReset->setTitleText("RESET");
	buttonReset->setName("buttonReset");
	buttonReset->setTitleFontSize(GameManager::getInstance()->getLabelPixels() / 3.5f);
	buttonReset->setTitleFontName("arial");
	buttonReset->addTouchEventListener([this] (Ref* pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			GameManager::getInstance()->setDpi(GetDPI_CPP());
			refreshLabels();
		}
	});

	physicalSize = Sprite::create();
	physicalSize->setTag(2);
	Label* physicalSizeSpriteLabel = Label::create("PHYSICAL SIZE", "arial", GameManager::getInstance()->getLabelPixels());
	physicalSize->addChild(physicalSizeSpriteLabel);
	Label* physicalSizeStatus = Label::create((GameManager::getInstance()->floatToString(GameManager::getInstance()->getDevicePhysicalSize(visibleSize.width, visibleSize.height, GameManager::getInstance()->getDPI())) + "\"").c_str(), "arial", GameManager::getInstance()->getLabelPixels() / 4);
	physicalSizeStatus->setColor(Color3B(179,179,179));
	physicalSizeStatus->setPosition(Vec2(0, -dpiSpriteLabel->getContentSize().height * 0.5f));
	physicalSizeStatus->setName("physicalSizeStatus");
	physicalSize->addChild(physicalSizeStatus);
	physicalSize->setPosition(Vec2(visibleSize.width/2 + origin.x, -100));
	this->addChild(physicalSize);

	menuSprites.pushBack(developer);
	menuSprites.pushBack(dpi);
	menuSprites.pushBack(physicalSize);

	MenuItemImage* r1 = MenuItemImage::create("rectFull.png", "rectFull.png");
	r1->setPosition(Point(0, 0));
	r1->setScale(0.2f);
	MenuItemImage* r2 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	r2->setPosition(Point(0, -50));
	r2->setScale(0.2f);
	MenuItemImage* r3 = MenuItemImage::create("rectEmpty.png","rectEmpty.png");
	r3->setPosition(Point(0, -100));
	r3->setScale(0.2f);

	Menu* menu2 = Menu::create(r1, r2, r3, NULL);
	menu2->setPosition(Point(visibleSize.width - r1->getContentSize().width * 0.4, visibleSize.height / 2 + 50));
	this->addChild(menu2);

	this->menuRight.pushBack(r1);
	this->menuRight.pushBack(r2);
	this->menuRight.pushBack(r3);

	menuRightI = 0;

    this->scheduleUpdate();
    this->resume();

    swipeBackStacker = 0;
    swipeBackTimer = 0;

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

	refreshLabels();

    return true;
}

void AboutScene::refreshLabels()
{
	static_cast<Label*>(this->dpi->getChildByName("dpiStatusLabel"))->setString(GameManager::getInstance()->intToString(GameManager::getInstance()->getDPI()));
	static_cast<Label*>(this->physicalSize->getChildByName("physicalSizeStatus"))->setString(GameManager::getInstance()->floatToString(GameManager::getInstance()->getMyPhysicalSize()) + "\"");

	if(GameManager::getInstance()->getDPI() == GetDPI_CPP())
		static_cast<ui::Button*>(this->dpi->getChildByName("buttonReset"))->runAction(FadeTo::create(0.5f, 0));
	else
		static_cast<ui::Button*>(this->dpi->getChildByName("buttonReset"))->runAction(FadeTo::create(0.5f, 180));
}

void AboutScene::update(float deltaTime)
{
	if(isTouchDown)
	{
		if(initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05f)
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
		swipeBackTimer += 1 * deltaTime;
	}
}
