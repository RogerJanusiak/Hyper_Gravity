#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "../includes/HEng/RunController.h"
#include "../includes/Utils/GlobalConstants.h"
#include "../includes/HEng/MainMenu.h"
#include "../includes/HEng/State.h"
#include "../includes/TEng/ConfirmPopup.h"

bool init(GlobalGameState& ggs) {
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO ) < 0 ) {
		SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	if (TTF_Init() < 0) {
		SDL_Log( "TTF could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	ggs.window = SDL_CreateWindow("Hyper_Gravity 0.4.5 (Unstable V4)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (ggs.window == nullptr) {
		SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	ggs.renderer = SDL_CreateRenderer(ggs.window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ggs.renderer == nullptr) {
		SDL_Log("Game renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
		return false;
	}

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		SDL_Log( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}

	ggs.controller = SDL_GameControllerOpen(SDL_NumJoysticks()-1);
	if(ggs.controller != nullptr) {
		SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
	}

	return true;
}

void loadGlobalResources(GlobalGameState& ggs) {
	ggs.buttonFont = TTF_OpenFont("resources/sans.ttf",scaleUI(14));
	ggs.verySmall = TTF_OpenFont("resources/sans.ttf",scaleUI(10));
	ggs.title = TTF_OpenFont("resources/Roboto-Regular.ttf",scaleUI(20));
	ggs.augTitle = TTF_OpenFont("resources/sans.ttf",scaleUI(12));
	ggs.augDescription = TTF_OpenFont("resources/sans.ttf",scaleUI(8));
}

void closeGlobalResources(const GlobalGameState& ggs) {
	TTF_CloseFont(ggs.buttonFont);
	TTF_CloseFont(ggs.verySmall);
	TTF_CloseFont(ggs.title);
	TTF_CloseFont(ggs.augTitle);
	TTF_CloseFont(ggs.augDescription);
}

void close(GlobalGameState ggs) {
	closeGlobalResources(ggs);

	SDL_DestroyRenderer(ggs.renderer);
	ggs.renderer = nullptr;

	SDL_GameControllerClose(ggs.controller);
	ggs.controller = nullptr;

	SDL_DestroyWindow(ggs.window);
	ggs.window = nullptr;

	Mix_CloseAudio();

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

int main( int argc, char* args[] ) {
	GlobalGameState ggs{};

	if(!init(ggs)) {
		SDL_Log("Initialization failed!\n");
		return 8;
	}

	loadGlobalResources(ggs);

	MainMenu mainMenu(ggs);
	setCurrentMainMenu(&mainMenu);

	float lastUpdate = 0;
	constexpr int targetFPS = 60;
	constexpr int frameDelay = 1000 / targetFPS;

	std::unique_ptr<RunController> rc;

	ggs.inMainMenu = true;
	ggs.level = 4;
	std::string test = "test";
	ConfirmPopup cp(ggs,test,nullptr);

	while(!ggs.quit) {
		Uint32 frameStart = SDL_GetTicks();
		Uint64 start = SDL_GetPerformanceCounter();
		Uint32 current = SDL_GetTicks();
		ggs.dt = (current - lastUpdate) / 1000.0f*1.05;
		lastUpdate = current;

		SDL_RenderClear(ggs.renderer);

		// TODO: Maybe there is a better way to do this?
		if(ggs.toPauseMenu) {
			mainMenu.changeMenu(pause);
			ggs.inPauseMenu = true;
			ggs.toPauseMenu = false;
		}

		if(ggs.toMainMenu) {
			mainMenu.changeMenu(head);
			ggs.inMainMenu = true;
			ggs.toMainMenu = false;
		}

		if(ggs.inMainMenu || ggs.inPauseMenu) {
			mainMenu.readInput();
			mainMenu.render();
		} else if(!ggs.inRun) {
			rc = std::make_unique<RunController>(ggs);
			ggs.inRun = true;
		}

		if(ggs.inRun && !ggs.inPauseMenu) {
			rc->run();
		}

		//cp.render();

		SDL_SetRenderDrawColor(ggs.renderer, 26, 26, 26, 255);
		SDL_RenderPresent(ggs.renderer);

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay) {
			SDL_Delay(frameDelay - frameTime);
		}



		Uint64 end = SDL_GetPerformanceCounter();

		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		ggs.fps = (1.0f / elapsed);

	}

	close(ggs);
}