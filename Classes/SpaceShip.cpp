#include "SpaceShip.h"
#include "Definitions.h"
#include "FriendlyShipBlast.h"
#include "GameScene.h"

USING_NS_CC;

SpaceShip::~SpaceShip()
{
	this->unscheduleUpdate();
	spaceShip->removeFromParentAndCleanup(true);
}

SpaceShip::SpaceShip(Layer* layer, Vec2 position, ShipType shipTypeC)
{
	gameSceneLayer = layer;

	shootAmount_ = 6;
	currentShootAmount_ = shootAmount_;
	shootAmmoRestoreTime_ = 0.8f;
	shootAmountTimer_ = 0;
	lifePoints = shootAmount_;

	deltaTime_ = 0;

	collidedTop = false;
	collidedLeft = false;
	collidedRight = false;
	collidedBottom = false;

	this->shipType = shipTypeC;
	GameManager::getInstance()->setMyShipType(shipTypeC);

	spaceShip = Sprite::createWithSpriteFrameName(GameManager::getInstance()->getMyShipTypeString() + "ShipAmount6.png");
	FriendlyShipBlast::spriteFrameName = GameManager::getInstance()->getMyShipTypeString() + "ShipBlast.png";

	spaceShip->setPosition(position);
	PhysicsBody *spaceShipRigid = PhysicsBody::createBox(Size(spaceShip->getContentSize().width * 0.66f, spaceShip->getContentSize().height), PhysicsMaterial(0,0,0), Vec2::ZERO);
	spaceShipRigid->setCollisionBitmask(SHIP_COLLISION_BITMASK);
	spaceShipRigid->setContactTestBitmask(true);
	spaceShipRigid->setDynamic(true);
	spaceShipRigid->setRotationEnable(false);
	spaceShip->setPhysicsBody(spaceShipRigid);
	spaceShip->setScale(0.4f * GameManager::getInstance()->getGlobalImageScaleFactor());

	scaleFactor_ = GameManager::getInstance()->getGlobalImageScaleFactor();

	layer->addChild(spaceShip);

	this->scheduleThings();
}

void SpaceShip::SpawnShipBlast(SpaceShip* spaceShip, Layer* layer, int stacker)
{
	if(currentShootAmount_ == 0 && stacker == 0)
		return;

	//static_cast<GameScene*>(gameSceneLayer)->flashBackground();
	FriendlyShipBlast *pShipBlast = new FriendlyShipBlast(layer,
														  spaceShip->getPosition() + Vec2(0,spaceShip->getContentSize().height / 4),
														  spaceShip,
														  stacker);

	shootAmountTimer_ = 0;
	if(stacker == 0)
		currentShootAmount_ -= 1;
	changeShipAmountSprite();
}

void SpaceShip::reduceLifePoints(int damagePoints)
{
	spawnDamagedParticles();

	if(lifePoints - damagePoints <= 0)
	{
		spawnDestroyParticles();
		static_cast<GameScene*>(gameSceneLayer)->gameOver(false);
		return;
	}
	else
	{
		lifePoints -= damagePoints;
		currentShootAmount_ -= damagePoints;
		changeShipAmountSprite();
	}
}

void SpaceShip::update(float deltaTime)
{
	this->deltaTime_ = deltaTime;

	shootAmountHandler(deltaTime);
}

void SpaceShip::unscheduleThings()
{
	this->unscheduleUpdate();
}

void SpaceShip::scheduleThings()
{
	this->scheduleUpdate();
	this->resume();
}

void SpaceShip::spawnDamagedParticles()
{
	ParticleSystemQuad* blueEmitter = GameEffects::getInstance()->createDamagedShipParticle("particles/" + GameManager::getInstance()->getMyShipTypeString() + "ParticleTexture.png");
	gameSceneLayer->addChild(blueEmitter);
	blueEmitter->setPosition(spaceShip->getPosition() + Vec2(0, spaceShip->getContentSize().height / 4));
	blueEmitter->setAutoRemoveOnFinish(true);

	ParticleSystemQuad* whiteEmitter = GameEffects::getInstance()->createDamagedShipParticle("particles/whiteParticleTexture.png");
	gameSceneLayer->addChild(whiteEmitter);
	whiteEmitter->setPosition(spaceShip->getPosition() + Vec2(0, spaceShip->getContentSize().height / 4));
	whiteEmitter->setAutoRemoveOnFinish(true);
}

void SpaceShip::spawnDestroyParticles()
{
	ParticleSystemQuad* blueEmitter = GameEffects::getInstance()->createShipExplodeParticle("particles/" + GameManager::getInstance()->getMyShipTypeString() + "ParticleTexture.png");
	gameSceneLayer->addChild(blueEmitter);
	blueEmitter->setPosition(spaceShip->getPosition());
	blueEmitter->setAutoRemoveOnFinish(true);

	ParticleSystemQuad* whiteEmitter = GameEffects::getInstance()->createShipExplodeParticle("particles/whiteParticleTexture.png");
	gameSceneLayer->addChild(whiteEmitter);
	whiteEmitter->setPosition(spaceShip->getPosition());
	whiteEmitter->setAutoRemoveOnFinish(true);
}

void SpaceShip::shootAmountHandler(float deltaTime)
{
	shootAmountTimer_ += (1 * deltaTime);

	if(shootAmountTimer_ >= shootAmmoRestoreTime_)
	{
		if(currentShootAmount_ == lifePoints)
			return;
		else if(currentShootAmount_ > lifePoints)
		{
			currentShootAmount_ = lifePoints;
		}

		currentShootAmount_ += 1;
		changeShipAmountSprite();
		shootAmountTimer_ = 0;
	}
}

void SpaceShip::changeShipAmountSprite()
{
	if(currentShootAmount_ < 0)
		currentShootAmount_ = 0;

	spaceShip->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(GameManager::getInstance()->getMyShipTypeString() + "ShipAmount" + GameManager::getInstance()->intToString(currentShootAmount_) + ".png"));
}

void SpaceShip::ShipMovementHandler(float accX, float accY)
{
	if((collidedTop && accY > 0) || (collidedBottom && accY < 0))
		accY = 0;

	if((collidedRight && accX > 0) || (collidedLeft && accX < 0))
		accX = 0;

	MoveBy* moveAction = MoveBy::create(0.2f, Vec2(accX * 50, accY * 50) * Director::getInstance()->getDeltaTime() * 50 * scaleFactor_);
	moveAction->setTag(5);
	spaceShip->runAction(moveAction);
}

void SpaceShip::MoveBackground(float accX, float accY, Node* backgroundNode)
{
	if((collidedTop && accY > 0) || (collidedBottom && accY < 0))
		accY = 0;

	if((collidedRight && accX > 0) || (collidedLeft && accX < 0))
		accX = 0;

	backgroundNode->runAction(MoveBy::create(0.4f, Vec2(accX * 10, accY * 10) * Director::getInstance()->getDeltaTime() * 50));
}

void SpaceShip::ShipRotationHandler(float xRot)
{
	this->spaceShip->setRotation(xRot * 15);
}

ShipType SpaceShip::GetShipType()
{
	return this->shipType;
}

void SpaceShip::setPositionX(float x)
{
	this->spaceShip->setPositionX(x);
}

void SpaceShip::setPositionY(float y)
{
	this->spaceShip->setPositionY(y);
}

float SpaceShip::getPositionX()
{
	return this->spaceShip->getPositionX();
}

float SpaceShip::getPositionY()
{
	return this->spaceShip->getPositionY();
}

Vec2 SpaceShip::getPosition()
{
	return this->spaceShip->getPosition();
}

Size SpaceShip::getContentSize()
{
	return this->spaceShip->getContentSize();
}

Sprite* SpaceShip::getSpaceShipSprite()
{
	return this->spaceShip;
}
