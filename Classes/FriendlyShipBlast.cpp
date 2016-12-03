#include "FriendlyShipBlast.h"
#include "EnemyShipBlast.h"

#include "Definitions.h"

USING_NS_CC;

std::string FriendlyShipBlast::spriteFrameName = "";

FriendlyShipBlast::~FriendlyShipBlast()
{
	this->unscheduleUpdate();
	contacting_ = false;
	blastSprite_->removeFromParentAndCleanup(true);
}

FriendlyShipBlast::FriendlyShipBlast(Layer* layer, Vec2 shipDuoLocation, SpaceShip* spaceShip, int stacker)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	contacting_ = false;

	blastSprite_ = Sprite::createWithSpriteFrameName(spriteFrameName);

	blastSprite_->setUserData(this);
	blastSprite_->setPosition(shipDuoLocation);
	if(stacker <= 0)
		stacker = 1;
	this->stacker = stacker;
	blastSprite_->setScale(0.4f * GameManager::getInstance()->getGlobalImageScaleFactor() * stacker);
	auto blastRigidBody = PhysicsBody::createBox(blastSprite_->getContentSize() * 0.4f, PHYSICSBODY_MATERIAL_DEFAULT, Vec2::ZERO);
	blastRigidBody->setCollisionBitmask(SHIP_BLAST_COLLISION_BITMASK);
	blastRigidBody->setRotationEnable(false);
	blastRigidBody->setDynamic(true);
	blastRigidBody->setContactTestBitmask(true);
	blastSprite_->setPhysicsBody(blastRigidBody);
	blastSprite_->setRotation(spaceShip->getSpaceShipSprite()->getRotation() * 1.5f);

	layer->addChild(blastSprite_);

	float angle = -this->blastSprite_->getRotation() + 90;
	this->direction_ = Vec2(cos(angle / 180.0f * d_PI), sin(angle / 180.0f * d_PI));

	this->scheduleUpdate();
	this->resume();
}

void FriendlyShipBlast::spawnDestroyedParticles()
{
	ParticleSystemQuad* blueEmitter = GameEffects::getInstance()->createDamagedShipParticle("particles/" + GameManager::getInstance()->getMyShipTypeString() + "ParticleTexture.png");
	blueEmitter->setStartSize(6);
	blueEmitter->setEndSize(2);
	blueEmitter->setAutoRemoveOnFinish(true);
	blueEmitter->setPosition(blastSprite_->getPosition() + Vec2(0, blastSprite_->getContentSize().height / 4));
	EnemyShipBlast::gameSceneLayer_->addChild(blueEmitter);

	ParticleSystemQuad* whiteEmitter = GameEffects::getInstance()->createDamagedShipParticle("particles/whiteParticleTexture.png");
	whiteEmitter->setStartSize(6);
	whiteEmitter->setEndSize(2);
	whiteEmitter->setAutoRemoveOnFinish(true);
	whiteEmitter->setPosition(blastSprite_->getPosition() + Vec2(0, blastSprite_->getContentSize().height / 4));
	EnemyShipBlast::gameSceneLayer_->addChild(whiteEmitter);
}

void FriendlyShipBlast::update(float deltaTime)
{
	if(contacting_)
	{
		delete this;
		return;
	}

	blastSprite_->runAction(MoveBy::create(0.2f, this->direction_ * deltaTime * speed_));
}
