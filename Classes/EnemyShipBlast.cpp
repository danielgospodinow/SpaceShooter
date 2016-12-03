#include "EnemyShipBlast.h"
#include "Definitions.h"
#include "MyJNIHelper.h"
#include "GameScene.h"

USING_NS_CC;

std::string EnemyShipBlast::spriteFrameName = "";
cocos2d::Layer* EnemyShipBlast::gameSceneLayer_ = nullptr;

EnemyShipBlast::~EnemyShipBlast()
{
	this->unscheduleUpdate();
	contacting_ = false;
	blastSprite_->removeFromParentAndCleanup(true);
}

EnemyShipBlast::EnemyShipBlast(float xCoord, float blastAngle, int stacker)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	contacting_ = false;

	blastSprite_ = Sprite::createWithSpriteFrameName(spriteFrameName);
	blastSprite_->setUserData(this);
	if(stacker <= 0)
		stacker = 1;
	this->stacker = stacker;
	blastSprite_->setScale(0.4f * GameManager::getInstance()->getGlobalImageScaleFactor() * stacker);
	auto blastRigidBody = PhysicsBody::createBox(blastSprite_->getContentSize() * 0.4f, PHYSICSBODY_MATERIAL_DEFAULT, Vec2::ZERO);
	blastRigidBody->setCollisionBitmask(ENEMY_SHIP_BLAST);
	blastRigidBody->setRotationEnable(false);
	blastRigidBody->setContactTestBitmask(true);
	blastRigidBody->setDynamic(true);
	blastSprite_->setPhysicsBody(blastRigidBody);
	blastSprite_->setPosition(Point(xCoord, visibleSize.height + blastSprite_->getContentSize().height));
	blastSprite_->setRotation(blastAngle);

	gameSceneLayer_->addChild(blastSprite_);

	float angle = -this->blastSprite_->getRotation() + 90;
	this->direction_ = Vec2(-cos(angle / 180.0f * d_PI), -sin(angle / 180.0f * d_PI));

	this->scheduleUpdate();
	this->resume();
}

void EnemyShipBlast::spawnDestroyedParticles()
{
	ParticleSystemQuad* blueEmitter = GameEffects::getInstance()->createDamagedShipParticle("particles/" + GameManager::getInstance()->getOpponentShipTypeString() + "ParticleTexture.png");
	blueEmitter->setStartSize(6);
	blueEmitter->setEndSize(2);
	blueEmitter->setAngle(-90);
	blueEmitter->setAutoRemoveOnFinish(true);
	blueEmitter->setPosition(blastSprite_->getPosition() + Vec2(0, blastSprite_->getContentSize().height / 4));
	gameSceneLayer_->addChild(blueEmitter);

	ParticleSystemQuad* whiteEmitter = GameEffects::getInstance()->createDamagedShipParticle("particles/whiteParticleTexture.png");
	whiteEmitter->setStartSize(6);
	whiteEmitter->setEndSize(2);
	whiteEmitter->setAngle(-90);
	whiteEmitter->setAutoRemoveOnFinish(true);
	whiteEmitter->setPosition(blastSprite_->getPosition() + Vec2(0, blastSprite_->getContentSize().height / 4));
	gameSceneLayer_->addChild(whiteEmitter);
}

void EnemyShipBlast::update(float deltaTime)
{
	if(this->contacting_)
	{
		delete this;
		return;
	}

	blastSprite_->runAction(MoveBy::create(0.2f, this->direction_ * deltaTime * speed_));
}
