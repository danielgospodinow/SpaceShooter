#include "GameEffects.h"
#include "GameManager.h"

USING_NS_CC;

GameEffects* GameEffects::instance_ = nullptr;
std::string GameEffects::damagedShipParticlePlistLocation = "particles/damagedShipParticles.plist";
std::string GameEffects::shipExplodePlistLocation = "particles/shipExplodeParticle.plist";

GameEffects* GameEffects::getInstance()
{
	if(instance_ == nullptr)
		instance_ = new GameEffects();

	return instance_;
}

ParticleSystemQuad* GameEffects::createDamagedShipParticle(std::string particleTextureLocation)
{
	ParticleSystemQuad* emitter = ParticleSystemQuad::create(damagedShipParticlePlistLocation);

	emitter->setSourcePosition(Vec2::ZERO);
	emitter->setStartSize(14);
	emitter->setEndSize(5);
	emitter->setTexture(TextureCache::sharedTextureCache()->addImage(particleTextureLocation));
	emitter->setStartColor(Color4F(255, 255, 255, 255));
	emitter->setEndColor(Color4F(255, 255, 255, 255));
	emitter->setScale(GameManager::getInstance()->getGlobalImageScaleFactor());
	emitter->setTag(7);

	return emitter;
}

ParticleSystemQuad* GameEffects::createShipExplodeParticle(std::string particleTextureLocation)
{
	ParticleSystemQuad* emitter = ParticleSystemQuad::create(shipExplodePlistLocation);

	emitter->setSourcePosition(Vec2::ZERO);
	emitter->setStartSize(14);
	emitter->setEndSize(5);
	emitter->setTexture(TextureCache::sharedTextureCache()->addImage(particleTextureLocation));
	emitter->setStartColor(Color4F(255, 255, 255, 255));
	emitter->setEndColor(Color4F(255, 255, 255, 255));
	emitter->setScale(GameManager::getInstance()->getGlobalImageScaleFactor());
	emitter->setTag(7);

	return emitter;
}
