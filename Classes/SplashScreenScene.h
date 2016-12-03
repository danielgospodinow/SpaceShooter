#ifndef __SPLASH_SCREEN_SCENE_H__
#define __SPLASH_SCREEN_SCENE_H__

#include "cocos2d.h"

class SplashScreenScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(SplashScreenScene)

private:
    void GoToMainMenuScene(float deltaTime);
    void GoToNameEnterScene(float deltaTime);
};
#endif
