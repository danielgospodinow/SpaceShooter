#include "SplashScreenScene.h"
#include "GameScene.h"
#include "NameSetScene.h"
#include "Definitions.h"
#include "MainMenuScene.h"

using namespace cocos2d;

Scene* SplashScreenScene::createScene()
{
	auto scene = Scene::create();
    auto layer = SplashScreenScene::create();
    scene->addChild(layer);

    return scene;
}

bool SplashScreenScene::init()
{
    if (!Layer::init())
        return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if(visibleSize.width > 1280)
    	GameManager::getInstance()->setLabelPixels(130);
    else if(visibleSize.width <= 1280 && visibleSize.width > 1024)
    	GameManager::getInstance()->setLabelPixels(100);
    else
    	GameManager::getInstance()->setLabelPixels(70);

    GameScene::initiateAllSprites();

    auto splashScreenSprite = Sprite::createWithSpriteFrameName("HelloWorld.png");
    splashScreenSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(splashScreenSprite);

    auto madeByLabel = Label::create("DEVELOPED BY DANIEL GOSPODINOW", "arial", GameManager::getInstance()->getLabelPixels() * 20 / 100);
    madeByLabel->setPosition(Point(visibleSize.width - madeByLabel->getContentSize().width / 1.8f, madeByLabel->getContentSize().height));
    this->addChild(madeByLabel);

    if(UserDefault::sharedUserDefault()->getStringForKey("name") != "")
    	this->scheduleOnce(schedule_selector(SplashScreenScene::GoToMainMenuScene), DISPLAY_TIME_SPLASH_SCENE);
    else
    	this->scheduleOnce(schedule_selector(SplashScreenScene::GoToNameEnterScene), DISPLAY_TIME_SPLASH_SCENE);

    return true;
}

void SplashScreenScene::GoToNameEnterScene(float deltaTime)
{
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, NameSetScene::createScene()));
}

void SplashScreenScene::GoToMainMenuScene(float deltaTime)
{
	auto mainMenuScene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, mainMenuScene));
}
