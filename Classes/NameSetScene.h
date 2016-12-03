#ifndef __NAME_SET_SCENE_H__
#define __NAME_SET_SCENE_H__

#include "cocos2d.h"
#include "ui/UITextField.h"

class NameSetScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(NameSetScene);

private:
    void update(float deltaTime);

    void TouchEnd(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

    cocos2d::ui::TextField* textField;

    cocos2d::Sprite* fullCircle;
	float circleScaler;

	bool isTouchDown;

	bool runOnce;
};
#endif
