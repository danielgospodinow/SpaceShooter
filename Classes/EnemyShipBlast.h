#ifndef __ENEMY_SHIP_BLAST_H__
#define __ENEMY_SHIP_BLAST_H__

#include "ShipBlast.h"

using namespace std;

class EnemyShipBlast : public ShipBlast
{
public:
	EnemyShipBlast(float xCoord, float blastAngle, int stacker);
	~EnemyShipBlast();

	void update(float deltaTime) override;
	void spawnDestroyedParticles() override;

	static std::string spriteFrameName;
	static cocos2d::Layer* gameSceneLayer_;

	int getStacker(){return this->stacker;};

private:
	int stacker;
};
#endif
