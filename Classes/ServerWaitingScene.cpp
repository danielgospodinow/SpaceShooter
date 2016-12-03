#include "ServerWaitingScene.h"
#include "NetworkChooseScene.h"
#include "Definitions.h"
#include "MyJNIHelper.h"
#include "MainMenuScene.h"
#include "ui/UIButton.h"

USING_NS_CC;

Scene* ServerWaitingScene::createScene()
{
	auto scene = Scene::create();
    auto layer = ServerWaitingScene::create();
    scene->addChild(layer);

    return scene;
}

bool ServerWaitingScene::init()
{
    if (!Layer::init())
        return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    statusLabelDots = 0;
   	statusLabelText = "WAITING FOR AN OPPONENT ";
   	statusLabel = Label::createWithTTF(statusLabelText, "fonts/arial.ttf", GameManager::getInstance()->getLabelPixels() * 75 / 100);
   	statusLabel->setPosition(Vec2(visibleSize.width/2 + origin.x - statusLabel->getContentSize().width/2, visibleSize.height/2 + origin.y - statusLabel->getContentSize().height/2));
   	statusLabel->setAnchorPoint(Vec2(0, 0));
   	this->addChild(statusLabel);

   	this->schedule(schedule_selector(ServerWaitingScene::statusLabelHandler), 0.5f);

   	StartHostGameBroadcasting_CPP();

   	this->setKeypadEnabled(true);
	EventListenerKeyboard* elk = EventListenerKeyboard::create();
	elk->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			StopHostGameBroadcasting_CPP();
			StopServerThread_CPP();
			Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, MainMenuScene::createScene()));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(elk, this);

	ui::Button* button = ui::Button::create();
	button->setTitleFontSize(GameManager::getInstance()->getLabelPixels() * 6 / 10.0f);
	button->setTitleFontName("arial");
	button->setTitleText("+");
	button->setName("button");
	button->addTouchEventListener([this] (Ref* sender, ui::Widget::TouchEventType type)
	{
		if(ui::Widget::TouchEventType::BEGAN == type)
		{
			if(this->getChildByName("ipAdressLabel") != NULL)
				return;

			Label* ipAdressLabel = Label::create(std::string(GetWantedIPAdress_CPP()), "arial", GameManager::getInstance()->getLabelPixels() * 4 / 10.0f);
			ipAdressLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x, static_cast<ui::Button*>(this->getChildByName("button"))->getPositionY()));
			ipAdressLabel->setName("ipAdressLabel");
			ipAdressLabel->setCascadeOpacityEnabled(true);
			ipAdressLabel->setOpacity(0);
			this->addChild(ipAdressLabel);
			ipAdressLabel->runAction(Sequence::create(FadeTo::create(0.5f, 255), FadeTo::create(4, 255), FadeTo::create(0.5f, 0), RemoveSelf::create(false), NULL));
		}
	});
	Label* tipLabel = Label::create("SHOW IP", "arial", GameManager::getInstance()->getLabelPixels() * 3 / 10.0f);
	button->setPosition(Vec2(Director::getInstance()->getVisibleSize().width + origin.x - button->getContentSize().width * 1.2f, visibleSize.height + origin.y - button->getContentSize().height / 2));
	tipLabel->setPosition(button->getPosition() + Vec2(-(tipLabel->getContentSize().width / 1.4f), -2));
	this->addChild(button);
	this->addChild(tipLabel);

    return true;
}

void ServerWaitingScene::statusLabelHandler(float deltaTime)
{
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

void ServerWaitingScene::ToastWantedIPAdress()
{
	ToastWantedIPAdress_CPP();
}
