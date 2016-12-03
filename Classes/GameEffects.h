#ifndef __GAME_EFFECTS_H__
#define __GAME_EFFECTS_H__

#include "cocos2d.h"

class GameEffects
{
public:
	static GameEffects* getInstance();

	cocos2d::ParticleSystemQuad* createDamagedShipParticle(std::string particleTextureLocation);
	cocos2d::ParticleSystemQuad* createShipExplodeParticle(std::string particleTextureLocation);

private:
	GameEffects(){};
	GameEffects(GameEffects const&){}; // copy constructor is private
	void operator=(GameEffects const&){};  // assignment operator is private

	static GameEffects* instance_;

	static std::string damagedShipParticlePlistLocation;
	static std::string shipExplodePlistLocation;
};

#endif
