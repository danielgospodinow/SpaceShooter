#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "MenuLayer.h"

class MainMenuScene : public MenuLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainMenuScene);
private:
    void GoToGameScene(cocos2d::Ref* sender);
    void GoToMultiplayerScene(cocos2d::Ref* sender);
    void ExitGame(cocos2d::Ref* sender);
    void GoToInfoScene(cocos2d::Ref* sender);

    void update(float deltaTime) override;

    void changeShipColor(std::string shipColorString, int red, int green, int blue);
    void setColors(int colorNumber);

    bool isTouchDown;

    float initialTouchPos[2];
    float currentTouchPos[2];

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    int currentColor;
    cocos2d::Sprite* backgroundImage;

    cocos2d::Sprite* debugPlaySprite;
    cocos2d::Sprite* playSprite;
    cocos2d::Sprite* exitSprite;
    cocos2d::Sprite* infoSprite;
    cocos2d::Vector<cocos2d::Sprite*> menuSprites;
    int currentMenuSelection;

    cocos2d::Vector<cocos2d::MenuItemImage*> menuBottom;
    int menuBottomI;
    cocos2d::Vector<cocos2d::MenuItemImage*> menuRight;
    int menuRightI;
};
#endif
