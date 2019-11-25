#pragma once
#include "gpRender.h"
//A BUNCH OF CONSTANTS FOR SIZES AND A FUNCTION TO GET A VECTOR OF TEXTURES TO BE REUSED
//THESE SHOULD BE GLOBAL CONSTANTS, NEVER CHANGE
//GLOBAL VARIABLES/CONSTANTS (misc)
#define PI 3.14159265
//GLOBAL SIZE CONSTANTS
#define FIGHTER_WIDTH 50
#define FIGHTER_HEIGHT 50
#define CRUISER_WIDTH 100
#define CRUISER_HEIGHT 100
#define CAPITAL_WIDTH 150
#define CAPITAL_HEIGHT 150
#define SHIP_SECTOR_LIMIT 3
#define DISTANCE_PURSUE 300
#define DISTANCE_ROAM_X 300
#define DISTANCE_ROAM_Y 150
#define FORMATION_OFFSET_1X 75
#define FORMATION_OFFSET_1Y 75
#define FORMATION_OFFSET_2X 150
#define FORMATION_OFFSET_2Y 150
//GLOBAL TEXTURE VALUES
//TO USE: CALL textures.at(TEX_FIGHT_HERO) to get the hero fighter
//0-8 ARE TEXTURES FOR SHIPS
//9-12 ARE TEXTURES FOR SUNS
//13 is asteroid
//14 is space station (one of them)
//15 is the laser projectile
//ADD MORE AS REQUIRED HERE AND IN initTextures
//#define TEX_SHIPS 0
#define TEX_FIGHT_HERO 0
#define TEX_FIGHT_ALLY 1
#define TEX_FIGHT_ENEMY 2
#define TEX_CRUIS_HERO 3
#define TEX_CRUIS_ALLY 4
#define TEX_CRUIS_ENEMY 5
#define TEX_CAPT_HERO 6
#define TEX_CAPT_ALLY 7
#define TEX_CAPT_ENEMY 8
#define TEX_SUN_BLUE 9
#define TEX_SUN_RED 10
#define TEX_SUN_WHITE 11
#define TEX_SUN_YELLOW 12
//#define TEX_ASTEROIDS 5
#define TEX_ASTEROID 13
#define TEX_STATION 14
#define TEX_LASER 15


//INIT TEXTURES INTO A VECTOR
std::vector<SDL_Texture*> initTextures(gpRender& gr);


//ENEMY SPAWN ZONES

