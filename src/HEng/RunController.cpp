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

	ggs.currentRunState = RunState::inMenu;
	runMenu = std::make_unique<RunMenu>(ggs, currentRun->getPlayer());
	runMenu->changeMenu(RunMenus::inventory);

}

void RunController::run() {
	if(waveController->hasAugmentBeenFound()) {
		waveController->setAugmentBeenFound();
		ggs.currentRunState = RunState::inMenu;
		selectAugmentFound();
	}

	switch (ggs.currentRunState) {
		case RunState::deathScreen: {
			renderDeathScreen();
			readInput();
		} break;
		case RunState::inWave: {
			waveController->readInput();
			waveController->operate();
			//readInput();
		} break;
		case RunState::inMenu: {
			runMenu->readInput();
			runMenu->run();
			runMenu->render();
		} break;
	}
}

void RunController::readInput() {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {

        if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
        	if(e.key.keysym.sym == SDLK_SPACE && ggs.currentRunState == RunState::deathScreen) {
        		ggs.currentRunState = RunState::inWave;
        		ggs.inRun = false;
        	}
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
            	if(ggs.currentRunState == RunState::deathScreen) {
            		ggs.currentRunState = RunState::inWave;
            		ggs.inRun = false;
            	}
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            ggs.controller = loadController();
        	continueText.loadFromRenderedText("To continue press A.",ggs.white,ggs.buttonFont);
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
        	continueText.loadFromRenderedText("To continue press Space.",ggs.white,ggs.buttonFont);
            ggs.controller = nullptr;
        }
    }
}

void RunController::renderDeathScreen() const {
	gameOverText.render((WINDOW_WIDTH-gameOverText.getWidth())/2, scale(150));
	grave.render((WINDOW_WIDTH-grave.getWidth())/2, scale(220));
	continueText.render((WINDOW_WIDTH-continueText.getWidth())/2, scale(430));
}

void RunController::selectAugmentFound() const {

	std::srand(std::time(0));
	int random_number = std::rand() % 9;

	if(random_number == 0) {
		ggs.newAugment = &ggs.damage1;
	} else if(random_number == 1) {
		ggs.newAugment = &ggs.damage2;
	} else if(random_number == 2) {
		ggs.newAugment = &ggs.damage3;
	} else if(random_number == 3) {
		ggs.newAugment = &ggs.clipIncrease1;
	} else if(random_number == 4) {
		ggs.newAugment = &ggs.clipIncrease2;
	} else if(random_number == 5) {
		ggs.newAugment = &ggs.clipIncrease3;
	} else if(random_number == 6) {
		ggs.newAugment = &ggs.reload1;
	} else if(random_number == 7) {
		ggs.newAugment = &ggs.reload2;
	} else if(random_number == 8) {
		ggs.newAugment = &ggs.reload3;
	}

}
