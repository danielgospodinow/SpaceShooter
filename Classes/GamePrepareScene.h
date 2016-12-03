#ifndef __GAME_PREPARE_SCENE_H__
#define __GAME_PREPARE_SCENE_H__

#include "cocos2d.h"

class GamePrepareScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GamePrepareScene);

private:
    void update(float deltaTime);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

    cocos2d::Sprite* fullCircle;
	float circleScaler;

	bool isTouchDown;

	bool runOnce;

	bool readyToStart;

	void statusLabelHandler(float deltaTime);
	cocos2d::Label* statusLabel;
	std::string statusLabelText;
	int statusLabelDots;

	cocos2d::Label* opponentNameLabel;
};
#endif
