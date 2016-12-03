#ifndef __SERVER_WAITING_SCENE_H__
#define __SERVER_WAITING_SCENE_H__

#include "cocos2d.h"

class ServerWaitingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ServerWaitingScene);

private:
    void ToastAllIPAdresses();
    void ToastWantedIPAdress();

    void statusLabelHandler(float deltaTime);
    cocos2d::Label* statusLabel;
	std::string statusLabelText;
	int statusLabelDots;
};
#endif
