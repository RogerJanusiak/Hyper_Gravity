#include "../../includes/HEng/RunMenu.h"
#include "../../includes/HEng/Weapon.h"
#include "../../includes/TEng/Buttons.h"
#include "../../includes/Utils/Input.h"

void upgrade(GlobalGameState& ggs, int attr1, int attr2) {

}

RunMenu::RunMenu(GlobalGameState& ggs) : ggs(ggs) {
	inventoryMenu.setup(ggs.renderer);

	inventoryMenuTitle.setup(ggs.renderer);
	inventoryMenuTitle.loadFromRenderedText("Remove/Move Augments",ggs.white,ggs.title);

	int spacing = (WINDOW_WIDTH-scaleUI(120*4))/5;
	int height = scaleUI(40)*2+inventoryMenuTitle.getHeight();
	int bt1 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing,height,"ui/upgrade-revolver.png", &upgrade, Weapon_Type::revolver),-1,-1,-1,-1);
	int bt2 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*2 + scaleUI(120),height,"ui/upgrade-shotgun.png", &upgrade, Weapon_Type::shotgun),-1,-1,bt1,-1);
	int bt3 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*3 + scaleUI(120*2),height,"ui/upgrade-rifle.png", &upgrade, Weapon_Type::rifle),-1,-1,bt2,-1);
	inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*4 + scaleUI(120*3),height,"ui/upgrade-laser-pistol.png", &upgrade, Weapon_Type::laserPistol),-1,-1,bt3,-1);

	int revolverAug1 = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160), &upgrade), bt1, -1,-1,-1);
	inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40),ggs.clipIncrease1, &upgrade), revolverAug1, -1,-1,-1);

	inventoryMenu.addTitle((WINDOW_WIDTH-inventoryMenuTitle.getWidth())/2, scaleUI(40), inventoryMenuTitle);
}

void RunMenu::render() {
	if(currentMenu != nullptr) {
		currentMenu->render();
	}
}

void RunMenu::changeMenu(RunMenus menu) {
	switch(menu) {
		case RunMenus::pause:
			break;
		case RunMenus::inventory:
			currentMenuState = RunMenus::inventory;
			currentMenu = &inventoryMenu;
			break;
		case RunMenus::moveAugment:
			break;
		case RunMenus::newAugment:
			break;
	}
}

void RunMenu::readInput() {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
        	if(e.key.keysym.sym == SDLK_ESCAPE && currentMenuState == RunMenus::inventory) {
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
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
        	if(currentMenu != nullptr) {
        		controllerEvent(*currentMenu,MENU_CONTROL::disconnect);
        	}
            ggs.controller = nullptr;
        } else if( e.type == SDL_MOUSEMOTION && currentMenu != nullptr) {
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


