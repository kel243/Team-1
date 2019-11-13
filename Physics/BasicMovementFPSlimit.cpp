#include "BasicMovementFPSlimit.h"
#include "TimeData.h"
#include "../Physics/Audio.h"

#define PI 3.14159265

constexpr double ACCEL = 30.0;
constexpr double ROTATION_ACCEL = 7200.0;
constexpr float MAX_SPEED = 6;
constexpr float MAX_DELTAV = 1;
constexpr float MAX_ROTATIONSPEED = 6;
constexpr float MAX_ROTATIONRATE = 2;

/*
constexpr int ZONE_WIDTH = 3840; 
constexpr int ZONE_HEIGHT = 2160;
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;*/

//movement is handled by increasing and decreasing the thrust (acceleration) in a particular direction and is capped by a max speed and acceleration
float speed = 0;
float deltaV = 0;
//int acceleration = 0;
float rotationRate = 0;
float rotationSpeed = 0;
float direction;


//General wrapper function to handle Key evenets
bool handleKeyEvents(SDL_Event e, Ship &ent){
	if (e.type == SDL_QUIT) {
		return  false;
	}
	else if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		handleKeyDownEvent(e, ent);
	}
	else if(e.type == SDL_KEYUP){
		handleKeyUpEvent(e, ent);
	}

	return true;
	
}

//Handles Up Key Events
void handleKeyUpEvent(SDL_Event e, Ship &ent){
	if(e.type == SDL_KEYUP){
		switch(e.key.keysym.sym){
			case SDLK_s:
				deltaV = 0;
				break;
			case SDLK_a:
			case SDLK_d:
				rotationRate = 0;
				break;
				
		}
	
	}
}

//Handles down Key Events
void handleKeyDownEvent(SDL_Event e, Ship &ent){
	direction = (ent.getAngle() - 90.0)*PI/180;	

	switch(e.key.keysym.sym) {
		case SDLK_w:
			
			//ent.setVY(ent.getVY() - MAX_SPEED);
			deltaV += (ACCEL * TimeData::get_timestep());
			Audio::play_thrust_sound();
			break;

		case SDLK_a:

			//ent.setVX(ent.getVX() - MAX_SPEED);
			rotationRate -= (ROTATION_ACCEL * TimeData::get_timestep());
			Audio::play_thrust_sound();
			break;

		case SDLK_s:
		
			//ent.setVY(ent.getVY() + MAX_SPEED);
			
			deltaV -= (ACCEL * TimeData::get_timestep());
			Audio::play_thrust_sound();
			break;

		case SDLK_d:
			
			//ent.setVX(ent.getVX() + MAX_SPEED);
			rotationRate += (ROTATION_ACCEL * TimeData::get_timestep());
			Audio::play_thrust_sound();
			break;
		case SDLK_x:
			speed = 0;
			deltaV = 0;
		
		case SDLK_g:
			if(ent.getCurrHp() != ent.getMaxHp())	
				ent.setCurrHp(ent.getCurrHp() + 5);
			std::cout << "Current hp: " << ent.getCurrHp() << std::endl;
			break;
		case SDLK_f:
			ent.setCurrHp(ent.getCurrHp() - 5);
			std::cout << "Current hp: " << ent.getCurrHp() << std::endl;
			break;
		
	}
	if(deltaV > MAX_DELTAV)
	{
		deltaV = MAX_DELTAV;
	}
	else if(deltaV < -MAX_DELTAV)
	{
		deltaV = -MAX_DELTAV;
	}
	if(rotationRate > MAX_ROTATIONRATE)
	{
		rotationRate = MAX_ROTATIONRATE;
	}
	else if(rotationRate < -MAX_ROTATIONRATE)
	{
		rotationRate = -MAX_ROTATIONRATE;
	}
	
}

//tis is te simple collision check from the examples
bool check_collision2(SDL_Rect* a, SDL_Rect* b) {
	// Check vertical overlap
	if (a->y + a->h <= b->y)
		return false;
	if (a->y >= b->y + b->h)
		return false;

	// Check horizontal overlap
	if (a->x >= b->x + b->w)
		return false;
	if (a->x + a->w <= b->x)
		return false;

	// Must overlap in both
	return true;
}

//sz is the size scale factor to apply to the bounding box to check proximity, 
//  which avoids doing a magnitude check between 2 vectors
//
//keep in mind that only bounding box a will be scaled up,
//  so if one of the two bounding boxes was very large,
//  then put the large one in the b parameter, and make the sz bigger
bool check_proximity(Sprite &aSprite, Sprite &bSprite, int sz) {
	SDL_Rect* a = aSprite.getDrawBox();
	SDL_Rect* b = bSprite.getDrawBox();
	// Check vertical overlap
	if (a->y + (a->h * sz) <= b->y)
		return false;
	if (a->y - (a->h * (sz - 1)) >= b->y + b->h)
		return false;

	// Check horizontal overlap
	if (a->x - (a->w * (sz - 1)) >= b->x + b->w)
		return false;
	if (a->x + (a->w * sz) <= b->x)
		return false;

	// Must overlap in both
	return true;
}

bool check_all_collisions2(SDL_Rect* a, std::vector<Sprite*> &osSprite){
	bool isCollision = false;
	//std::cout << "osEntity.size() = " << osEntity.size() << std::endl;
	for(int i = 1;  i < osSprite.size(); i++){
		if(osSprite.at(i)->getRenderOrder() != 1 && osSprite.at(i)->getRenderOrder() != 3 && osSprite.at(i)->getRenderOrder() != 4){
			//so, one of these should result in collison if they are the same box
			isCollision |= check_collision2(a, osSprite.at(i)->getDrawBox());
			//std::cout << "Is last command Illegal?" << std::endl;
			//std::cout << "Checked collisions: " << i << std::endl;
		}
	}
	return isCollision;
}

std::vector<float> calculateMomentumConserv(Ship &playerent, std::vector<Ship*> &osShip){
	float playerXspd = playerent.getSpeedX();
	float playerYspd = playerent.getSpeedY();
	float otherXspd;
	float otherYspd;
	float playerResX = 0;
	float playerResY = 0;
	float otherResX = 0;
	float otherResY = 0;
	bool isCollision = false;
	for(int i = 1; i < osShip.size(); i++){
		isCollision = check_collision2(playerent.getDrawBox(), osShip.at(i)->getDrawBox());
		if(isCollision == true){
			otherXspd = osShip.at(i)->getSpeedX();
			otherYspd = osShip.at(i)->getSpeedY();
			playerResX = (playerXspd + otherXspd)/2;
			playerResY = (playerYspd + otherYspd)/2;
			otherResX = (playerXspd + otherXspd)/2;
			otherResY = (playerYspd + otherYspd)/2;

			osShip.at(i)->setSpeedX(otherResX);
			osShip.at(i)->setSpeedY(otherResY);
			osShip.at(i)->setX(osShip.at(i)->getTrueX() + (int)otherResX);
			osShip.at(i)->setY(osShip.at(i)->getTrueY() + (int)otherResY);
		}
	}

	return {playerResX, playerResY};
}




void updatePosition(Ship &ent, std::vector<Sprite*> &osSprite, int ZONE_WIDTH, int ZONE_HEIGHT){
	//needs to be changed to update all objects in the list
	speed += deltaV;
	rotationSpeed += rotationRate;
	if (rotationSpeed < 0)
	{
		rotationSpeed++;
	}
	else if (rotationSpeed > 0)
	{
		rotationSpeed--;
	}
	if(speed >MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	else if(speed < 0)
	{
		speed = 0;
	}
	if(rotationSpeed > MAX_ROTATIONSPEED)
	{
		rotationSpeed = MAX_ROTATIONSPEED;
	}
	else if(rotationSpeed < -MAX_ROTATIONSPEED)
	{
		rotationSpeed = -MAX_ROTATIONSPEED;
	}
	
	//std::cout << ent.getVX() << ", " << ent.getVY() <<std::endl;
	ent.setAngle(ent.getAngle() + rotationSpeed);
	float speedX = speed*cos((ent.getAngle() - 90.0)*PI/180);
	float speedY = speed*sin((ent.getAngle() - 90.0)*PI/180);
	// Try to move Horizontally

	std::vector<float> gravPulls = calculateGravityPull(ent, osSprite);
	speedX = speedX+gravPulls[0];
	speedY = speedY+gravPulls[1];
	ent.setSpeedX(speedX);
	ent.setSpeedY(speedY);

	ent.setX(ent.getTrueX() + speedX);
	if(ent.getTrueX() < 0 


		|| (ent.getX() + ent.getW() > ZONE_WIDTH) 
		|| check_all_collisions2(ent.getDrawBox(), osSprite)){

		ent.setX(ent.getTrueX() - speedX);
	}
	ent.setY(ent.getTrueY() + speedY);
	if(ent.getY() < 0 
		|| (ent.getY() + ent.getH() > ZONE_HEIGHT) 
		|| check_all_collisions2(ent.getDrawBox(), osSprite)){

		ent.setY(ent.getTrueY() - speedY);
	}

	std::cout << ent.getAngle() - 90 << std::endl;
	std::cout << "x: " << ent.getTrueX()  << std::endl;	
	std::cout << "y: " << ent.getTrueY()  << std::endl;
	std::cout << "speedX: " << speedX << std::endl;
	std::cout << "speedY: " << speedY << std::endl;
	std::cout << "Grav x: " << gravPulls[0] << std::endl;
	std::cout << "Grav y: " << gravPulls[1] << std::endl;

}

void updatePosition3(Ship &ent, std::vector<Sprite*> &osSprite, std::vector<Ship*> &osShip, int ZONE_WIDTH, int ZONE_HEIGHT){
	//needs to be changed to update all objects in the list
	speed += deltaV;
	rotationSpeed += rotationRate;
	if (rotationSpeed < 0)
	{
		rotationSpeed++;
	}
	else if (rotationSpeed > 0)
	{
		rotationSpeed--;
	}
	if(speed >MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	else if(speed < 0)
	{
		speed = 0;
	}
	if(rotationSpeed > MAX_ROTATIONSPEED)
	{
		rotationSpeed = MAX_ROTATIONSPEED;
	}
	else if(rotationSpeed < -MAX_ROTATIONSPEED)
	{
		rotationSpeed = -MAX_ROTATIONSPEED;
	}
	
	//std::cout << ent.getVX() << ", " << ent.getVY() <<std::endl;
	ent.setAngle(ent.getAngle() + rotationSpeed);
	float speedX = speed*cos((ent.getAngle() - 90.0)*PI/180);
	float speedY = speed*sin((ent.getAngle() - 90.0)*PI/180);
	// Try to move Horizontally

	std::vector<float> gravPulls = calculateGravityPull(ent, osSprite);
	speedX = speedX+gravPulls[0];
	speedY = speedY+gravPulls[1];
	std::vector<float> momentumShift = calculateMomentumConserv(ent, osShip);
	if(momentumShift[0] != 0|| momentumShift[1] != 0){
		std::cout << "Push x: " << momentumShift[0] << std::endl;
		std::cout << "Push y: " << momentumShift[1] << std::endl;

		speedX = momentumShift[0];
		speedY = momentumShift[1];

		ent.setX(ent.getTrueX() + speedX);
		speedX = 0;
		if(ent.getTrueX() < 0 


			|| (ent.getX() + ent.getW() > ZONE_WIDTH) 
			|| check_all_collisions2(ent.getDrawBox(), osSprite)){

			ent.setX(ent.getTrueX() - momentumShift[0]);
		}

		ent.setY(ent.getTrueY() + speedY);
		speedY = 0;
		if(ent.getY() < 0 
			|| (ent.getY() + ent.getH() > ZONE_HEIGHT) 
			|| check_all_collisions2(ent.getDrawBox(), osSprite)){

			ent.setY(ent.getTrueY() - momentumShift[1]);
		}
	}else{
		ent.setSpeedX(speedX);
		ent.setSpeedY(speedY);

		ent.setX(ent.getTrueX() + speedX);
		if(ent.getTrueX() < 0 


			|| (ent.getX() + ent.getW() > ZONE_WIDTH) 
			|| check_all_collisions2(ent.getDrawBox(), osSprite)){

			ent.setX(ent.getTrueX() - speedX);
		}
		ent.setY(ent.getTrueY() + speedY);
		if(ent.getY() < 0 
			|| (ent.getY() + ent.getH() > ZONE_HEIGHT) 
			|| check_all_collisions2(ent.getDrawBox(), osSprite)){

			ent.setY(ent.getTrueY() - speedY);
		}
	}

}


