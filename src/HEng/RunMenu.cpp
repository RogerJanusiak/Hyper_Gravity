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
	ggs.addedAugment = true;
	ggs.weaponChanged = attr1;
	ggs.slotChanged = attr2;
}

void selectAugmentToMove(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.startMoveAugment = true;
	ggs.weaponChanged = attr1;
	ggs.slotChanged = attr2;
}

void selectAugmentToMoveFromNew(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.startMoveAugment = true;
	ggs.fromNew = true;
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
	inventoryMenuTitle.loadFromRenderedText("Edit Augments",ggs.white,ggs.title);

	moveTitle.setup(ggs.renderer);
	moveTitle.loadFromRenderedText("Select New Slot for Augment",ggs.white,ggs.title);

	newTitle.setup(ggs.renderer);
	newTitle.loadFromRenderedText("Select Slot for: ",ggs.white,ggs.title);

	height = scaleUI(30)*2+inventoryMenuTitle.getHeight();

	row1 = height;
	row2 = height+AugButton::getStaticHeight()+verticalSpacing;
	row3 = row2+AugButton::getStaticHeight()+verticalSpacing+20;
	row4 = row3+AugButton::getStaticHeight()+verticalSpacing;

	revolverTexture.setup(scaleUI(64),scaleUI(34), ggs.renderer);
	revolverTexture.loadFromFile("revolver.png");

	shotgunTexture.setup(scaleUI(64),scaleUI(21), ggs.renderer);
	shotgunTexture.loadFromFile("shotgun.png");

	rifleTexture.setup(scaleUI(64),scaleUI(24), ggs.renderer);
	rifleTexture.loadFromFile("rifle.png");

	laserPistolTexture.setup(scaleUI(60),scaleUI(42), ggs.renderer);
	laserPistolTexture.loadFromFile("laserPistol.png");

	aButton.setup(scaleUI(18), scaleUI(18), ggs.renderer);
	aButton.loadFromFile("ui/abutton.png");
	xButton.setup(scaleUI(18), scaleUI(18), ggs.renderer);
	xButton.loadFromFile("ui/xbutton.png");

	leftClickText.setup(ggs.renderer);
	leftClickText.loadFromRenderedText("L Click:", ggs.white, ggs.augTitle);
	rightClickText.setup(ggs.renderer);
	rightClickText.loadFromRenderedText("R Click:", ggs.white, ggs.augTitle);

	moveAugText.setup(ggs.renderer);
	moveAugText.loadFromRenderedText("Move Aug", ggs.white, ggs.augTitle);
	removeAugText.setup(ggs.renderer);
	removeAugText.loadFromRenderedText("Remove Aug", ggs.white, ggs.augTitle);
	selectSlotText.setup(ggs.renderer);
	selectSlotText.loadFromRenderedText("Equip Aug", ggs.white, ggs.augTitle);

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
		fromAnotherMenu = true;
		initBaseInventoryMenu();
		ggs.augmentRemoved = false;
	}

	if(ggs.startMoveAugment) {
		if(ggs.slotChanged == 0) {
			ggs.augmentToMove = getWeapon(player,ggs.weaponChanged)->getPrimaryAugment();
		} else {
			ggs.augmentToMove = getWeapon(player,ggs.weaponChanged)->getSecondaryAugment();
		}
		initMoveInventoryMenu();
		ggs.startMoveAugment = false;
	}

	if(ggs.moveAugment) {
		if(ggs.slotChanged == 0) {
			getWeapon(player,ggs.weaponChanged)->removePrimaryAugment();
		} else {
			getWeapon(player,ggs.weaponChanged)->removeSecondaryAugment();
		}
		if(ggs.slotMovedTo == 0) {
			getWeapon(player,ggs.weaponMovedTo)->changePrimaryAugment(ggs.augmentToMove);
		} else {
			getWeapon(player,ggs.weaponMovedTo)->changeSecondaryAugment(ggs.augmentToMove);
		}
		if(ggs.fromNew) {
			fromMoveMenu = true;
			initNewInventoryMenu();
			ggs.fromNew = false;
		} else {
			fromMoveMenu = true;
			initBaseInventoryMenu();
		}

		ggs.augmentToMove = nullptr;
		ggs.moveAugment = false;

	}

	if(newAugment) {
		initNewInventoryMenu();
		newAugment = false;
	}

	if(ggs.addedAugment) {
		if(ggs.slotChanged == 0) {
			getWeapon(player,ggs.weaponChanged)->changePrimaryAugment(ggs.newAugment);
		} else {
			getWeapon(player,ggs.weaponChanged)->changeSecondaryAugment(ggs.newAugment);
		}
		ggs.addedAugment = false;
		fromAnotherMenu = true;
		initBaseInventoryMenu();
		ggs.newAugment = nullptr;
	}

}

void RunMenu::render() const {
	if(currentMenu != nullptr) {
		currentMenu->render();
	}

	SDL_SetRenderDrawColor(ggs.renderer, 255, 255, 255, 255); // White

	int revolverX = column1-revolverTexture.getWidth()-scaleUI(40);
	int revolverY = row1+(AugButton::getStaticHeight()*2+verticalSpacing-revolverTexture.getHeight())/2;
	SDL_Point revolverPoints1[4] = {{revolverX+revolverTexture.getWidth()/2,revolverY+revolverTexture.getHeight()/2},{revolverX+revolverTexture.getWidth()+scaleUI(20),revolverY+revolverTexture.getHeight()/2},{revolverX+revolverTexture.getWidth()+scaleUI(20),row1+AugButton::getStaticHeight()/2},{column1,row1+AugButton::getStaticHeight()/2}};
	SDL_Point revolverPoints2[4] = {{revolverX+revolverTexture.getWidth()/2,revolverY+revolverTexture.getHeight()/2},{revolverX+revolverTexture.getWidth()+scaleUI(20),revolverY+revolverTexture.getHeight()/2},{revolverX+revolverTexture.getWidth()+scaleUI(20),row2+AugButton::getStaticHeight()/2},{column1,row2+AugButton::getStaticHeight()/2}};
	SDL_RenderDrawLines(ggs.renderer, revolverPoints1, 4);
	SDL_RenderDrawLines(ggs.renderer, revolverPoints2, 4);

	int shotgunX = column1-shotgunTexture.getWidth()-scaleUI(40);
	int shotgunY = row3+(AugButton::getStaticHeight()*2+verticalSpacing-shotgunTexture.getHeight())/2;
	SDL_Point shotgunPoints1[4] = {{shotgunX+shotgunTexture.getWidth()/2,shotgunY+shotgunTexture.getHeight()/2},{shotgunX+shotgunTexture.getWidth()+scaleUI(20),shotgunY+shotgunTexture.getHeight()/2},{shotgunX+shotgunTexture.getWidth()+scaleUI(20),row3+AugButton::getStaticHeight()/2},{column1,row3+AugButton::getStaticHeight()/2}};
	SDL_Point shotgunPoints2[4] = {{shotgunX+shotgunTexture.getWidth()/2,shotgunY+shotgunTexture.getHeight()/2},{shotgunX+shotgunTexture.getWidth()+scaleUI(20),shotgunY+shotgunTexture.getHeight()/2},{shotgunX+shotgunTexture.getWidth()+scaleUI(20),row4+AugButton::getStaticHeight()/2},{column1,row4+AugButton::getStaticHeight()/2}};
	SDL_RenderDrawLines(ggs.renderer, shotgunPoints1, 4);
	SDL_RenderDrawLines(ggs.renderer, shotgunPoints2, 4);

	int rifleX =column2+AugButton::getStaticWidth()+scaleUI(40);
	int rifleY = row1+(AugButton::getStaticHeight()*2+verticalSpacing-rifleTexture.getHeight())/2;
	SDL_Point riflePoints1[4] = {{rifleX+rifleTexture.getWidth()/2,rifleY+rifleTexture.getHeight()/2},{rifleX-scaleUI(20),rifleY+rifleTexture.getHeight()/2},{rifleX-scaleUI(20),row1+AugButton::getStaticHeight()/2},{column2+AugButton::getStaticWidth(),row1+AugButton::getStaticHeight()/2}};
	SDL_Point riflePoints2[4] = {{rifleX+rifleTexture.getWidth()/2,rifleY+rifleTexture.getHeight()/2},{rifleX-scaleUI(20),rifleY+rifleTexture.getHeight()/2},{rifleX-scaleUI(20),row2+AugButton::getStaticHeight()/2},{column2+AugButton::getStaticWidth(),row2+AugButton::getStaticHeight()/2}};
	SDL_RenderDrawLines(ggs.renderer, riflePoints1, 4);
	SDL_RenderDrawLines(ggs.renderer, riflePoints2, 4);

	int laserPistolX =column2+AugButton::getStaticWidth()+scaleUI(40);
	int laserPistolY = row3+(AugButton::getStaticHeight()*2+verticalSpacing-laserPistolTexture.getHeight())/2;
	SDL_Point laserPistolPoints1[4] = {{laserPistolX+laserPistolTexture.getWidth()/2,laserPistolY+laserPistolTexture.getHeight()/2},{laserPistolX-scaleUI(20),laserPistolY+laserPistolTexture.getHeight()/2},{laserPistolX-scaleUI(20),row3+AugButton::getStaticHeight()/2},{column2+AugButton::getStaticWidth(),row3+AugButton::getStaticHeight()/2}};
	SDL_Point laserPistolPoints2[4] = {{laserPistolX+laserPistolTexture.getWidth()/2,laserPistolY+laserPistolTexture.getHeight()/2},{laserPistolX-scaleUI(20),laserPistolY+laserPistolTexture.getHeight()/2},{laserPistolX-scaleUI(20),row4+AugButton::getStaticHeight()/2},{column2+AugButton::getStaticWidth(),row4+AugButton::getStaticHeight()/2}};
	SDL_RenderDrawLines(ggs.renderer, laserPistolPoints1, 4);
	SDL_RenderDrawLines(ggs.renderer, laserPistolPoints2, 4);

	revolverTexture.render(revolverX,revolverY);
	shotgunTexture.render(shotgunX,shotgunY);
	rifleTexture.render(rifleX,rifleY);
	laserPistolTexture.render(laserPistolX,laserPistolY);

	if(ggs.controller != nullptr) {
		if(currentScreen == view) {
			aButton.render(scaleUI(10), scaleUI(10));
			xButton.render(scaleUI(10), scaleUI(20)+aButton.getHeight());
			moveAugText.render(scaleUI(15)+aButton.getWidth(),scaleUI(9)+aButton.getHeight()/2-moveAugText.getHeight()/2);
			removeAugText.render(scaleUI(15)+xButton.getWidth(),scaleUI(19)+aButton.getHeight()+xButton.getHeight()/2-removeAugText.getHeight()/2);
		} else if(currentScreen == move) {
			aButton.render(scaleUI(10), scaleUI(10));
			selectSlotText.render(scaleUI(15)+aButton.getWidth(),scaleUI(9)+aButton.getHeight()/2-selectSlotText.getHeight()/2);
		} else if(currentScreen == newAug) {
			aButton.render(scaleUI(10), scaleUI(10));
			xButton.render(scaleUI(10), scaleUI(20)+aButton.getHeight());
			selectSlotText.render(scaleUI(15)+aButton.getWidth(),scaleUI(9)+aButton.getHeight()/2-selectSlotText.getHeight()/2);
			moveAugText.render(scaleUI(15)+xButton.getWidth(),scaleUI(19)+aButton.getHeight()+xButton.getHeight()/2-moveAugText.getHeight()/2);
		}
	} else {
		if(currentScreen == view) {
			leftClickText.render(scaleUI(10), scaleUI(10));
			rightClickText.render(scaleUI(10), scaleUI(20)+leftClickText.getHeight());
			moveAugText.render(scaleUI(15)+leftClickText.getWidth(),scaleUI(10));
			removeAugText.render(scaleUI(15)+rightClickText.getWidth(),scaleUI(20)+leftClickText.getHeight());
		} else if(currentScreen == move) {
			leftClickText.render(scaleUI(10), scaleUI(10));
			selectSlotText.render(scaleUI(15)+leftClickText.getWidth(),scaleUI(10));
		} else if(currentScreen == newAug) {
			leftClickText.render(scaleUI(10), scaleUI(10));
			rightClickText.render(scaleUI(10), scaleUI(20)+leftClickText.getHeight());
			selectSlotText.render(scaleUI(15)+leftClickText.getWidth(),scaleUI(10));
			moveAugText.render(scaleUI(15)+rightClickText.getWidth(),scaleUI(20)+leftClickText.getHeight());
		}
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
        	if((e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_e) && currentMenuState == RunMenus::inventory) {
        		currentMenu = nullptr;
        		ggs.currentRunState = RunState::inWave;
        		player.revolver.upgrade();
        		player.shotgun.upgrade();
        		player.rifle.upgrade();
        		player.laserPistol.upgrade();
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
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1 || SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1 || SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK) == 1) {
            	currentMenu = nullptr;
            	ggs.currentRunState = RunState::inWave;
            	player.revolver.upgrade();
            	player.shotgun.upgrade();
            	player.rifle.upgrade();
            	player.laserPistol.upgrade();

            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) == 1) {
            	if(currentMenu != nullptr) {
            		controllerEvent(*currentMenu,MENU_CONTROL::secondarySelect);
            	}
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
            } else if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) {
        		if(ggs.controllerStickReset) {
        			if(currentMenu != nullptr) {
        				controllerEvent(*currentMenu,MENU_CONTROL::right);
        			}
        			ggs.controllerStickReset = false;
        		}
        	} else if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) {
        		if(ggs.controllerStickReset) {
        			if(currentMenu != nullptr) {
        				controllerEvent(*currentMenu,MENU_CONTROL::left);
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
	currentScreen = view;
	inventoryMenu.addTitle((WINDOW_WIDTH-inventoryMenuTitle.getWidth())/2, scaleUI(30), inventoryMenuTitle);

	int rp, rs, sp, ss, rip, ris, lp, ls;

	if(player.revolver.getPrimaryAugment() != nullptr) {
		rp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1,player.revolver.getPrimaryAugment(), &selectAugmentToMove,revolver, 0 ,&removeAugment, revolver,0), -1, -1,-1,-1);
	} else {
		rp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1, &noActionAug), -1, -1,-1,-1);
	}
	if(player.revolver.getSecondaryAugment() != nullptr) {
		rs = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, player.revolver.getSecondaryAugment(), &selectAugmentToMove,revolver, 1 ,&removeAugment, revolver,1), rp, -1,-1,-1);
	} else {
		rs = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, &noActionAug), rp, -1,-1,-1);
	}
	if(player.shotgun.getPrimaryAugment() != nullptr) {
		sp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3,player.shotgun.getPrimaryAugment(), &selectAugmentToMove,shotgun, 0 ,&removeAugment, shotgun,0), rs, -1,-1,-1);
	} else {
		sp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3, &noActionAug), rs, -1,-1,-1);
	}
	if(player.shotgun.getSecondaryAugment() != nullptr) {
		ss = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, player.shotgun.getSecondaryAugment(), &selectAugmentToMove,shotgun, 1 ,&removeAugment, shotgun,1), sp, -1,-1,-1);
	} else {
		ss = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, &noActionAug), sp, -1,-1,-1);
	}
	if(player.rifle.getPrimaryAugment() != nullptr) {
		rip = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1,player.rifle.getPrimaryAugment(), &selectAugmentToMove,rifle, 0 ,&removeAugment, rifle,0), -1, -1,rp,-1);
	} else {
		rip = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1, &noActionAug), -1, -1,rp,-1);
	}
	if(player.rifle.getSecondaryAugment() != nullptr) {
		ris = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, player.rifle.getSecondaryAugment(), &selectAugmentToMove,rifle, 1 ,&removeAugment, rifle,1), rip, -1,rs,-1);
	} else {
		ris = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, &noActionAug), rip, -1,rs,-1);
	}
	if(player.laserPistol.getPrimaryAugment() != nullptr) {
		lp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3,player.laserPistol.getPrimaryAugment(), &selectAugmentToMove,laserPistol, 0 ,&removeAugment, Weapon_Type::laserPistol,0), ris, -1,sp,-1);
	} else {
		lp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3, &noActionAug), ris, -1,sp,-1);
	}
	if(player.laserPistol.getSecondaryAugment() != nullptr) {
		ls = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, player.laserPistol.getSecondaryAugment(), &selectAugmentToMove,laserPistol, 1 ,&removeAugment, Weapon_Type::laserPistol,1), lp, -1,ss,-1);
	} else {
		ls = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, &noActionAug), lp, -1,ss,-1);
	}

	if(ggs.controller != nullptr) {
		if(fromAnotherMenu) {
			int button = 0;
			switch(ggs.weaponChanged) {
			default:
			case revolver:
				button = ggs.slotChanged == 0 ? rp : rs;
				break;
			case shotgun:
				button = ggs.slotChanged == 0 ? sp : ss;
				break;
			case rifle:
				button = ggs.slotChanged == 0 ? rip : ris;
				break;
			case laserPistol:
				button = ggs.slotChanged == 0 ? lp : ls;
				break;
			}
			inventoryMenu.setSelected(button);
			fromAnotherMenu = false;
		} else if(fromMoveMenu) {
			int button = 0;
			switch(ggs.weaponMovedTo) {
			default:
			case revolver:
				button = ggs.slotMovedTo == 0 ? rp : rs;
				break;
			case shotgun:
				button = ggs.slotMovedTo == 0 ? sp : ss;
				break;
			case rifle:
				button = ggs.slotMovedTo == 0 ? rip : ris;
				break;
			case laserPistol:
				button = ggs.slotMovedTo == 0 ? lp : ls;
				break;
			}
			inventoryMenu.setSelected(button);
			fromMoveMenu = false;
		} else {
			inventoryMenu.loadMenu();
		}
	}
}

void RunMenu::initMoveInventoryMenu() {
	inventoryMenu.reset();
	currentScreen = move;
	int rp, rs, sp, ss, rip, ris, lp, ls;

	inventoryMenu.addTitle((WINDOW_WIDTH-moveTitle.getWidth())/2, scaleUI(30), moveTitle);

	if(player.revolver.getPrimaryAugment() != nullptr) {
		rp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1,player.revolver.getPrimaryAugment(), &moveAugment,revolver, 0), -1, -1,-1,-1);
	} else {
		rp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1, &moveAugment, revolver, 0), -1, -1,-1,-1);
	}
	if(player.revolver.getSecondaryAugment() != nullptr) {
		rs = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, player.revolver.getSecondaryAugment(), &moveAugment,revolver, 1), rp, -1,-1,-1);
	} else {
		rs = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, &moveAugment, revolver, 1), rp, -1,-1,-1);
	}
	if(player.shotgun.getPrimaryAugment() != nullptr) {
		sp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3,player.shotgun.getPrimaryAugment(), &moveAugment,shotgun, 0), rs, -1,-1,-1);
	} else {
		sp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3, &moveAugment,shotgun, 0), rs, -1,-1,-1);
	}
	if(player.shotgun.getSecondaryAugment() != nullptr) {
		ss = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, player.shotgun.getSecondaryAugment(), &moveAugment,shotgun, 1), sp, -1,-1,-1);
	} else {
		ss = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, &moveAugment, shotgun, 1), sp, -1,-1,-1);
	}
	if(player.rifle.getPrimaryAugment() != nullptr) {
		rip = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1,player.rifle.getPrimaryAugment(), &moveAugment,rifle, 0), -1, -1,rp,-1);
	} else {
		rip = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1, &moveAugment, rifle ,0), -1, -1,rp,-1);
	}
	if(player.rifle.getSecondaryAugment() != nullptr) {
		ris = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, player.rifle.getSecondaryAugment(), &moveAugment,rifle, 1), rip, -1,rs,-1);
	} else {
		ris = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, &moveAugment, rifle, 1), rip, -1,rs,-1);
	}
	if(player.laserPistol.getPrimaryAugment() != nullptr) {
		 lp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3,player.laserPistol.getPrimaryAugment(), &moveAugment,laserPistol, 0), ris, -1,sp,-1);
	} else {
		 lp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3, &moveAugment, laserPistol, 0), ris, -1,sp,-1);
	}
	if(player.laserPistol.getSecondaryAugment() != nullptr) {
		ls = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, player.laserPistol.getSecondaryAugment(), &moveAugment,laserPistol, 1), lp, -1,ss,-1);
	} else {
		ls = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, &moveAugment,laserPistol,1), lp, -1,ss,-1);
	}

	if(ggs.controller != nullptr) {
		int button = 0;
		switch(ggs.weaponChanged) {
		default:
		case revolver:
			button = ggs.slotChanged == 0 ? rp : rs;
			break;
		case shotgun:
			button = ggs.slotChanged == 0 ? sp : ss;
			break;
		case rifle:
			button = ggs.slotChanged == 0 ? rip : ris;
			break;
		case laserPistol:
			button = ggs.slotChanged == 0 ? lp : ls;
			break;
		}
		inventoryMenu.setSelected(button);
	}

}

void RunMenu::initNewInventoryMenu() {
	inventoryMenu.reset();
	currentScreen = newAug;
	int rp, rs, sp, ss, rip, ris, lp, ls;

	inventoryMenu.addTitle((WINDOW_WIDTH-newTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2, scaleUI(30), newTitle);

	if(player.revolver.getPrimaryAugment() != nullptr) {
		rp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1,player.revolver.getPrimaryAugment(), &addAugment,revolver, 0,&selectAugmentToMoveFromNew,revolver, 0), -1, -1,-1,-1);
	} else {
		rp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row1, &addAugment, revolver, 0), -1, -1,-1,-1);
	}
	if(player.revolver.getSecondaryAugment() != nullptr) {
		rs = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, player.revolver.getSecondaryAugment(), &addAugment,revolver, 1,&selectAugmentToMoveFromNew,revolver, 1), rp, -1,-1,-1);
	} else {
		rs = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row2, &addAugment, revolver, 1), rp, -1,-1,-1);
	}
	if(player.shotgun.getPrimaryAugment() != nullptr) {
		sp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3,player.shotgun.getPrimaryAugment(), &addAugment,shotgun, 0,&selectAugmentToMoveFromNew,shotgun, 0), rs, -1,-1,-1);
	} else {
		sp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row3, &addAugment,shotgun, 0), rs, -1,-1,-1);
	}
	if(player.shotgun.getSecondaryAugment() != nullptr) {
		ss = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, player.shotgun.getSecondaryAugment(), &addAugment,shotgun, 1,&selectAugmentToMoveFromNew,shotgun, 1), sp, -1,-1,-1);
	} else {
		ss = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column1,row4, &addAugment, shotgun, 1), sp, -1,-1,-1);
	}
	if(player.rifle.getPrimaryAugment() != nullptr) {
		rip = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1,player.rifle.getPrimaryAugment(), &addAugment,rifle, 0,&selectAugmentToMoveFromNew,rifle, 0), -1, -1,rp,-1);
	} else {
		rip = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row1, &addAugment, rifle ,0), -1, -1,rp,-1);
	}
	if(player.rifle.getSecondaryAugment() != nullptr) {
		ris = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, player.rifle.getSecondaryAugment(), &addAugment,rifle, 1,&selectAugmentToMoveFromNew,rifle, 1), rip, -1,rs,-1);
	} else {
		ris = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row2, &addAugment, rifle, 1), rip, -1,rs,-1);
	}
	if(player.laserPistol.getPrimaryAugment() != nullptr) {
		 lp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3,player.laserPistol.getPrimaryAugment(), &addAugment,laserPistol, 0,&selectAugmentToMoveFromNew,laserPistol, 0), ris, -1,sp,-1);
	} else {
		 lp = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row3, &addAugment, laserPistol, 0), ris, -1,sp,-1);
	}
	if(player.laserPistol.getSecondaryAugment() != nullptr) {
		ls = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, player.laserPistol.getSecondaryAugment(), &addAugment,laserPistol, 1,&selectAugmentToMoveFromNew,laserPistol, 1), lp, -1,ss,-1);
	} else {
		ls = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,column2,row4, &addAugment,laserPistol,1), lp, -1,ss,-1);
	}

	inventoryMenu.addButton(std::make_unique<AugButton>(ggs,(WINDOW_WIDTH-newTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2 + newTitle.getWidth() + scaleUI(30),scaleUI(15),ggs.newAugment, &noActionAug), -1, -1,-1,-1);

	if(ggs.controller != nullptr) {
		int button = 0;
		switch(ggs.weaponMovedTo) {
		default:
		case revolver:
			button = ggs.slotMovedTo == 0 ? rp : rs;
			break;
		case shotgun:
			button = ggs.slotMovedTo == 0 ? sp : ss;
			break;
		case rifle:
			button = ggs.slotMovedTo == 0 ? rip : ris;
			break;
		case laserPistol:
			button = ggs.slotMovedTo == 0 ? lp : ls;
			break;
		}
		inventoryMenu.setSelected(button);
		fromMoveMenu = false;
	}
}
