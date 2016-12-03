#include "MenuLayer.h"
#include "EnemyShipBlast.h"

#include "GameManager.h"

USING_NS_CC;

MenuLayer::MenuLayer()
{
	fullCircle = NULL;
	circleScaler = 0;

	isTouchDown = false;

	initialTouchPos[0] = 0;
	initialTouchPos[1] = 0;
	currentTouchPos[0] = 0;
	currentTouchPos[1] = 0;
}

void MenuLayer::circleInitiate()
{
	cocos2d::Sprite* emptyCircle = cocos2d::Sprite::createWithSpriteFrameName("circleEmpty.png");
	emptyCircle->setName("emptyCircle");
	fullCircle = cocos2d::Sprite::createWithSpriteFrameName("circleFull.png");
	emptyCircle->setScale(0.6f);
	fullCircle->setScale(0);
	emptyCircle->setPosition(cocos2d::Vec2(50, cocos2d::Director::getInstance()->getVisibleSize().height - 50));
	fullCircle->setPosition(cocos2d::Vec2(50, cocos2d::Director::getInstance()->getVisibleSize().height - 50));
	this->addChild(fullCircle);
	this->addChild(emptyCircle);
	circleScaler = 0;
}

void MenuLayer::menuHandler(bool increase, cocos2d::Vector<cocos2d::MenuItemImage*>* menu, int* menuIterator)
{
	menu->at(*menuIterator)->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("rectEmpty.png"));
	menu->at(*menuIterator)->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("rectEmpty.png"));
	if(increase)
	{
		*menuIterator += 1;
		if(*menuIterator > menu->size() - 1)
			*menuIterator = 0;
	}
	else
	{
		*menuIterator -= 1;
		if(*menuIterator < 0)
			*menuIterator = menu->size() - 1;
	}
	menu->at(*menuIterator)->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("rectFull.png"));
	menu->at(*menuIterator)->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("rectFull.png"));
}
