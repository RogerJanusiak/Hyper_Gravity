#include "../../includes/HEng/RunController.h"
#include "../../includes/Utils/Input.h"

RunController::RunController(GlobalGameState& ggs) : ggs(ggs) {
	currentRun = std::make_unique<Run>(ggs,ggs.level);
	waveController = std::make_unique<WaveController>(ggs, *currentRun);

	gameOverText.setup(ggs.renderer);
	gameOverText.loadFromRenderedText(" Run Over, You Died! ", ggs.white,ggs.title);

	continueText.setup(ggs.renderer);
	if(ggs.controller != nullptr) {
		continueText.loadFromRenderedText("To continue press A.",ggs.white,ggs.buttonFont);
	} else {
		continueText.loadFromRenderedText("To continue press Space.",ggs.white,ggs.buttonFont);
	}

	grave.setup(scale(64*2.5), scale(72*2.5), ggs.renderer);
	grave.loadFromFile("gravestone.png");

}

void RunController::run() {
	if(!ggs.playerDeath) {
		waveController->readInput();
		waveController->operate();
	} else {
		render();
		readInput();
	}
}

void RunController::readInput() {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
		if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
        	if(e.key.keysym.sym == SDLK_SPACE) {
        		ggs.playerDeath = false;
        		ggs.inRun = false;
        	}
        } else if(e.type == SDL_KEYUP) {

        } else if( e.type == SDL_JOYAXISMOTION) {

        } else if(e.type == SDL_JOYDEVICEADDED ) {
            loadController();
        	continueText.loadFromRenderedText("To continue press A.",ggs.white,ggs.buttonFont);
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            ggs.controller = nullptr;
        	continueText.loadFromRenderedText("To continue press Space.",ggs.white,ggs.buttonFont);
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
        	if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
        		ggs.playerDeath = false;
        		ggs.inRun = false;
        	}
        }
    }
}

void RunController::render() {

	gameOverText.render((WINDOW_WIDTH-gameOverText.getWidth())/2, scale(150));
	grave.render((WINDOW_WIDTH-grave.getWidth())/2, scale(220));
	continueText.render((WINDOW_WIDTH-continueText.getWidth())/2, scale(430));

}
