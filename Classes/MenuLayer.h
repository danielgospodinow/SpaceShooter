#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"

class MenuLayer: public cocos2d::Layer
{
public:
	MenuLayer();

protected:

	void update(float deltaTime) = 0;

	template<typename T> void mainMenuHandler(bool increase, cocos2d::Vector<T*>* menuItems, int* currentMenuElement, int maxElements)
	{
		int oldMenuItemIteration = *currentMenuElement;
			cocos2d::Vec2 oldMenuItemLocation;
			cocos2d::Vec2 currentMenuItemLocation;
			cocos2d::Vec2 currentMenuItemWantedLocation;

			if(increase)
			{
				oldMenuItemLocation = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width/2 + cocos2d::Director::getInstance()->getVisibleOrigin().x, cocos2d::Director::getInstance()->getVisibleSize().height + 100);
				currentMenuItemLocation = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width / 2 + cocos2d::Director::getInstance()->getVisibleOrigin().x, -100);
				currentMenuItemWantedLocation = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width/2 + cocos2d::Director::getInstance()->getVisibleOrigin().x, cocos2d::Director::getInstance()->getVisibleSize().height/2 + cocos2d::Director::getInstance()->getVisibleOrigin().y);

				*currentMenuElement += 1;
				if(*currentMenuElement > maxElements - 1)
					*currentMenuElement = 0;
			}
			else
			{
				oldMenuItemLocation = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width/2 + cocos2d::Director::getInstance()->getVisibleOrigin().x, -100);
				currentMenuItemLocation = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width / 2 + cocos2d::Director::getInstance()->getVisibleOrigin().x, cocos2d::Director::getInstance()->getVisibleSize().height + menuItems->at(*currentMenuElement)->getContentSize().height);
				currentMenuItemWantedLocation = cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width/2 + cocos2d::Director::getInstance()->getVisibleOrigin().x, cocos2d::Director::getInstance()->getVisibleSize().height/2 + cocos2d::Director::getInstance()->getVisibleOrigin().y);

				*currentMenuElement -= 1;
				if(*currentMenuElement < 0)
					*currentMenuElement = maxElements - 1;
			}

			static_cast<Node*>(menuItems->at(oldMenuItemIteration))->runAction(cocos2d::MoveTo::create(0.2f, oldMenuItemLocation));
			static_cast<Node*>(menuItems->at(*currentMenuElement))->setPosition(currentMenuItemLocation);
			static_cast<Node*>(menuItems->at(*currentMenuElement))->runAction(cocos2d::MoveTo::create(0.2f, currentMenuItemWantedLocation));
	};

	void menuHandler(bool increase, cocos2d::Vector<cocos2d::MenuItemImage*>* menu, int* menuIterator);
	void circleInitiate();

    cocos2d::Sprite* fullCircle;
    float circleScaler;

    bool isTouchDown;

    float initialTouchPos[2];
    float currentTouchPos[2];
};
#endif
