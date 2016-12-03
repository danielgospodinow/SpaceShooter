#ifndef __FRIENDLY_SHIP_BLAST_H__
#define __FRIENDLY_SHIP_BLAST_H__

#include "ShipBlast.h"
#include "SpaceShip.h"

using namespace std;

class FriendlyShipBlast: public ShipBlast
{
public:
	FriendlyShipBlast(cocos2d::Layer* layer, cocos2d::Vec2 shipDuoLocation, SpaceShip* spaceShip, int stacker);
	~FriendlyShipBlast();

	void update(float deltaTime) override;
	void spawnDestroyedParticles() override;

	static std::string spriteFrameName;

	int getStacker(){return this->stacker;};

private:
	int stacker;
};
#endif
