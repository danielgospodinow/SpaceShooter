#ifndef __CLIENT_CONNECT_SCENE_H__
#define __CLIENT_CONNECT_SCENE_H__

#include "ui/UITextField.h"
#include "MenuLayer.h"

class ClientConnectScene : public MenuLayer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ClientConnectScene)

private:
    void update(float deltaTime);

	cocos2d::Size visibleSize;

    cocos2d::ui::TextField* textField;

    std::string ipAdress;
    int port;

	bool runOnce;

    int swipeBackStacker;
    int swipeBackTimer;
};
#endif
