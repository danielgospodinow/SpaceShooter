#ifndef __SPACE_SHIP_H__
#define __SPACE_SHIP_H__

#include "cocos2d.h"
#include "cocos/base/CCVector.h"

enum ShipType {Green, Blue, Yellow, Red, Purple, None};

class SpaceShip : cocos2d::Node
{
public:
	SpaceShip(cocos2d::Layer* layer, cocos2d::Vec2 position, ShipType shipTypeC);
	~SpaceShip();

	ShipType GetShipType();

	void update(float deltaTime);
	void setPositionX(float x);
	void setPositionY(float y);
	void shootAmountHandler(float deltaTime);
	void changeShipAmountSprite();
	void SpawnShipBlast(SpaceShip* spaceShip, cocos2d::Layer* layer, int stacker);
	void ShipMovementHandler(float accX, float accY);
	void ShipRotationHandler(float xRot);
	void MoveBackground(float accX, float accY, cocos2d::Node* backgroundNode);
	void reduceLifePoints(int damagePoints);
	void spawnDamagedParticles();
	void unscheduleThings();
	void scheduleThings();
    void reduceCurrentShipShootAmount(){this->currentShootAmount_ -= 1; this->changeShipAmountSprite();}
    int getCurrentShipShootAmout(){return this->currentShootAmount_;}

	bool collidedTop;
	bool collidedBottom;
	bool collidedRight;
	bool collidedLeft;

	float getPositionX();
	float getPositionY();

	cocos2d::Vec2 getPosition();
	cocos2d::Size getContentSize();
	cocos2d::Sprite* getSpaceShipSprite();

private:
	void spawnDestroyParticles();

	cocos2d::Sprite* spaceShip;
	ShipType shipType;

	cocos2d::Layer* gameSceneLayer;

	float deltaTime_;

	int lifePoints;
	int shootAmount_;
	int currentShootAmount_;
	float shootAmmoRestoreTime_;
	float shootAmountTimer_;

	float scaleFactor_;
};
#endif
