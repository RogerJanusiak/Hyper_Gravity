#include <SDL_events.h>

#include "../../includes/HEng/MainMenu.h"

#include <memory>

#include "../../includes/Utils/Input.h"

MainMenu::MainMenu(GlobalGameState& ggs) : ggs(ggs) {
    initMenus();
}

void MainMenu::render() const {
	currentMenu->render();
}

// TODO: Read input from WASD and Arrow Keys for menu input.
void MainMenu::readInput() {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_ESCAPE && currentMenu == &levelSelect) {
                changeMenu(head);
            } else if(e.key.keysym.sym == SDLK_ESCAPE && currentMenu == &pauseMenu) {
                ggs.inPauseMenu = false;
            }
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::select);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                if(currentMenu == &levelSelect) {
                    changeMenu(head);
                } else if(currentMenu == &pauseMenu) {
                    ggs.inPauseMenu = false;
                }
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::up);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::down);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::left);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::right);
            } else if (SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1) {
                if(currentMenu == &pauseMenu) {
                    ggs.inPauseMenu = false;
                }
            }

        } else if(e.type == SDL_JOYDEVICEADDED ) {
            controllerEvent(*currentMenu,MENU_CONTROL::connect);
            ggs.controller = loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            controllerEvent(*currentMenu,MENU_CONTROL::disconnect);
            ggs.controller = nullptr;
        } else  if( e.type == SDL_MOUSEMOTION) {
            mouseMove(ggs, *currentMenu);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            menuSelect(*currentMenu);
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(ggs.controllerStickReset) {
                    controllerEvent(*currentMenu,MENU_CONTROL::down);
                    ggs.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(ggs.controllerStickReset) {
                    controllerEvent(*currentMenu,MENU_CONTROL::up);
                    ggs.controllerStickReset = false;
                }
            } else {
                ggs.controllerStickReset = true;
            }
        }
    }
}

void MainMenu::changeMenu(const Menu menu) {
    switch(menu) {
        case head: {
            currentMenu = &mainMenu;
        } break;
        case level: {
            currentMenu = &levelSelect;
        } break;
        case pause: {
            currentMenu = &pauseMenu;
        } break;
        default:
            break;
    }
    currentMenu->loadMenu();
}

MainMenu* currentMainMenu = nullptr;

void setCurrentMainMenu(MainMenu* mainMenu) {
    currentMainMenu = mainMenu;
}

void showLevelSelect(GlobalGameState& ggs, int attr1, int attr2) {
    currentMainMenu->changeMenu(level);
}

void selectLevel(GlobalGameState& ggs, int attr1, int attr2) {
    ggs.level = attr1;
    ggs.inMainMenu = false;
}

void resumeGame(GlobalGameState& ggs, int attr1, int attr2) {
    ggs.inPauseMenu = false;
}

void quitToMenu(GlobalGameState& ggs, int attr1, int attr2) {
    ggs.inPauseMenu = false;
    ggs.inRun = false;
    ggs.toMainMenu = true;
}

void MainMenu::initMenus() {
    const int centeredX = (WINDOW_WIDTH-Button::getStaticWidth())/2;

    mainMenu.setup(ggs.renderer);
    const int arcadeModeButton = mainMenu.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(135), "Arcade Mode", &showLevelSelect),-1,-1,-1,-1);
    const int settingsButton = mainMenu.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(185), "Settings", &noAction),arcadeModeButton,-1,-1,-1);
    mainMenu.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(235), "Quit To Desktop", &quitToDesktop),settingsButton,-1,-1,-1);

    logoTexture.setup(scaleUI(454),scaleUI(92),ggs.renderer);
    logoTexture.loadFromFile("logo.png");
    mainMenu.addTitle((WINDOW_WIDTH-logoTexture.getWidth())/2,scaleUI(35), logoTexture);

    levelSelect.setup(ggs.renderer);
    const int level1Button = levelSelect.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(135), "Warehouse", &selectLevel,3),-1,-1,-1,-1);
    const int level2Button = levelSelect.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(185), "Lobby", &selectLevel,4),level1Button,-1,-1,-1);
    const int level3Button = levelSelect.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(235), "The Ducts", &selectLevel,1),level2Button,-1,-1,-1);
    levelSelect.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(285), "Air Port", &selectLevel,2),level3Button,-1,-1,-1);
    levelSelect.addTitle((WINDOW_WIDTH-logoTexture.getWidth())/2,scaleUI(35), logoTexture);

    pauseMenu.setup(ggs.renderer);

    const int resumeButton = pauseMenu.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(135), "Resume Game", &resumeGame),-1,-1,-1,-1);
    const int quitToMenuButton = pauseMenu.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(185), "Quit To Menu", &quitToMenu),resumeButton,-1,-1,-1);
    pauseMenu.addButton(std::make_unique<Button>(ggs, centeredX,scaleUI(235), "Quit To Desktop", &quitToDesktop),quitToMenuButton,-1,-1,-1);
    pauseMenu.addTitle((WINDOW_WIDTH-logoTexture.getWidth())/2,scaleUI(35), logoTexture);

}
