#include "GameScene.h"
#include "Definitions.h"
#include "FriendlyShipBlast.h"
#include "ServerWaitingScene.h"
#include <cstdlib>
#include "MainMenuScene.h"

USING_NS_CC;

bool GameScene::debugMode = false;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );

    auto layer = GameScene::create();
    layer->setName("layer"); //This is used in the JNI restart game method;
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
        return false;

    if(EnemyShipBlast::gameSceneLayer_ == NULL)
		EnemyShipBlast::gameSceneLayer_ = static_cast<Layer*>(this);
    EnemyShipBlast::spriteFrameName = GameManager::getInstance()->getOpponentShipTypeString() + std::string("ShipBlast.png");

    backgroundParent = Node::create();

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    screenCenter = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    timeToShoot_ = 0;
    shootDelay_ = 0.07f;
    gameOverTimer = 0;

    setAccelerometerListener();
	setTouchHandler();
	setCollisionHandler();
	setEdgeCollider();
	setSpaceShip(GameManager::getInstance()->getMyShipType());

	this->scheduleUpdate();

	if(debugMode)
	{
        GameManager::getInstance()->setOpponentShipTypeWithString("red");
        this->schedule(schedule_selector(GameScene::debug_missleSpawnScheduler), 2);
	}

    this->setKeypadEnabled(true);
	elk = EventListenerKeyboard::create();
	elk->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_HOME)
		{
			leaveGame();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(elk, this);

	countDownCounter = 0;

	touchHoldDown_ = 0;
	isTouchDown_ = false;
	blastStacker_ = 0;

	this->setCascadeOpacityEnabled(true);

	gameFinishStarted = false;

    return true;
}

void GameScene::deleteSpaceShipBlasts()
{
	Vector<Node*> nodes = this->getChildren();
	for(Node* node: nodes)
	{
		if(node->getPhysicsBody() == NULL)
			continue;

		if(node->getPhysicsBody()->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK)
		{
			FriendlyShipBlast* fsb = static_cast<FriendlyShipBlast*>(node->getPhysicsBody()->getNode()->getUserData());
			fsb->startContact();
		}
		else if(node->getPhysicsBody()->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast* esb = static_cast<EnemyShipBlast*>(node->getPhysicsBody()->getNode()->getUserData());
			esb->startContact();
		}
	}
}

void GameScene::leaveGame()
{
	if(gameFinishStarted)
		return;

	if(debugMode)
	{
		debugMode = !debugMode;
		this->unschedule(schedule_selector(GameScene::debug_missleSpawnScheduler));
	}

	deleteSpaceShipBlasts();

	_eventDispatcher->removeEventListener(elk);

	GameManager::getInstance()->networkFunctionOnOpponent("LeaveOpponentGame");
	CloseServerClientConnection_CPP();

	this->unschedule(schedule_selector(GameScene::gameOverScheduler));

	removeAccelerometerListener();
	removeTouchHandler();

	if(spaceShip != NULL)
	{
		spaceShip->getSpaceShipSprite()->getPhysicsBody()->removeFromWorld();
		spaceShip->getSpaceShipSprite()->stopAllActions();
		spaceShip->getSpaceShipSprite()->runAction(FadeTo::create(1, 30));
		spaceShip->unscheduleThings();
	}

	this->fadeOutElements();

	this->schedule(schedule_selector(GameScene::leaveGameCountDownScheduler), 1, 4, 1);
}

void GameScene::leaveGameCountDownScheduler(float deltaTime)
{
	countDownCounter += 1;

	if(countDownCounter == 1)
	{
		Label* countDownLabel = Label::create("3", "arial", 430);
		countDownLabel->setName("cdl");
		countDownLabel->setPosition(Vec2(screenCenter));
		countDownLabel->runAction(ScaleBy::create(0.3f, 0.3f));
		this->addChild(countDownLabel);
	}
	else if(countDownCounter == 2)
	{
		Label* tempcdl = static_cast<Label*>(this->getChildByName("cdl"));
		tempcdl->setScale(1);
		tempcdl->setString("2");
		tempcdl->runAction(ScaleBy::create(0.3f, 0.3f));
	}
	else if(countDownCounter == 3)
	{
		Label* tempcdl = static_cast<Label*>(this->getChildByName("cdl"));
		tempcdl->setScale(1);
		tempcdl->setString("1");
		tempcdl->runAction(ScaleBy::create(0.3f, 0.3f));

		if(spaceShip != NULL)
			delete spaceShip;
	}
	else if(countDownCounter == 4)
	{
		this->removeChild(static_cast<Label*>(this->getChildByName("cdl")));
		this->unschedule(schedule_selector(GameScene::leaveGameCountDownScheduler));

		this->scheduleOnce(schedule_selector(GameScene::leaveGameScheduler), 0);
	}
}

void GameScene::leaveGameScheduler(float deltaTime)
{
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, MainMenuScene::createScene()));

	EnemyShipBlast::gameSceneLayer_ = NULL;
	GameManager::getInstance()->setOpponentShipTypeWithString("");
}

void GameScene::debug_missleSpawnScheduler(float deltaTime)
{
	int stacker = (rand()%6 + 1);
	EnemyShipBlast* debugESB = new EnemyShipBlast(Director::getInstance()->getVisibleSize().width / 2, 0, stacker);
}

void GameScene::gameOver(bool isFromOpponent)
{
	if(gameFinishStarted)
		return;
	gameFinishStarted = true;

	if(debugMode)
		this->unschedule(schedule_selector(GameScene::debug_missleSpawnScheduler));

	gameOverTimer = 0;

	this->unscheduleUpdate();
	isTouchDown_ = false;
	this->removeAccelerometerListener();
	this->removeTouchHandler();
	this->fadeOutElements();

	Label* gameOverLabel = Label::create("WIN", "arial", GameManager::getInstance()->getLabelPixels());
	gameOverLabel->setPosition(screenCenter);
	gameOverLabel->setCascadeOpacityEnabled(true);
	gameOverLabel->setOpacity(0);
	gameOverLabel->runAction(Sequence::create(FadeTo::create(0.3f, 255), FadeTo::create(1.4f, 255), FadeTo::create(0.3f, 0), NULL));

	if(!isFromOpponent)
	{
		GameManager::getInstance()->networkFunctionOnOpponent("RestartOpponentGame");

		this->runAction(
		  Sequence::create(
		    DelayTime::create(0),
		    CallFunc::create([this]() {
				delete spaceShip;
				spaceShip = NULL;
			}),
		    nullptr
		  )
		);

		gameOverLabel->setString("DEFEAT");
	}
	else
	{
		spaceShip->getSpaceShipSprite()->runAction(FadeTo::create(0.3f, 0));
	}

	this->addChild(gameOverLabel);

	this->schedule(schedule_selector(GameScene::gameOverScheduler));
	this->resume();
}

void GameScene::gameOverScheduler(float deltaTime)
{
	gameOverTimer += 1 * deltaTime;

	if(gameOverTimer > 2)
	{
		this->unschedule(schedule_selector(GameScene::gameOverScheduler));

		this->restartGame();
	}
}

void GameScene::restartGame()
{
	this->runAction(
	  Sequence::create(
		DelayTime::create(1),
		CallFunc::create([this]()
		{
			this->setAccelerometerListener();
			this->setTouchHandler();
			this->fadeInElements();

			gameFinishStarted = false;
		}),
		nullptr
	  )
	);

	this->scheduleUpdate();
	this->setSpaceShip(GameManager::getInstance()->getMyShipType());
	spaceShip->getSpaceShipSprite()->setOpacity(0);
	spaceShip->getSpaceShipSprite()->runAction(Sequence::create(FadeTo::create(0.6f, 0),FadeTo::create(0.4f, 255), NULL));
	this->resume();

	if(debugMode)
		this->schedule(schedule_selector(GameScene::debug_missleSpawnScheduler), 2);
}

void GameScene::fadeInElements()
{
	for(Node* node: this->getChildren())
	{
		if(node->getTag() == 7)
			node->runAction(FadeTo::create(0.3f, 255));
	}
}

void GameScene::fadeOutElements()
{
	for(Node* node: this->getChildren())
	{
		if(node->getTag() == 7)
			node->runAction(FadeTo::create(1, 30));
	}
}

void GameScene::update(float deltaTime)
{
	timeToShoot_ += 1 * deltaTime;

	if(isTouchDown_)
	{
		touchHoldDown_ += 1 * deltaTime;

		shipBlastStackHandler(deltaTime);
	}
}

void GameScene::shipBlastStackHandler(float deltaTime)
{
	if(touchHoldDown_ > 0.4f)
	{
		if(spaceShip->getCurrentShipShootAmout() <= 0)
			return;

		blastStacker_ += 1;
		spaceShip->reduceCurrentShipShootAmount();
		float currentScale = spaceShip->getSpaceShipSprite()->getScale();
		spaceShip->getSpaceShipSprite()->setScale(currentScale * 1.2f);
		spaceShip->getSpaceShipSprite()->runAction(ScaleTo::create(0.3f, currentScale));

		touchHoldDown_ = 0;
		return;
	}
}

void GameScene::setTouchHandler()
{
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [this](Touch* touch, Event* event)
	{
		if(spaceShip == NULL || spaceShip->getCurrentShipShootAmout() == 0)
			return true;

		isTouchDown_ = true;
		spaceShip->unscheduleThings();

		return true;
	};

	touchListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		if(spaceShip == NULL)
			return;

		if(timeToShoot_ > shootDelay_)
		{
			timeToShoot_ = 0;
			spaceShip->SpawnShipBlast(spaceShip, this, blastStacker_);
		}

		touchHoldDown_ = 0;
		blastStacker_ = 0;
		isTouchDown_ = false;
		spaceShip->scheduleThings();

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::setEdgeCollider()
{
	float sideEdgeOffset = 0, bottomEdgeOffset = 0;

	if(GameManager::getInstance()->getMyPhysicalSize() > GameManager::getInstance()->getOpponentPhysicalSize())
	{
		sideEdgeOffset = ((((GameManager::getInstance()->getMyPhysicalWidth() - GameManager::getInstance()->getOpponentPhysicalWidth()) / 2)) * visibleSize.width) / GameManager::getInstance()->getMyPhysicalWidth();
		bottomEdgeOffset = visibleSize.height - ((GameManager::getInstance()->getOpponentPhysicalHeight() * visibleSize.height) / GameManager::getInstance()->getMyPhysicalHeight());
	}

	auto edgeBodyTop = PhysicsBody::createBox(Size(visibleSize.width * 3, 50), PhysicsMaterial(0,0,0), Vec2::ZERO);
	auto edgeBodyBottom = PhysicsBody::createBox(Size(visibleSize.width * 3, 5), PhysicsMaterial(0,0,0), Vec2::ZERO);
	auto edgeBodyLeft = PhysicsBody::createBox(Size(5, visibleSize.height), PhysicsMaterial(0,0,0), Vec2::ZERO);
	auto edgeBodyRight = PhysicsBody::createBox(Size(5, visibleSize.height), PhysicsMaterial(0,0,0), Vec2::ZERO);

	edgeBodyTop->setDynamic(false);
	edgeBodyBottom->setDynamic(false);
	edgeBodyLeft->setDynamic(false);
	edgeBodyRight->setDynamic(false);

	edgeBodyTop->setCollisionBitmask(MAP_TOP_EDGE_COLLISION_BITMASK);
	edgeBodyBottom->setCollisionBitmask(MAP_BOTTOM_EDGE_COLLISION_BITMASK);
	edgeBodyLeft->setCollisionBitmask(MAP_LEFT_EDGE_COLLISION_BITMASK);
	edgeBodyRight->setCollisionBitmask(MAP_RIGHT_EDGE_COLLISION_BITMASK);

	edgeBodyTop->setContactTestBitmask(true);
	edgeBodyBottom->setContactTestBitmask(true);
	edgeBodyLeft->setContactTestBitmask(true);
	edgeBodyRight->setContactTestBitmask(true);

	auto edgeNodeTop = Node::create();
	auto edgeNodeBottom = Node::create();
	auto edgeNodeLeft = Node::create();
	auto edgeNodeRight = Node::create();

	edgeNodeTop->setPosition(visibleSize.width/2 + origin.x, visibleSize.height + origin.y + 23);
	edgeNodeBottom->setPosition(visibleSize.width/2 + origin.x, bottomEdgeOffset);
	edgeNodeLeft->setPosition(0 + sideEdgeOffset, visibleSize.height/2 + origin.y);
	edgeNodeRight->setPosition(visibleSize.width + origin.x - sideEdgeOffset, visibleSize.height/2 + origin.y);

	edgeNodeTop->setPhysicsBody(edgeBodyTop);
	edgeNodeBottom->setPhysicsBody(edgeBodyBottom);
	edgeNodeLeft->setPhysicsBody(edgeBodyLeft);
	edgeNodeRight->setPhysicsBody(edgeBodyRight);

	this->addChild(edgeNodeTop);
	this->addChild(edgeNodeBottom);
	this->addChild(edgeNodeLeft);
	this->addChild(edgeNodeRight);

	Sprite* outterEdge = GameManager::getInstance()->createColoredBlankSprite(Size(visibleSize.width, visibleSize.height), Color3B(40, 40, 40));
	outterEdge->setPosition(screenCenter);
	outterEdge->setTag(7);
	this->addChild(outterEdge);

	Sprite* innerEdge = GameManager::getInstance()->createColoredBlankSprite(Size(visibleSize.width - 2 * sideEdgeOffset, visibleSize.height - bottomEdgeOffset), Color3B(0,0,0));
	innerEdge->setPosition(screenCenter + Vec2(0, bottomEdgeOffset / 2));
	this->addChild(innerEdge);
	innerEdge->setTag(7);
}

void GameScene::setAccelerometerListener()
{
	if(accelerometerListener != NULL)
		return;

	Device::setAccelerometerEnabled(true);
    Device::setAccelerometerInterval(1.0 / 60);
	accelerometerListener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::OnAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accelerometerListener, this);
}

void GameScene::removeAccelerometerListener()
{
	Device::setAccelerometerEnabled(false);
	_eventDispatcher->removeEventListener(accelerometerListener);
	accelerometerListener = NULL;
}

void GameScene::initiateAllSprites()
{
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("images.plist", "images.png");
}

void GameScene::removeTouchHandler()
{
	_eventDispatcher->removeEventListener(touchListener);
}

void GameScene::setSpaceShip(ShipType shipType)
{
	isTouchDown_ = false;

	if(spaceShip != NULL)
	{
		delete spaceShip;
		spaceShip = NULL;
	}

	spaceShip = new SpaceShip(this, screenCenter, shipType);
}

void GameScene::OnAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event)
{
	spaceShip->ShipMovementHandler(acc->x, acc->y);
	spaceShip->ShipRotationHandler(acc->x);

	//spaceShip->MoveBackground(acc->x, acc->y, backgroundParent);
}

void GameScene::setCollisionHandler()
{
	auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeperate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *aBody = contact.getShapeA()->getBody();
	PhysicsBody *bBody = contact.getShapeB()->getBody();

	if((aBody->getCollisionBitmask() == MAP_TOP_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK) ||
		(aBody->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_TOP_EDGE_COLLISION_BITMASK))
	{
		int exitXCoord;
		float blastAngle;
		int stacker;

		if (aBody->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK)
		{
			FriendlyShipBlast* shipBlastC = static_cast<FriendlyShipBlast*>(aBody->getNode()->getUserData());
			exitXCoord = shipBlastC->getBlastSprite()->getPosition().x;
			blastAngle = aBody->getRotation();
			stacker = shipBlastC->getStacker();
			shipBlastC->startContact();
		}
		else if (bBody->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK)
		{
			FriendlyShipBlast* shipBlastC = static_cast<FriendlyShipBlast*>(bBody->getNode()->getUserData());
			exitXCoord = shipBlastC->getBlastSprite()->getPosition().x;
			blastAngle = bBody->getRotation();
			stacker = shipBlastC->getStacker();
			shipBlastC->startContact();
		}

		float oponentInstantiateXCoordInPercent = 100 - ((exitXCoord - GameManager::getInstance()->getHalfVoidSpace()) * 100 / GameManager::getInstance()->getOpponentResolutionWidthInMyPixels());

		std::string funcParams = std::string("SpawnEnemyShipBlast,") + GameManager::getInstance()->floatToString(oponentInstantiateXCoordInPercent) + std::string(",") + GameManager::getInstance()->floatToString(blastAngle) + std::string(",") + GameManager::getInstance()->intToString(stacker);
		GameManager::getInstance()->networkFunctionOnOpponent(funcParams);
	}

	else if((aBody->getCollisionBitmask() == ENEMY_SHIP_BLAST && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == ENEMY_SHIP_BLAST))
	{
		int damage = 0;

		if (aBody->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast *enemyShipBlastC = static_cast<EnemyShipBlast*>(aBody->getNode()->getUserData());
			enemyShipBlastC->startContact();
			damage = enemyShipBlastC->getStacker();
		}
		else if (bBody->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast *enemyShipBlastC = static_cast<EnemyShipBlast*>(bBody->getNode()->getUserData());
			enemyShipBlastC->startContact();
			damage = enemyShipBlastC->getStacker();
		}

		spaceShip->reduceLifePoints(damage);
	}

	else if((aBody->getCollisionBitmask() == ENEMY_SHIP_BLAST && bBody->getCollisionBitmask() == MAP_BOTTOM_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_BOTTOM_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == ENEMY_SHIP_BLAST))
	{
		if (aBody->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast *enemyShipBlastC = static_cast<EnemyShipBlast*>(aBody->getNode()->getUserData());
			enemyShipBlastC->startContact();
		}
		else if (bBody->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast *enemyShipBlastC = static_cast<EnemyShipBlast*>(bBody->getNode()->getUserData());
			enemyShipBlastC->startContact();
		}
	}

	else if((aBody->getCollisionBitmask() == ENEMY_SHIP_BLAST && bBody->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == SHIP_BLAST_COLLISION_BITMASK && bBody->getCollisionBitmask() == ENEMY_SHIP_BLAST))
	{
		if (aBody->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast *enemyShipBlastC = static_cast<EnemyShipBlast*>(aBody->getNode()->getUserData());
			FriendlyShipBlast *friendlyShipBlastC = static_cast<FriendlyShipBlast*>(bBody->getNode()->getUserData());
			int enemySBStacker = enemyShipBlastC->getStacker(), friendlySBStacker = friendlyShipBlastC->getStacker();

			if(enemySBStacker > friendlySBStacker)
			{
				friendlyShipBlastC->startContact();
				friendlyShipBlastC->spawnDestroyedParticles();
			}
			else if(enemySBStacker < friendlySBStacker)
			{
				enemyShipBlastC->startContact();
				enemyShipBlastC->spawnDestroyedParticles();
			}
			else
			{
				enemyShipBlastC->startContact();
				enemyShipBlastC->spawnDestroyedParticles();
				friendlyShipBlastC->startContact();
				friendlyShipBlastC->spawnDestroyedParticles();
			}
		}
		else if (bBody->getCollisionBitmask() == ENEMY_SHIP_BLAST)
		{
			EnemyShipBlast *enemyShipBlastC = static_cast<EnemyShipBlast*>(bBody->getNode()->getUserData());
			FriendlyShipBlast *friendlyShipBlastC = static_cast<FriendlyShipBlast*>(aBody->getNode()->getUserData());
			int enemySBStacker = enemyShipBlastC->getStacker(), friendlySBStacker = friendlyShipBlastC->getStacker();

			if(enemySBStacker > friendlySBStacker)
			{
				friendlyShipBlastC->startContact();
				friendlyShipBlastC->spawnDestroyedParticles();
			}
			else if(enemySBStacker < friendlySBStacker)
			{
				enemyShipBlastC->startContact();
				enemyShipBlastC->spawnDestroyedParticles();
			}
			else
			{
				enemyShipBlastC->startContact();
				enemyShipBlastC->spawnDestroyedParticles();
				friendlyShipBlastC->startContact();
				friendlyShipBlastC->spawnDestroyedParticles();
			}
		}
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_TOP_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_TOP_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->getSpaceShipSprite()->stopAllActionsByTag(5);
		spaceShip->collidedTop = true;
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_BOTTOM_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_BOTTOM_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->getSpaceShipSprite()->stopAllActionsByTag(5);
		spaceShip->collidedBottom = true;
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_RIGHT_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_RIGHT_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->getSpaceShipSprite()->stopAllActionsByTag(5);
		spaceShip->collidedRight = true;
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_LEFT_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_LEFT_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->getSpaceShipSprite()->stopAllActionsByTag(5);
		spaceShip->collidedLeft = true;
	}

	return true;
}

bool GameScene::onContactSeperate(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *aBody = contact.getShapeA()->getBody();
	PhysicsBody *bBody = contact.getShapeB()->getBody();

	if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_TOP_EDGE_COLLISION_BITMASK) ||
		(aBody->getCollisionBitmask() == MAP_TOP_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->collidedTop = false;
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_BOTTOM_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_BOTTOM_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->collidedBottom = false;
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_LEFT_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_LEFT_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->collidedLeft = false;
	}

	else if(((aBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK && bBody->getCollisionBitmask() == MAP_RIGHT_EDGE_COLLISION_BITMASK) ||
			(aBody->getCollisionBitmask() == MAP_RIGHT_EDGE_COLLISION_BITMASK && bBody->getCollisionBitmask() == SHIP_COLLISION_BITMASK)))
	{
		spaceShip->collidedRight = false;
	}

	return true;
}

void GameScene::flashBackground()
{
	for(int i=0; i<backgroundParent->getChildren().size(); i++)
	{
		Vector<Node*> bpV = backgroundParent->getChildren();
		Action* action = Sequence::create(FadeTo::create(0.3f, 90), FadeTo::create(0.3f, 40), NULL);
		bpV.at(i)->runAction(action);
	}
}
