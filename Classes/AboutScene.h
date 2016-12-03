#ifndef __ABOUT_SCENE_H__
#define __ABOUT_SCENE_H__

#include "MenuLayer.h"
#include "ui/UIButton.h"

class AboutScene : public MenuLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(AboutScene);

private:
    void update(float deltaTime) override;
    void refreshLabels();

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    cocos2d::Sprite* dpi;
    cocos2d::Sprite* developer;
    cocos2d::Sprite* physicalSize;

    cocos2d::ui::Button* plusButton;
    cocos2d::ui::Button* minusButton;
    cocos2d::ui::Button* resetButton;

    cocos2d::Vector<cocos2d::Sprite*> menuSprites;
    int currentMenuSelection;

    cocos2d::Vector<cocos2d::MenuItemImage*> menuRight;
    int menuRightI;

    int swipeBackStacker;
    float swipeBackTimer;
};
#endif
