#include "../../includes/HEng/RunMenu.h"
#include "../../includes/HEng/Weapon.h"
#include "../../includes/TEng/Buttons.h"
#include "../../includes/Utils/Input.h"
#include "../../includes/Entities/Player.h"

void removeAugment(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.augmentRemoved = true;
	ggs.weaponChanged = attr1;
	ggs.slotChanged = attr2;
}

void addAugment(GlobalGameState& ggs, int attr1, int attr2) {
	//ggs.addedAugment = true;
	//ggs.slotAdded = attr1;
}

void selectAugmentToMove(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.startMoveAugment = true;
	ggs.weaponChanged = attr1;
	ggs.slotChanged = attr2;
}

void moveAugment(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.moveAugment = true;
	ggs.weaponMovedTo = attr1;
	ggs.slotMovedTo = attr2;
}

void noActionAug(GlobalGameState& ggs, int attr1, int attr2) {

}

RunMenu::RunMenu(GlobalGameState& ggs, Player& player) : ggs(ggs), player(player) {

	inventoryMenu.setup(ggs.renderer);
	inventoryMenuTitle.setup(ggs.renderer);
	inventoryMenuTitle.loadFromRenderedText("Remove/Move Augments",ggs.white,ggs.title);
	inventoryMenu.addTitle((WINDOW_WIDTH-inventoryMenuTitle.getWidth())/2, scaleUI(30), inventoryMenuTitle);

	spacing = (WINDOW_WIDTH-scaleUI(120*4))/5;
	height = scaleUI(30)*2+inventoryMenuTitle.getHeight();

	player.revolver.changePrimaryAugment(&ggs.damage1);
	player.shotgun.changeSecondaryAugment(&ggs.reload3);

	initBaseInventoryMenu();

}

Weapon* getWeapon(Player& player, const int weapon) {
	switch (weapon) {
		default:
		case revolver:
			return &player.revolver;
		case shotgun:
			return &player.shotgun;
		case rifle:
			return &player.rifle;
		case laserPistol:
			return &player.laserPistol;
	}
}

void RunMenu::run() {
	if(ggs.augmentRemoved) {
		if(ggs.slotChanged == 0) {
			getWeapon(player,ggs.weaponChanged)->removePrimaryAugment();
		} else {
			getWeapon(player,ggs.weaponChanged)->removeSecondaryAugment();
		}
		initBaseInventoryMenu();
		ggs.augmentRemoved = false;
	}

	if(ggs.startMoveAugment) {
		if(ggs.slotChanged == 0) {
			ggs.newAugment = getWeapon(player,ggs.weaponChanged)->getPrimaryAugment();
		} else {
			ggs.newAugment = getWeapon(player,ggs.weaponChanged)->getSecondaryAugment();
		}
		initMoveInventoryMenu();
		ggs.startMoveAugment = false;
	}

	if(ggs.moveAugment) {
		if(ggs.slotMovedTo == 0) {
			getWeapon(player,ggs.weaponMovedTo)->changePrimaryAugment(ggs.newAugment);
		} else {
			getWeapon(player,ggs.weaponMovedTo)->changeSecondaryAugment(ggs.newAugment);
		}
		if(ggs.slotChanged == 0) {
			getWeapon(player,ggs.weaponChanged)->removePrimaryAugment();
		} else {
			getWeapon(player,ggs.weaponChanged)->removeSecondaryAugment();
		}
		initBaseInventoryMenu();
		ggs.newAugment = nullptr;
		ggs.moveAugment = false;
	}
}

void RunMenu::render() const {
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
        	if (e.button.button == SDL_BUTTON_LEFT) {
        		if(currentMenu != nullptr) {
        			menuSelect(*currentMenu);
        		}
        	} else if (e.button.button == SDL_BUTTON_RIGHT) {
        		if(currentMenu != nullptr) {
        			menuSecondarySelect(*currentMenu);
        		}
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

void RunMenu::initBaseInventoryMenu() {
	inventoryMenu.reset();

	int verticalSpacing = scaleUI(20);
	int column1 = scaleUI(110);
	int column2 = scaleUI(110*2)+AugButton::getStaticWidth();

	int row1 = height;
	int row2 = height+AugButton::getStaticHeight()+verticalSpacing;
	int row3 = row2+AugButton::getStaticHeight()+verticalSpacing+20;
	int row4 = row3+AugButton::getStaticHeight()+verticalSpacing;

	if(player.shotgun.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3,player.shotgun.getPrimaryAugment(), &selectAugmentToMove,shotgun, 0 ,&removeAugment, shotgun,0), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3, &noActionAug), -1, -1,-1,-1);
	}
	if(player.shotgun.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, player.shotgun.getSecondaryAugment(), &selectAugmentToMove,shotgun, 1 ,&removeAugment, shotgun,1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, &noActionAug), -1, -1,-1,-1);
	}
	if(player.rifle.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1,player.rifle.getPrimaryAugment(), &selectAugmentToMove,rifle, 0 ,&removeAugment, rifle,0), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1, &noActionAug), -1, -1,-1,-1);
	}
	if(player.rifle.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, player.rifle.getSecondaryAugment(), &selectAugmentToMove,rifle, 1 ,&removeAugment, rifle,1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, &noActionAug), -1, -1,-1,-1);
	}
	if(player.laserPistol.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3,player.laserPistol.getPrimaryAugment(), &selectAugmentToMove,laserPistol, 0 ,&removeAugment, Weapon_Type::laserPistol,0), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3, &noActionAug), -1, -1,-1,-1);
	}
	if(player.laserPistol.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, player.laserPistol.getSecondaryAugment(), &selectAugmentToMove,laserPistol, 1 ,&removeAugment, Weapon_Type::laserPistol,1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, &noActionAug), -1, -1,-1,-1);
	}
	if(player.revolver.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1,player.revolver.getPrimaryAugment(), &selectAugmentToMove,revolver, 0 ,&removeAugment, revolver,0), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1, &noActionAug), -1, -1,-1,-1);
	}
	if(player.revolver.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, player.revolver.getSecondaryAugment(), &selectAugmentToMove,revolver, 1 ,&removeAugment, revolver,1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, &noActionAug), -1, -1,-1,-1);
	}
}

void RunMenu::initMoveInventoryMenu() {
	inventoryMenu.reset();

	int verticalSpacing = scaleUI(20);
	int column1 = scaleUI(110);
	int column2 = scaleUI(110*2)+AugButton::getStaticWidth();

	int row1 = height;
	int row2 = height+AugButton::getStaticHeight()+verticalSpacing;
	int row3 = row2+AugButton::getStaticHeight()+verticalSpacing+20;
	int row4 = row3+AugButton::getStaticHeight()+verticalSpacing;

	if(player.shotgun.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3,player.shotgun.getPrimaryAugment(), &moveAugment,shotgun, 0), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3, &moveAugment,shotgun, 0), -1, -1,-1,-1);
	}
	if(player.shotgun.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, player.shotgun.getSecondaryAugment(), &moveAugment,shotgun, 1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, &moveAugment, shotgun, 1), -1, -1,-1,-1);
	}
	if(player.rifle.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1,player.rifle.getPrimaryAugment(), &moveAugment,rifle, 0), -1, -1,-1,-1);
	} else {
		 inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1, &moveAugment, rifle ,0), -1, -1,-1,-1);
	}
	if(player.rifle.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, player.rifle.getSecondaryAugment(), &moveAugment,rifle, 1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, &moveAugment, rifle, 1), -1, -1,-1,-1);
	}
	if(player.laserPistol.getPrimaryAugment() != nullptr) {
		 inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3,player.laserPistol.getPrimaryAugment(), &moveAugment,laserPistol, 0), -1, -1,-1,-1);
	} else {
		 inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3, &moveAugment, laserPistol, 0), -1, -1,-1,-1);
	}
	if(player.laserPistol.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, player.laserPistol.getSecondaryAugment(), &moveAugment,laserPistol, 1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, &moveAugment,laserPistol,1), -1, -1,-1,-1);
	}
	if(player.revolver.getPrimaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1,player.revolver.getPrimaryAugment(), &moveAugment,revolver, 0), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1, &moveAugment, revolver, 0), -1, -1,-1,-1);
	}
	if(player.revolver.getSecondaryAugment() != nullptr) {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, player.revolver.getSecondaryAugment(), &moveAugment,revolver, 1), -1, -1,-1,-1);
	} else {
		inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, &moveAugment, revolver, 1), -1, -1,-1,-1);
	}
}