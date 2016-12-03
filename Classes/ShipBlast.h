#ifndef __SHIP_BLAST_H__
#define __SHIP_BLAST_H__

#include "cocos2d.h"
#include "GameManager.h"

using namespace std;

class ShipBlast: public cocos2d::Node
{
public:
	virtual void update(float deltaTime) = 0;
	cocos2d::Sprite* getBlastSprite() {return this->blastSprite_;};
	void startContact() {contacting_ = true;};
	void endContact() {contacting_ = false;}
	virtual void spawnDestroyedParticles() = 0;

	virtual ~ShipBlast(){};

protected:
	cocos2d::Sprite *blastSprite_;
	cocos2d::Vec2 direction_;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	bool contacting_;
	float speed_ =  1600 * GameManager::getInstance()->getGlobalImageScaleFactor();
};
#endif
