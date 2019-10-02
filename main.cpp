#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "General/gpEntity.h"
#include "Physics/BasicMovementFPSlimit.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int FPS = 60;
constexpr int frameDelay = 1000/ FPS;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

bool init() {	
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
		
	gWindow = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	/* Create a renderer for our window
	 * Use hardware acceleration (last arg)
	 * Choose first driver that can provide hardware acceleration
	 *   (second arg, -1)
	 */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr) {	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			
	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	
	return true;
}

SDL_Texture* loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

void close() {
	for (auto i : gTex) {
		SDL_DestroyTexture(i);
		i = nullptr;
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main() {
	Uint32 frameStart;
	int frameTime;
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	SDL_Rect dr = {100, 100, 150, 150};
	SDL_Texture* tex = loadImage("Assets/Objects/ship_cruiser_ally.png");

	gpEntity testent(dr, tex);

	SDL_Event e;

	// Flag to control game loop
	bool gameon = true;

	// Game loop
	while(gameon) {
		frameStart = SDL_GetTicks();
		while(SDL_PollEvent(&e)) {
			// Only handling 1 event at the moment: closing the game window
			gameon = handleKeyEvents(e, testent);
			
		}
		
		testent.handelEntityPos(SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_RenderClear(gRenderer);	
		std::cout <<  testent.getVY()  << std::endl;

		SDL_RenderCopy(gRenderer, testent.getTexture(), NULL, testent.getDrawBox());
		SDL_RenderPresent(gRenderer);

		frameTime = SDL_GetTicks() - frameStart;

		if(frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	
	close();
}


	/**gpEntity testent;

	// Load media
	gTex.push_back(loadImage("Assets/Credits/credits.png"));
	gTex.push_back(loadImage("Assets/Credits/ai.png"));
	gTex.push_back(loadImage("Assets/Credits/cs1666_gjc26.png"));
	gTex.push_back(loadImage("Assets/Credits/CS1666_mrs185.png"));
	gTex.push_back(loadImage("Assets/Credits/CS1666_evs25.png"));
	gTex.push_back(loadImage("Assets/Credits/cs1666_jcz18.png"));
	gTex.push_back(loadImage("Assets/Credits/level-generation.png"));
	gTex.push_back(loadImage("Assets/Credits/CS1666_pjo13.png"));
	gTex.push_back(loadImage("Assets/Credits/cs1666_pep24.png"));
	gTex.push_back(loadImage("Assets/Credits/Keith C Stebler.png"));
	gTex.push_back(loadImage("Assets/Credits/physics.png"));
	gTex.push_back(loadImage("Assets/Credits/CS1666_MCD66.png"));
	gTex.push_back(loadImage("Assets/Credits/cs1666_kel117.png"));
	gTex.push_back(loadImage("Assets/Credits/CS1666_Linghai.png"));
	gTex.push_back(loadImage("Assets/Credits/CS1666_apw50.png"));
	gTex.push_back(loadImage("Assets/Credits/fin.png"));
	

	for(auto image : gTex){
		SDL_RenderClear(gRenderer);

		// Render the image
		SDL_RenderCopy(gRenderer, image, NULL, NULL);
		// Display rendering
		SDL_RenderPresent(gRenderer);
		// Wait 5 seconds
		SDL_Delay(5000);
	}**/
