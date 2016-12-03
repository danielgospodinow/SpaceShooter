#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <android/log.h>
#include <sstream>
#include <string>
#include "GameManager.h"
#include "GameEffects.h"

#define  LOG_TAG    "cocos2d-x debug info"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define DISPLAY_TIME_SPLASH_SCENE 2
#define TRANSITION_TIME 0.5

#define OBSTACLE_COLLISION_BITMASK 0x000001
#define SHIP_BLAST_COLLISION_BITMASK 0x000002
#define MAP_EDGE_COLLISION_BITMASK 0x000003
#define MAP_TOP_EDGE_COLLISION_BITMASK 0x000004
#define MAP_BOTTOM_EDGE_COLLISION_BITMASK 0x000005
#define MAP_LEFT_EDGE_COLLISION_BITMASK 0x000006
#define MAP_RIGHT_EDGE_COLLISION_BITMASK 0x000007
#define SHIP_COLLISION_BITMASK 0x000008
#define ENEMY_SHIP_BLAST 0x000009

#define SPACE_SHIP_SPEED 3
#define d_PI 3.14f

#endif
