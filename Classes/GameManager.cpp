#include "GameManager.h"

GameManager* GameManager::instance_ = nullptr;

GameManager* GameManager::getInstance()
{
	if(instance_ == nullptr)
		instance_ = new GameManager();

	return instance_;
}
