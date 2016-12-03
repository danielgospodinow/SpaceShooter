#ifndef __NETWORK_CHOOSE_SCENE_H__
#define __NETWORK_CHOOSE_SCENE_H__

#include "MenuLayer.h"

class NetworkChooseScene : public MenuLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(NetworkChooseScene);

private:
    void update(float deltaTime) override;

    void GoToClientConnectScene(cocos2d::Ref* sender);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    cocos2d::Sprite* startServer;
    cocos2d::Sprite* joinServer;

    bool serverStarted;

    cocos2d::Vector<cocos2d::Sprite*> menuSprites;
    int currentMenuSelection;

    cocos2d::Vector<cocos2d::MenuItemImage*> menuRight;
    int menuRightI;

    int swipeBackStacker;
    float swipeBackTimer;
};
#endif
