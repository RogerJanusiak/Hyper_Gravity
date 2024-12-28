#include "../../includes/HEng/RunMenu.h"
#include "../../includes/HEng/Weapon.h"
#include "../../includes/TEng/Buttons.h"
#include "../../includes/Utils/Input.h"
#include "../../includes/Entities/Player.h"

void selectWeapon(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.weaponSelected = true;
	ggs.selectedWeapon = attr1;
}

void removeAugment(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.augmentRemoved = true;
	ggs.removedAugment = attr1;
}

void addAugment(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.addedAugment = true;
	ggs.slotAdded = attr1;
}

void moveAugment(GlobalGameState& ggs, int attr1, int attr2) {
	ggs.moveAugment = true;
	ggs.slotAdded = attr2;
	ggs.augmentToMove = getAugment(ggs, attr1);
}

void noActionAug(GlobalGameState& ggs, int attr1, int attr2) {

}

RunMenu::RunMenu(GlobalGameState& ggs, Player& player) : ggs(ggs), player(player) {

	inventoryMenu.setup(ggs.renderer);
	inventoryMenuTitle.setup(ggs.renderer);
	inventoryMenuTitle.loadFromRenderedText("Remove/Move Augments",ggs.white,ggs.title);
	inventoryMenu.addTitle((WINDOW_WIDTH-inventoryMenuTitle.getWidth())/2, scaleUI(40), inventoryMenuTitle);

	newAugmentMenuTitle.setup(ggs.renderer);
	newAugmentMenuTitle.loadFromRenderedText("Equip Augment: ",ggs.white,ggs.title);

	moveAugmentMenuTitle.setup(ggs.renderer);
	moveAugmentMenuTitle.loadFromRenderedText("Move Augment: ",ggs.white,ggs.title);

	newAugmentMenu.setup(ggs.renderer);
	newAugmentMenu.addTitle((WINDOW_WIDTH-newAugmentMenuTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2, scaleUI(40), newAugmentMenuTitle);

	moveAugmentMenu.setup(ggs.renderer);
	moveAugmentMenu.addTitle((WINDOW_WIDTH-newAugmentMenuTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2, scaleUI(40), moveAugmentMenuTitle);

	spacing = (WINDOW_WIDTH-scaleUI(120*4))/5;
	height = scaleUI(40)*2+inventoryMenuTitle.getHeight();

	initBaseInventoryMenu();

}

void RunMenu::run() {
	if(currentMenuState == RunMenus::inventory) {
		if(ggs.weaponSelected) {
			ggs.weaponSelected = false;
			initBaseInventoryMenu();
			switch (ggs.selectedWeapon) {
			default:
				loadWeaponAugments(revolver);
				break;
			case rifle:
				loadWeaponAugments(rifle);
				break;
			case shotgun:
				loadWeaponAugments(shotgun);
				break;
			case laserPistol:
				loadWeaponAugments(laserPistol);
				break;
			}
		} else if(ggs.augmentRemoved) {
			ggs.augmentRemoved = false;
			initBaseInventoryMenu();
			switch (ggs.selectedWeapon) {
			default:
				if(ggs.removedAugment == 0) {
					player.revolver.removePrimaryAugment();
				} else {
					player.revolver.removeSecondaryAugment();
				}
				loadWeaponAugments(revolver);
				break;
			case rifle:
				if(ggs.removedAugment == 0) {
					player.rifle.removePrimaryAugment();
				} else {
					player.rifle.removeSecondaryAugment();
				}
				loadWeaponAugments(rifle);
				break;
			case shotgun:
				if(ggs.removedAugment == 0) {
					player.shotgun.removePrimaryAugment();
				} else {
					player.shotgun.removeSecondaryAugment();
				}
				loadWeaponAugments(shotgun);
				break;
			case laserPistol:
				if(ggs.removedAugment == 0) {
					player.laserPistol.removePrimaryAugment();
				} else {
					player.laserPistol.removeSecondaryAugment();
				}
				loadWeaponAugments(laserPistol);
				break;
			}
		} else if(ggs.moveAugment) {
			switch (ggs.selectedWeapon) {
			default:
				if(ggs.slotAdded == 0) {
					player.revolver.removePrimaryAugment();
				} else {
					player.revolver.removeSecondaryAugment();
				}
				break;
			case rifle:
				if(ggs.slotAdded == 0) {
					player.rifle.removePrimaryAugment();
				} else {
					player.rifle.removeSecondaryAugment();
				}
				break;
			case shotgun:
				if(ggs.slotAdded == 0) {
					player.shotgun.removePrimaryAugment();
				} else {
					player.shotgun.removeSecondaryAugment();
				}
				break;
			case laserPistol:
				if(ggs.slotAdded == 0) {
					player.laserPistol.removePrimaryAugment();
				} else {
					player.laserPistol.removeSecondaryAugment();
				}
				break;
			}
			ggs.moveAugment = false;
			changeMenu(RunMenus::moveAugment);
			initMoveAugmentMenu();
		}
	}
	else if(currentMenuState == RunMenus::newAugment) {
		if(ggs.weaponSelected) {
			ggs.weaponSelected = false;
			initNewAugmentMenu();
			switch (ggs.selectedWeapon) {
			default:
				loadWeaponAugmentsNew(revolver);
				break;
			case rifle:
				loadWeaponAugmentsNew(rifle);
				break;
			case shotgun:
				loadWeaponAugmentsNew(shotgun);
				break;
			case laserPistol:
				loadWeaponAugmentsNew(laserPistol);
				break;
			}
		} else if(ggs.addedAugment) {
			ggs.addedAugment = false;
			changeMenu(RunMenus::inventory);
			initBaseInventoryMenu();
			switch (ggs.selectedWeapon) {
			default:
				if(ggs.slotAdded == 0) {
					player.revolver.changePrimaryAugment(ggs.newAugment);
				} else {
					player.revolver.changeSecondaryAugment(ggs.newAugment);
				}
				loadWeaponAugments(revolver);
				break;
			case rifle:
				if(ggs.slotAdded == 0) {
					player.rifle.changePrimaryAugment(ggs.newAugment);
				} else {
					player.rifle.changeSecondaryAugment(ggs.newAugment);
				}
				loadWeaponAugments(rifle);
				break;
			case shotgun:
				if(ggs.slotAdded == 0) {
					player.shotgun.changePrimaryAugment(ggs.newAugment);
				} else {
					player.shotgun.changeSecondaryAugment(ggs.newAugment);
				}
				loadWeaponAugments(shotgun);
				break;
			case laserPistol:
				if(ggs.slotAdded == 0) {
					player.laserPistol.changePrimaryAugment(ggs.newAugment);
				} else {
					player.laserPistol.changeSecondaryAugment(ggs.newAugment);
				}
				loadWeaponAugments(laserPistol);
				break;
			}
			ggs.newAugment = nullptr;
		}
	}
	else if(currentMenuState == RunMenus::moveAugment) {
		if(ggs.weaponSelected) {
			ggs.weaponSelected = false;
			initMoveAugmentMenu();
			switch (ggs.selectedWeapon) {
			default:
				loadWeaponAugmentsMove(revolver);
				break;
			case rifle:
				loadWeaponAugmentsMove(rifle);
				break;
			case shotgun:
				loadWeaponAugmentsMove(shotgun);
				break;
			case laserPistol:
				loadWeaponAugmentsMove(laserPistol);
				break;
			}
		} else if(ggs.addedAugment) {
			ggs.addedAugment = false;
			changeMenu(RunMenus::inventory);
			initBaseInventoryMenu();
			switch (ggs.selectedWeapon) {
			default:
				if(ggs.slotAdded == 0) {
					player.revolver.changePrimaryAugment(ggs.augmentToMove);
				} else {
					player.revolver.changeSecondaryAugment(ggs.augmentToMove);
				}
				loadWeaponAugments(revolver);
				break;
			case rifle:
				if(ggs.slotAdded == 0) {
					player.rifle.changePrimaryAugment(ggs.augmentToMove);
				} else {
					player.rifle.changeSecondaryAugment(ggs.augmentToMove);
				}
				loadWeaponAugments(rifle);
				break;
			case shotgun:
				if(ggs.slotAdded == 0) {
					player.shotgun.changePrimaryAugment(ggs.augmentToMove);
				} else {
					player.shotgun.changeSecondaryAugment(ggs.augmentToMove);
				}
				loadWeaponAugments(shotgun);
				break;
			case laserPistol:
				if(ggs.slotAdded == 0) {
					player.laserPistol.changePrimaryAugment(ggs.augmentToMove);
				} else {
					player.laserPistol.changeSecondaryAugment(ggs.augmentToMove);
				}
				loadWeaponAugments(laserPistol);
				break;
			}
			ggs.newAugment = nullptr;
		}
	}

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
		case RunMenus::newAugment:
			currentMenuState = RunMenus::newAugment;
			currentMenu = &newAugmentMenu;
			break;
		case RunMenus::moveAugment:
			currentMenuState = RunMenus::moveAugment;
			currentMenu = &moveAugmentMenu;
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
	bt1 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing,height,"ui/upgrade-revolver.png", &selectWeapon, Weapon_Type::revolver),-1,-1,-1,-1);
	bt2 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*2 + scaleUI(120),height,"ui/upgrade-shotgun.png", &selectWeapon, Weapon_Type::shotgun),-1,-1,bt1,-1);
	bt3 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*3 + scaleUI(120*2),height,"ui/upgrade-rifle.png", &selectWeapon, Weapon_Type::rifle),-1,-1,bt2,-1);
	bt4 = inventoryMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*4 + scaleUI(120*3),height,"ui/upgrade-laser-pistol.png", &selectWeapon, Weapon_Type::laserPistol),-1,-1,bt3,-1);
}

void RunMenu::loadWeaponAugments(Weapon_Type weapon) {
	switch (weapon) {
		case shotgun: {
			int shotgunAug1;
			if(player.shotgun.getPrimaryAugment() != nullptr) {
				shotgunAug1 = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160),player.shotgun.getPrimaryAugment(), &removeAugment,0, 0 ,&moveAugment, player.shotgun.getPrimaryAugment()->id,0), bt2, -1,-1,-1);
			} else {
				shotgunAug1 = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160), &removeAugment,0), bt2, -1,-1,-1);
			}
			if(player.shotgun.getSecondaryAugment() != nullptr) {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), player.shotgun.getSecondaryAugment(), &removeAugment,1, 0 ,&moveAugment, player.shotgun.getSecondaryAugment()->id,1), shotgunAug1, -1,-1,-1);
			} else {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), &removeAugment,1), shotgunAug1, -1,-1,-1);
			}
		} break;
		case rifle: {
			int rifleAug1;
			if(player.rifle.getPrimaryAugment() != nullptr) {
				rifleAug1 = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160),player.rifle.getPrimaryAugment(), &removeAugment,0, 0 ,&moveAugment, player.rifle.getPrimaryAugment()->id,0), bt3, -1,-1,-1);
			} else {
				rifleAug1 = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160), &removeAugment,0), bt3, -1,-1,-1);
			}
			if(player.rifle.getSecondaryAugment() != nullptr) {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), player.rifle.getSecondaryAugment(), &removeAugment,1, 0 ,&moveAugment, player.rifle.getSecondaryAugment()->id,1), rifleAug1, -1,-1,-1);
			} else {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), &removeAugment,1), rifleAug1, -1,-1,-1);
			}
		} break;
		case laserPistol: {
			int laserPistol;
			if(player.laserPistol.getPrimaryAugment() != nullptr) {
				laserPistol = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160),player.laserPistol.getPrimaryAugment(), &removeAugment,0, 0 ,&moveAugment, player.laserPistol.getPrimaryAugment()->id,0), bt4, -1,-1,-1);
			} else {
				laserPistol = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160), &removeAugment,0), bt4, -1,-1,-1);
			}
			if(player.laserPistol.getSecondaryAugment() != nullptr) {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160+64+40), player.laserPistol.getSecondaryAugment(), &removeAugment,1, 0 ,&moveAugment, player.laserPistol.getSecondaryAugment()->id,1), laserPistol, -1,-1,-1);
			} else {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160+64+40), &removeAugment,1), laserPistol, -1,-1,-1);
			}
		} break;
		default: {
			int revolver;
			if(player.revolver.getPrimaryAugment() != nullptr) {
				revolver = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160),player.revolver.getPrimaryAugment(), &removeAugment,0, 0 ,&moveAugment, player.revolver.getPrimaryAugment()->id,0), bt1, -1,-1,-1);
			} else {
				revolver = inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160), &removeAugment,0), bt1, -1,-1,-1);
			}
			if(player.revolver.getSecondaryAugment() != nullptr) {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40), player.revolver.getSecondaryAugment(), &removeAugment,1, 0 ,&moveAugment, player.revolver.getSecondaryAugment()->id,1), revolver, -1,-1,-1);
			} else {
				inventoryMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40), &removeAugment,1), revolver, -1,-1,-1);
			}
		} break;
	}
}

void RunMenu::initNewAugmentMenu() {
	if(ggs.moveAugment) {
		newAugmentMenu.addTitle((WINDOW_WIDTH-newAugmentMenuTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2, scaleUI(40), moveAugmentMenuTitle);
	} else {
		newAugmentMenu.addTitle((WINDOW_WIDTH-newAugmentMenuTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2, scaleUI(40), newAugmentMenuTitle);
	}

	newAugmentMenu.reset();
	newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,(WINDOW_WIDTH-newAugmentMenuTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2 + newAugmentMenuTitle.getWidth() + scaleUI(30) ,scaleUI(30), ggs.newAugment, &noActionAug,1), -1, -1,-1,-1);

	bt1 = newAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing,height,"ui/upgrade-revolver.png", &selectWeapon, Weapon_Type::revolver),-1,-1,-1,-1);
	bt2 = newAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*2 + scaleUI(120),height,"ui/upgrade-shotgun.png", &selectWeapon, Weapon_Type::shotgun),-1,-1,bt1,-1);
	bt3 = newAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*3 + scaleUI(120*2),height,"ui/upgrade-rifle.png", &selectWeapon, Weapon_Type::rifle),-1,-1,bt2,-1);
	bt4 = newAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*4 + scaleUI(120*3),height,"ui/upgrade-laser-pistol.png", &selectWeapon, Weapon_Type::laserPistol),-1,-1,bt3,-1);

}

void RunMenu::loadWeaponAugmentsNew(Weapon_Type weapon) {
	switch (weapon) {
		case shotgun: {
			int shotgunAug1;
			if(player.shotgun.getPrimaryAugment() != nullptr) {
				shotgunAug1 = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160),player.shotgun.getPrimaryAugment(), &addAugment,0), bt2, -1,-1,-1);
			} else {
				shotgunAug1 = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160), &addAugment,0), bt2, -1,-1,-1);
			}
			if(player.shotgun.getSecondaryAugment() != nullptr) {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), player.shotgun.getSecondaryAugment(), &addAugment,1), shotgunAug1, -1,-1,-1);
			} else {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), &addAugment,1), shotgunAug1, -1,-1,-1);
			}
		} break;
		case rifle: {
			int rifleAug1;
			if(player.rifle.getPrimaryAugment() != nullptr) {
				rifleAug1 = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160),player.rifle.getPrimaryAugment(), &addAugment,0), bt3, -1,-1,-1);
			} else {
				rifleAug1 = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160), &addAugment,0), bt3, -1,-1,-1);
			}
			if(player.rifle.getSecondaryAugment() != nullptr) {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), player.rifle.getSecondaryAugment(), &addAugment,1), rifleAug1, -1,-1,-1);
			} else {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), &addAugment,1), rifleAug1, -1,-1,-1);
			}
		} break;
		case laserPistol: {
			int laserPistol;
			if(player.laserPistol.getPrimaryAugment() != nullptr) {
				laserPistol = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160),player.laserPistol.getPrimaryAugment(), &addAugment,0), bt4, -1,-1,-1);
			} else {
				laserPistol = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160), &addAugment,0), bt4, -1,-1,-1);
			}
			if(player.laserPistol.getSecondaryAugment() != nullptr) {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160+64+40), player.laserPistol.getSecondaryAugment(), &addAugment,1), laserPistol, -1,-1,-1);
			} else {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160+64+40), &addAugment,1), laserPistol, -1,-1,-1);
			}
		} break;
		default: {
			int revolver;
			if(player.revolver.getPrimaryAugment() != nullptr) {
				revolver = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160),player.revolver.getPrimaryAugment(), &addAugment,0), bt1, -1,-1,-1);
			} else {
				revolver = newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160), &addAugment,0), bt1, -1,-1,-1);
			}
			if(player.revolver.getSecondaryAugment() != nullptr) {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40), player.revolver.getSecondaryAugment(), &addAugment,1), revolver, -1,-1,-1);
			} else {
				newAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40), &addAugment,1), revolver, -1,-1,-1);
			}
		} break;
	}
}

void RunMenu::initMoveAugmentMenu() {

	moveAugmentMenu.reset();
	moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,(WINDOW_WIDTH-moveAugmentMenuTitle.getWidth()-AugButton::getStaticWidth()-scaleUI(30))/2 + moveAugmentMenuTitle.getWidth() + scaleUI(30) ,scaleUI(30), ggs.augmentToMove, &noActionAug,1), -1, -1,-1,-1);

	bt1 = moveAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing,height,"ui/upgrade-revolver.png", &selectWeapon, Weapon_Type::revolver),-1,-1,-1,-1);
	bt2 = moveAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*2 + scaleUI(120),height,"ui/upgrade-shotgun.png", &selectWeapon, Weapon_Type::shotgun),-1,-1,bt1,-1);
	bt3 = moveAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*3 + scaleUI(120*2),height,"ui/upgrade-rifle.png", &selectWeapon, Weapon_Type::rifle),-1,-1,bt2,-1);
	bt4 = moveAugmentMenu.addButton(std::make_unique<InventoryButton>(ggs,spacing*4 + scaleUI(120*3),height,"ui/upgrade-laser-pistol.png", &selectWeapon, Weapon_Type::laserPistol),-1,-1,bt3,-1);

}

void RunMenu::loadWeaponAugmentsMove(Weapon_Type weapon) {
	switch (weapon) {
		case shotgun: {
			int shotgunAug1;
			if(player.shotgun.getPrimaryAugment() != nullptr) {
				shotgunAug1 = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160),player.shotgun.getPrimaryAugment(), &addAugment,0), bt2, -1,-1,-1);
			} else {
				shotgunAug1 = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160), &addAugment,0), bt2, -1,-1,-1);
			}
			if(player.shotgun.getSecondaryAugment() != nullptr) {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), player.shotgun.getSecondaryAugment(), &addAugment,1), shotgunAug1, -1,-1,-1);
			} else {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*2+scaleUI(120+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), &addAugment,1), shotgunAug1, -1,-1,-1);
			}
		} break;
		case rifle: {
			int rifleAug1;
			if(player.rifle.getPrimaryAugment() != nullptr) {
				rifleAug1 = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160),player.rifle.getPrimaryAugment(), &addAugment,0), bt3, -1,-1,-1);
			} else {
				rifleAug1 = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160), &addAugment,0), bt3, -1,-1,-1);
			}
			if(player.rifle.getSecondaryAugment() != nullptr) {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), player.rifle.getSecondaryAugment(), &addAugment,1), rifleAug1, -1,-1,-1);
			} else {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*3+scaleUI(120*2+60)-AugButton::getStaticWidth()/2,height+scaleUI(160+64+40), &addAugment,1), rifleAug1, -1,-1,-1);
			}
		} break;
		case laserPistol: {
			int laserPistol;
			if(player.laserPistol.getPrimaryAugment() != nullptr) {
				laserPistol = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160),player.laserPistol.getPrimaryAugment(), &addAugment,0), bt4, -1,-1,-1);
			} else {
				laserPistol = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160), &addAugment,0), bt4, -1,-1,-1);
			}
			if(player.laserPistol.getSecondaryAugment() != nullptr) {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160+64+40), player.laserPistol.getSecondaryAugment(), &addAugment,1), laserPistol, -1,-1,-1);
			} else {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing*4+scaleUI(120*4)-AugButton::getStaticWidth(),height+scaleUI(160+64+40), &addAugment,1), laserPistol, -1,-1,-1);
			}
		} break;
		default: {
			int revolver;
			if(player.revolver.getPrimaryAugment() != nullptr) {
				revolver = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160),player.revolver.getPrimaryAugment(), &addAugment,0), bt1, -1,-1,-1);
			} else {
				revolver = moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160), &addAugment,0), bt1, -1,-1,-1);
			}
			if(player.revolver.getSecondaryAugment() != nullptr) {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40), player.revolver.getSecondaryAugment(), &addAugment,1), revolver, -1,-1,-1);
			} else {
				moveAugmentMenu.addButton(std::make_unique<AugButton>(ggs,spacing,height+scaleUI(160+64+40), &addAugment,1), revolver, -1,-1,-1);
			}
		} break;
	}
}


