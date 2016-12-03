#ifndef __CLIENT_WAITING_SCENE_H__
#define __CLIENT_WAITING_SCENE_H__

#include "MenuLayer.h"

class ClientWaitingScene : public MenuLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ClientWaitingScene);

	void addHostGame(std::string args);
	void removeHostGame(std::string args);

private:
    void update(float deltaTime);

    void initStatusLabel();
    void statusLabelHandler(float deltaTime);
	cocos2d::Label* statusLabel;
	std::string statusLabelText;
	int statusLabelDots;

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    bool runOnce;

    std::vector<std::string> allHostGames;
    int totalHostGames;
    cocos2d::Menu* menu;
    cocos2d::Vector<cocos2d::Label*> vectorLabels;
    int currentMenuSelection;
    cocos2d::Vector<cocos2d::MenuItemImage*> menuImages;
    int menuI;
    cocos2d::Vec2 offScreenPos;
    cocos2d::Vec2 centerScreenPos;

    bool gotAGame;
};
#endif
