#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "../General/Sprite.h"
#include "../Physics/BasicMovementFPSlimit.h"
#include "../General/gpRender.h"
#include "ai_enviroment.h"
#include "AI.h"
#include "../General/Ship.h"

constexpr int PLAYER_WIDTH = 50;
constexpr int PLAYER_HEIGHT = 50;
constexpr int ZONE_WIDTH = 1280; 
constexpr int ZONE_HEIGHT = 720;

void run_ai_enviro(gpRender gr){


	//Vector used to store all on screen entities

	std::vector<Sprite*> osSprite;

	//Camera Initilization
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	bool fixed = true;

	//gpRender object that is used to render object onto screen
	
	//Ship object init
	Ship aiShip;

	//AI init

	AI ai;

	aiShip.setSprite("Assets/Objects/ship_capital_ally.png");

	SDL_Texture* tex = gr.loadImage(aiShip.getSprite());
	//SDL_Rect db = {50,325,75,75};
	SDL_Rect db = {10,10,PLAYER_WIDTH,PLAYER_HEIGHT};

	Sprite playerent(db, tex);
	osSprite.push_back(&playerent);

	//Red giant Initilzation-
	SDL_Texture* tex2 = gr.loadImage("Assets/Objects/red_giant.png");
	SDL_Rect db2 = {500,200,300,300};
	Sprite starent(db2, tex2);

	osSprite.push_back(&starent);

	SDL_Rect bgtile1[400];
	SDL_Rect bgtile2[100];
	std::vector<std::vector<SDL_Rect*> > bgzonelayer1( ZONE_WIDTH/20 , std::vector<SDL_Rect*> (ZONE_HEIGHT/20, 0));
	std::vector<std::vector<SDL_Rect*> > bgzonelayer2( ZONE_WIDTH/40 , std::vector<SDL_Rect*> (ZONE_HEIGHT/40, 0));

	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			bgtile1[x + 20*y].x = x * 20;
			bgtile1[x + 20*y].y = y * 20;
			bgtile1[x + 20*y].w = 20;
			bgtile1[x + 20*y].h = 20;
		}
	}

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			bgtile2[x + 40*y].x = x * 40;
			bgtile2[x + 40*y].y = y * 40;
			bgtile2[x + 40*y].w = 40;
			bgtile2[x + 40*y].h = 40;
		}
	}

	SDL_Event e;
	bool gameon = true;
	//int animation = 0;
	//bool cycle;
	//bool animate;
	//Uint32 anim_last_time = SDL_GetTicks();

	//Game Loop
	while(gameon) {
		gr.setFrameStart(SDL_GetTicks());

		//Handles all incoming Key events
		while(SDL_PollEvent(&e)) {
			gameon = handleKeyEvents(e, playerent);	
			/*switch(e.key.keysym.sym) {
				case SDLK_w:
					if(e.type == SDL_KEYDOWN){
						animate = true;
					}
					else if (e.type == SDL_KEYUP){
						animate = false;
					}
					break;
			}*/
		}

		updatePosition(playerent, osSprite, ZONE_WIDTH, ZONE_HEIGHT);

		/*if (animate){
			if (SDL_GetTicks() - anim_last_time > 150) {
				if (animation == 0){
					cycle = true;
				}
				else if(animation == 3){
					cycle = false;
				}
				
				if (cycle){
					animation++;
				}
				else{
					animation--;
				}
				
				anim_last_time = SDL_GetTicks();
				playerent.setF(animation);
			}
		}
		else{
			animation = 0;
			playerent.setF(animation);
		}*/
		

		//Renders all renderable objects onto the screen
		gr.renderOnScreenEntity(osSprite, bgzonelayer1, bgzonelayer2, camera, fixed);
	}
}