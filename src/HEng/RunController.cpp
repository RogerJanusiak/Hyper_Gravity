#include "../../includes/HEng/RunController.h"
#include "../../includes/Utils/Input.h"

void upgrade(GlobalGameState& ggs, int attr1, int attr2) {

}

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

	buttonSound.init("resources/sounds/buttonClick.wav", 0,-1);
	inventoryMenu.setup(ggs.renderer, &buttonSound);

	inventoryMenuTitle.setup(ggs.renderer);
	inventoryMenuTitle.loadFromRenderedText("Choose Weapon to Upgrade",ggs.white,ggs.title);

	int spacing = (WINDOW_WIDTH-scaleUI(120*4))/5;
	int height = (WINDOW_HEIGHT - scaleUI(120))/2;
	//int bt1 = inventoryMenu.addButton(spacing,height,"ui/upgrade-revolver.png",-1,-1,-1,-1, &upgrade,ggs, 2,Weapon_Type::revolver);
	//int bt2 = inventoryMenu.addButton(spacing*2 + scaleUI(120),height,"ui/upgrade-shotgun.png",-1,-1,bt1,-1, &upgrade,ggs, 2,Weapon_Type::shotgun);
	//int bt3 = inventoryMenu.addButton(spacing*3 + scaleUI(120*2),height,"ui/upgrade-rifle.png",-1,-1,bt2,-1, &upgrade,ggs, 2,Weapon_Type::rifle);
	//inventoryMenu.addButton(spacing*4 + scaleUI(120*3),height,"ui/upgrade-laser-pistol.png",-1,-1,bt3,-1, &upgrade,ggs, 2,Weapon_Type::laserPistol);
	inventoryMenu.addTitle((WINDOW_WIDTH-inventoryMenuTitle.getWidth())/2, scale(100), inventoryMenuTitle);
}

void RunController::run() {
	switch (ggs.currentRunState) {
		case RunState::deathScreen: {
			renderDeathScreen();
			readInput();
		} break;
		case RunState::inWave: {
			waveController->readInput();
			waveController->operate();
		} break;
		case RunState::inventoryScreen: {
			currentMenu = &inventoryMenu;
			renderInventoryScreen();
			readInput();
		} break;
	}
}

void RunController::readInput() {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
        	if(e.key.keysym.sym == SDLK_SPACE) {
        		ggs.currentRunState = RunState::inWave;
        		ggs.inRun = false;
        	} else if(e.key.keysym.sym == SDLK_ESCAPE && ggs.currentRunState == RunState::inventoryScreen) {
        		currentMenu = nullptr;
        		ggs.currentRunState = RunState::inWave;
        	}

        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
            	if(ggs.currentRunState == RunState::deathScreen) {
            		ggs.currentRunState = RunState::inWave;
            		ggs.inRun = false;
            	} else {
            		if(currentMenu != nullptr) {
            			controllerEvent(*currentMenu,MENU_CONTROL::select);
            		}
            	}
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                //if(currentMenu == &levelSelect) {

                //}
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
            	if(currentMenu != nullptr) {
            		controllerEvent(*currentMenu,MENU_CONTROL::up);
            	}
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
            	if(currentMenu != nullptr) {
            		controllerEvent(*currentMenu,MENU_CONTROL::down);
            	}
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
            	if(currentMenu != nullptr) {
            		controllerEvent(*currentMenu,MENU_CONTROL::left);
            	}
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
            	if(currentMenu != nullptr) {
            		controllerEvent(*currentMenu,MENU_CONTROL::right);
            	}
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
        	if(currentMenu != nullptr) {
        		controllerEvent(*currentMenu,MENU_CONTROL::connect);
        	}
            ggs.controller = loadController();
        	continueText.loadFromRenderedText("To continue press A.",ggs.white,ggs.buttonFont);
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
        	if(currentMenu != nullptr) {
        		controllerEvent(*currentMenu,MENU_CONTROL::disconnect);
        	}
        	continueText.loadFromRenderedText("To continue press Space.",ggs.white,ggs.buttonFont);
            ggs.controller = nullptr;
        } else  if( e.type == SDL_MOUSEMOTION && currentMenu != nullptr) {
        	mouseMove(ggs, *currentMenu);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
        	if(currentMenu != nullptr) {
        		menuSelect(*currentMenu);
        	}
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(ggs.controllerStickReset) {
                	if(currentMenu != nullptr) {
                		controllerEvent(*currentMenu,MENU_CONTROL::down);
                	}
                    ggs.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(ggs.controllerStickReset) {
                	if(currentMenu != nullptr) {
                		controllerEvent(*currentMenu,MENU_CONTROL::up);
                	}
                    ggs.controllerStickReset = false;
                }
            } else {
                ggs.controllerStickReset = true;
            }
        }
    }
}

void RunController::renderDeathScreen() const {
	gameOverText.render((WINDOW_WIDTH-gameOverText.getWidth())/2, scale(150));
	grave.render((WINDOW_WIDTH-grave.getWidth())/2, scale(220));
	continueText.render((WINDOW_WIDTH-continueText.getWidth())/2, scale(430));
}

void RunController::loadUpdateScreen() {

}

void RunController::renderInventoryScreen() {
	inventoryMenu.render();
}

