#ifndef __GAME_SCREEN_SCENE_H__
#define __GAME_SCREEN_SCENE_H__

#include "cocos2d.h"
#include "SpaceShip.h"
#include "EnemyShipBlast.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);

    static void initiateAllSprites();
    void flashBackground();

    void gameOver(bool isFromOpponent);
    void restartGame();
    void leaveGame();

    void removeTouchHandler();

    static bool debugMode;

private:
    void update(float deltaTime);
    void gameOverScheduler(float deltaTime);
    void debug_missleSpawnScheduler(float deltaTime);
    void OnAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event);
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    bool onContactSeperate(cocos2d::PhysicsContact &contact);
    void setEdgeCollider();
    void setSpaceShip(ShipType shipType);
    void setTouchHandler();
    void setCollisionHandler();
    void setAccelerometerListener();
    void removeAccelerometerListener();
    void leaveGameScheduler(float deltaTime);
    void leaveGameCountDownScheduler(float deltaTime);
    void deleteSpaceShipBlasts();
    void shipBlastStackHandler(float deltaTime);
    void fadeOutElements();
    void fadeInElements();

    SpaceShip* spaceShip;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Vec2 screenCenter;

    float timeToShoot_;
    float shootDelay_;

    float touchHoldDown_;
    bool isTouchDown_;
    int blastStacker_;

    cocos2d::Node* backgroundParent;

    cocos2d::EventListenerAcceleration* accelerometerListener;
    cocos2d::EventListenerTouchOneByOne* touchListener;
    cocos2d::EventListenerKeyboard* elk;

    float gameOverTimer;

    int countDownCounter;

    bool gameFinishStarted;
};
#endif
