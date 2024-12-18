#include <chrono>

#include "../includes/Animations.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <list>
#include <memory>
#include <sstream>
#include <vector>

#include "../includes/Bullet.h"
#include "../includes/Robor.h"
#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"
#include "../includes/Player.h"
#include "../includes/Spawn.h"
#include "../includes/State.h"
#include "../includes/Robro.h"
#include "../includes/Romo.h"
#include "../includes/Roo.h"

#include <SDL_ttf.h>
#include <thread>

#include "../includes/Roborto.h"
#include "../includes/Sound.h"
#include "../includes/UI.h"

#include <future>

SDL_Window *gameWindow = nullptr;
SDL_Renderer *gameRenderer = nullptr;

std::string gameFilesPath;

//Game Controller 1 handler
SDL_GameController* controller = nullptr;

bool init();
void close();
void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::list<Entity*>& allCharacterEntities);
void renderPlatforms(std::list<Platform*>& platforms);

void loadLevelFromCSV(std::string& filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns, std::vector<SDL_Rect>& teleports);
void loadController();
void resetState();

void getWaveEnemyEntities(int waveNumber, std::vector<Spawn>* spawns, std::vector<std::unique_ptr<Entity>>& entities, std::vector<std::unique_ptr<Enemy>>& enemies);

void checkIfSpawnsAreOnScreen(std::vector<Spawn>& enemySpawns);

void moveCamera(std::list<Entity*>& allCharacterEntities, std::list<Platform*>& platforms, std::vector<Spawn*>& allSpawns, float dt);

bool pauseEnemy = false;

Player* timpyPointer = nullptr;

SDL_Rect fullScreenRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

State state;

void startScreen() {
    SDL_RenderClear(gameRenderer);

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            state.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_ESCAPE && state.menu == level) {
                state.menu = head;
            }
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                controllerEvent(state,MENU_CONTROL::select);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                controllerEvent(state,MENU_CONTROL::back);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
                controllerEvent(state,MENU_CONTROL::up);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
                controllerEvent(state,MENU_CONTROL::down);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
                controllerEvent(state,MENU_CONTROL::left);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
                controllerEvent(state,MENU_CONTROL::right);
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            controllerEvent(state,MENU_CONTROL::connect);
            loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            controllerEvent(state,MENU_CONTROL::disconnect);
            controller = nullptr;
            state.controller = false;
        } else  if( e.type == SDL_MOUSEMOTION) {
            mouseMove(state);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            menuSelect(state);
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::down);
                    state.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::up);
                    state.controllerStickReset = false;
                }
            } else {
                state.controllerStickReset = true;
            }
        }
    }

    renderMenu(state);
    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
    SDL_RenderPresent(gameRenderer);
}

void pauseScreen() {
    SDL_RenderClear(gameRenderer);

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            state.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                state.menu = notInMenu;
            }
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                controllerEvent(state,MENU_CONTROL::select);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                state.menu = notInMenu;
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
                controllerEvent(state,MENU_CONTROL::up);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
                controllerEvent(state,MENU_CONTROL::down);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
                controllerEvent(state,MENU_CONTROL::left);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
                controllerEvent(state,MENU_CONTROL::right);
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            controllerEvent(state,MENU_CONTROL::connect);
            loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            controllerEvent(state,MENU_CONTROL::disconnect);
            controller = nullptr;
            state.controller = false;
        } else  if( e.type == SDL_MOUSEMOTION) {
            mouseMove(state);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            menuSelect(state);
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::down);
                    state.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::up);
                    state.controllerStickReset = false;
                }
            } else {
                state.controllerStickReset = true;
            }
        }
    }

    renderMenu(state);
    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
    SDL_RenderPresent(gameRenderer);
}

int main( int argc, char* args[] ) {
    if(!init()) {
        SDL_Log("Initialization failed!\n");
    } else {

        SDL_Event e;
        Uint32 lastUpdate = SDL_GetTicks();

        Sound explosion("resources/sounds/shortExplosion.wav", 0,-1);
        Sound mediumExplosion("resources/sounds/mediumExplosion.wav", 0,-1);
        Sound song("resources/sounds/song.wav", -1,0);

        std::list<Explosion> explosions;

        if(controller != nullptr) {
            controllerEvent(state,MENU_CONTROL::connect);
        }

        SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);

        Texture background;
        background.setup(WINDOW_WIDTH,WINDOW_HEIGHT,gameRenderer);
        background.loadFromFile("background.png");

        SDL_Rect backgroundRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

        std::vector<Spawn> playerSpawns;
        playerSpawns.reserve(2);

        Weapon revolver(Weapon_Type::revolver,gameRenderer, state);
        Weapon rifle(Weapon_Type::rifle,gameRenderer, state);
        Weapon shotgun(Weapon_Type::shotgun,gameRenderer, state);
        Weapon knife(Weapon_Type::knife,gameRenderer, state);
        Weapon laserPistol(Weapon_Type::laserPistol,gameRenderer, state);

        Entity eTimpy = Entity(&playerSpawns,gameRenderer,10);
        Player timpy = Player(&eTimpy,&revolver);
        Texture teleportCursor = Texture();
        teleportCursor.setup(scale(50),scale(60),gameRenderer);
        teleportCursor.loadFromFile("Timpy.png");
        timpyPointer = &timpy;

        UI_init(gameRenderer, state, &timpy);

        //Game Loop
        while(!state.quit) {
            resetState();
            playerSpawns.clear();

            std::list<Platform> ePlatforms;
            std::vector<Spawn> enemySpawns;
            std::vector<SDL_Rect> teleports;
            teleports.reserve(10);
            enemySpawns.reserve(15);

            std::vector<Spawn*> allSpawns;
            allSpawns.reserve(15);

            std::list<Platform*> platforms;

            std::list<Entity> eBullets;
            std::list<Bullet> bullets;

            if(controller != nullptr) {
                controllerEvent(state,MENU_CONTROL::connect);
            }

            while(!state.started && !state.quit) {
                startScreen();
            }

            state.camY = 0;
            state.camV = 0;

            bool waveOverride = false;

            bool shootingReset = true;

            bool inWave;
            int waveNumber = 0;

            float lastFPS = 0;

            char* appDir = SDL_GetBasePath();
            std::string currentPath(appDir);
            SDL_free(appDir);
            std::string levelPath;

            if(state.level == 1) {
                levelPath = currentPath + "resources/levels/airduct.csv";
                state.currentWeight = &state.ductWeight;
            } else if(state.level == 2) {
                levelPath = currentPath + "resources/levels/airport.csv";
                state.currentWeight = &state.airportWeight;
            } else if(state.level == 3) {
                levelPath = currentPath + "resources/levels/lab.csv";
                state.currentWeight = &state.standardWeight;
            } else if (state.level == 4) {
                levelPath = currentPath + "resources/levels/lobby.csv";
                state.currentWeight = &state.standardWeight;
            }

            loadLevelFromCSV((levelPath), ePlatforms, enemySpawns, playerSpawns, teleports);

            for(auto it = enemySpawns.begin(); it != enemySpawns.end(); ++it) {
                allSpawns.push_back(&*it);
            }
            for(auto it = playerSpawns.begin(); it != playerSpawns.end(); ++it) {
                allSpawns.push_back(&*it);
            }

            for(auto it = ePlatforms.begin(); it != ePlatforms.end(); ++it) {
                platforms.push_back(&*it);
            }

            timpy.getEntity()->forceSpawn();

            std::vector<std::vector<std::unique_ptr<Entity>>> waveSetEEnemies;
            waveSetEEnemies.reserve(5);
            std::vector<std::vector<std::unique_ptr<Enemy>>> waveSetEnemies;
            waveSetEnemies.reserve(5);

            while(state.started && !state.quit) {

                bool leftMovement = false;
                bool rightMovement = false;

                if(waveNumber == 0) {
                    resetState();
                    timpy.setSecondaryWeapon(nullptr);
                    timpy.setAbility(none);
                }

                inWave = true;
                waveNumber++;

                int waveSetLocation = (waveNumber-1) % 5;
                if(waveSetLocation == 0) {
                    waveSetEEnemies.clear();
                    waveSetEnemies.clear();
                    for(auto& enemy : state.setEnemies) {
                        enemy = 0;
                    }
                    for(int i = 0; i < 5; i++) {
                        std::vector<std::unique_ptr<Entity>> eEnemies;
                        std::vector<std::unique_ptr<Enemy>> enemies;

                        getWaveEnemyEntities(waveNumber + i,&enemySpawns, eEnemies, enemies);

                        waveSetEEnemies.push_back(std::move(eEnemies));
                        waveSetEnemies.push_back(std::move(enemies));
                    }
                    for(auto& set : waveSetEnemies) {
                        for(auto& enemies : set) {
                            state.setEnemies[enemies->getType()]++;
                        }
                    }
                }

                std::list<Entity*> allCharacterEntities;
                for (auto& eEnemy: waveSetEEnemies[waveSetLocation]) {
                    allCharacterEntities.push_back(eEnemy.get());
                }

                allCharacterEntities.push_back(timpy.getEntity());

                checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                state.menu = weaponUpgrade;

                launchUpgradeMenu(state);
                loadUpgradeMenu(state);
                while((waveNumber-1) % 5 == 0 && (state.menu == weaponUpgrade || state.menu == abilityUpgrade || state.menu == playerUpgrade) && !state.quit) {
                    while(SDL_PollEvent(&e) != 0) {
                        if( e.type == SDL_QUIT ) {
                            state.quit = true;
                        } else if(e.type == SDL_KEYDOWN) {
                            if(e.key.keysym.sym == SDLK_ESCAPE) {
                                closeUpgradeMenu(state,0,0);
                            }
                        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
                            menuSelect(state);
                        } else if(e.type == SDL_MOUSEMOTION) {
                            mouseMove(state);
                        } else if( e.type == SDL_JOYAXISMOTION) {
                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                                if(state.controllerStickReset) {
                                    controllerEvent(state,MENU_CONTROL::down);
                                    state.controllerStickReset = false;
                                }
                            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                                if(state.controllerStickReset) {
                                    controllerEvent(state,MENU_CONTROL::up);
                                    state.controllerStickReset = false;
                                }
                            } else if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) {
                                if(state.controllerStickReset) {
                                    controllerEvent(state,MENU_CONTROL::right);
                                    state.controllerStickReset = false;
                                }
                            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) {
                                if(state.controllerStickReset) {
                                    controllerEvent(state,MENU_CONTROL::left);
                                    state.controllerStickReset = false;
                                }
                            } else {
                                state.controllerStickReset = true;
                            }
                        } else if( e.type == SDL_JOYBUTTONDOWN ) {
                            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                                controllerEvent(state,MENU_CONTROL::select);
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                                closeUpgradeMenu(state,0,0);
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1) {
                                if(state.menu == weaponUpgrade) {
                                    showAbilityUpgradeMenu(state,0,0);
                                } else if(state.menu == abilityUpgrade) {
                                    showPlayerUpgradeMenu(state,0,0);
                                }
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1) {
                                if(state.menu == abilityUpgrade) {
                                    showWeaponUpgradeMenu(state,0,0);
                                } else if(state.menu == playerUpgrade) {
                                    showAbilityUpgradeMenu(state,0,0);
                                }
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
                                controllerEvent(state,MENU_CONTROL::up);
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
                                controllerEvent(state,MENU_CONTROL::down);
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
                                controllerEvent(state,MENU_CONTROL::left);
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
                                controllerEvent(state,MENU_CONTROL::right);
                            }
                        }
                    }

                    SDL_RenderClear(gameRenderer);

                    //background.render(0,0);
                    SDL_SetRenderDrawColor(gameRenderer,35,35,35,155);
                    SDL_RenderFillRect(gameRenderer,&backgroundRect);

                    for(auto& spawn : allSpawns) {
                        spawn->render(gameRenderer);
                    }

                    for(auto& teleport : teleports) {
                        SDL_SetRenderDrawColor(gameRenderer, 106, 13, 173, 200);
                        SDL_RenderFillRect(gameRenderer, &teleport);
                    }

                    for (auto it = bullets.begin(); it != bullets.end();++it) {
                        it->render();
                    }

                    for (auto it =  waveSetEnemies[waveSetLocation].begin(); it !=  waveSetEnemies[waveSetLocation].end();++it) {
                        (*it)->render();
                    }

                    renderPlatforms(platforms);

                    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 200);
                    SDL_RenderFillRect(gameRenderer, &fullScreenRect);

                    renderMenu(state);

                    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
                    SDL_RenderPresent(gameRenderer);

                }
                timpy.setXNormalV(0);
                switch(state.weapon1) {
                    case 0:
                        timpy.setPrimaryWeapon(&revolver);
                        break;
                    case 1:
                        timpy.setPrimaryWeapon(&rifle);
                        break;
                    case 2:
                        timpy.setPrimaryWeapon(&shotgun);
                        break;
                    case 3:
                        timpy.setPrimaryWeapon(&knife);
                        break;
                    case 4:
                        timpy.setPrimaryWeapon(&laserPistol);
                        break;
                    default:
                        timpy.setPrimaryWeapon(nullptr);
                }
                switch(state.weapon2) {
                    case 0:
                        timpy.setSecondaryWeapon(&revolver);
                        break;
                    case 1:
                        timpy.setSecondaryWeapon(&rifle);
                        break;
                    case 2:
                        timpy.setSecondaryWeapon(&shotgun);
                        break;
                    case 3:
                        timpy.setSecondaryWeapon(&knife);
                        break;
                    case 4:
                        timpy.setSecondaryWeapon(&laserPistol);
                        break;
                    default:
                        timpy.setSecondaryWeapon(nullptr);
                }
                if(Weapon* weapon = timpy.getPrimaryWeapon(); weapon != nullptr) {
                    weapon->upgrade(state);
                    weapon->reset();
                }
                if(Weapon* weapon = timpy.getSecondaryWeapon(); weapon != nullptr) {
                    weapon->upgrade(state);
                    weapon->reset();
                }

                state.menu = notInMenu;

                bool waveStarted = false;
                Uint32 startWaveLoad = SDL_GetTicks();

                updateInGameText(timpy.getCombo(),waveNumber, timpy.getXP(),timpy.getHealth(), timpy.getShield());

                while(state.started && inWave && !state.quit) {

                    Uint64 start = SDL_GetPerformanceCounter();

                    Uint32 current = SDL_GetTicks();
                    float dt = (current - lastUpdate) / 1000.0f;
                    lastUpdate = current;

                    if((current-startWaveLoad)/1000.0f > 1) {
                        waveStarted = true;
                    }

                    //Controls Loop
                    while(SDL_PollEvent(&e) != 0 && state.menu != weaponUpgrade) {
                        if( e.type == SDL_QUIT ) {
                            state.quit = true;
                        } else if( e.type == SDL_KEYDOWN ) {
                            if(e.key.keysym.sym == SDLK_1) {
                                state.developerMode = !state.developerMode;
                            }
                            if(e.key.keysym.sym == SDLK_2 && state.developerMode) {
                                waveNumber = 30;
                                waveOverride = true;
                            }
                            if(e.key.keysym.sym == SDLK_3 && state.developerMode) {

                            }
                            if(e.key.keysym.sym == SDLK_4 && state.developerMode) {
                                pauseEnemy = !pauseEnemy;
                            } else if(e.key.keysym.sym == SDLK_ESCAPE) {
                                state.menu = pause;
                            }
                            if(e.key.keysym.sym == SDLK_d) {
                                timpy.setXNormalV(1);
                                rightMovement = true;
                            }
                            if(e.key.keysym.sym == SDLK_a) {
                                timpy.setXNormalV(-1);
                                leftMovement = true;
                            }
                            if(e.key.keysym.sym == SDLK_l) {
                                if(state.teleportSelection) {
                                    state.tcVx = 1;
                                    state.tcMovingRight = true;
                                } else {
                                    timpy.setDirection(true);
                                }

                            } else if(e.key.keysym.sym == SDLK_j) {
                                if(state.teleportSelection) {
                                    state.tcVx = -1;
                                    state.tcMovingLeft = true;
                                } else {
                                    timpy.setDirection(false);
                                }
                            } else if(e.key.keysym.sym == SDLK_k) {
                                if(state.teleportSelection) {
                                    state.tcVy = 1;
                                    state.tcMovingDown = true;
                                }
                            } else if(e.key.keysym.sym == SDLK_i) {
                                if(state.teleportSelection) {
                                    state.tcVy = -1;
                                    state.tcMovingUp = true;
                                }
                            } else if(e.key.keysym.sym == SDLK_w) {
                                timpy.changeWeapon();
                            } else if(e.key.keysym.sym == SDLK_r) {
                                timpy.getWeapon()->forceReload();
                            }
                            if(e.key.keysym.sym == SDLK_SPACE && waveStarted && !state.c4Placed) {
                                if((shootingReset || state.resetShooting) && !state.teleportSelection) {
                                    timpy.getWeapon()->shoot(&eBullets,&bullets,state,timpy.getDirection(),timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y);
                                    shootingReset = false;
                                    state.resetShooting = false;
                                }
                            } else if(e.key.keysym.sym == SDLK_e) {
                                timpy.useAbility(state);
                            }
                        } else if(e.type == SDL_KEYUP) {
                            if(e.key.keysym.sym == SDLK_d)
                                rightMovement = false;
                            if(e.key.keysym.sym == SDLK_a)
                                leftMovement = false;
                            if(!leftMovement && !rightMovement) {
                                timpy.setXNormalV(0);
                            } else if (leftMovement) {
                                timpy.setXNormalV(-1);
                            } else {
                                timpy.setXNormalV(1);
                            }

                            if(e.key.keysym.sym == SDLK_l) {
                                state.tcMovingRight = false;
                            } else if(e.key.keysym.sym == SDLK_j) {
                                state.tcMovingLeft = false;
                            } else if(e.key.keysym.sym == SDLK_k) {
                                state.tcMovingDown = false;
                            } else if(e.key.keysym.sym == SDLK_i) {
                                state.tcMovingUp = false;
                            }

                            if(!state.tcMovingLeft && !state.tcMovingRight) {
                                state.tcVx = 0;
                            } else if (state.tcMovingLeft) {
                                state.tcVx = -1;
                            } else {
                                state.tcVx = 1;
                            }

                            if(!state.tcMovingUp && !state.tcMovingDown) {
                                state.tcVy = 0;
                            } else if (state.tcMovingUp) {
                                state.tcVy = -1;
                            } else {
                                state.tcVy = 1;
                            }

                            if(e.key.keysym.sym == SDLK_SPACE) {
                                shootingReset = true;
                            }
                        } else if( e.type == SDL_JOYAXISMOTION) {
                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > JOYSTICK_DEAD_ZONE) {
                                if(shootingReset && !state.teleportSelection && !state.c4Placed) {
                                    if(timpy.getWeapon()->shoot(&eBullets,&bullets,state,timpy.getDirection(),timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y)) {
                                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 150 );
                                    } else {
                                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 75 );
                                    }
                                    shootingReset = false;
                                }
                            } else {
                                shootingReset = true;
                            }

                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) > JOYSTICK_DEAD_ZONE) {
                                if(state.teleportSelection) {
                                    state.tcVx = 1;
                                } else {
                                    timpy.setDirection(true);
                                }
                            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) < -JOYSTICK_DEAD_ZONE) {
                                if(state.teleportSelection) {
                                    state.tcVx = -1;
                                } else {
                                    timpy.setDirection(false);
                                }
                            } else {
                                state.tcVx = 0;
                            }

                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY) > JOYSTICK_DEAD_ZONE) {
                                if(state.teleportSelection) {
                                    state.tcVy = 1;
                                }
                            } else if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY) < -JOYSTICK_DEAD_ZONE) {
                                if(state.teleportSelection) {
                                    state.tcVy = -1;
                                }
                            } else {
                                state.tcVy = 0;
                            }

                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) {
                                timpy.setXNormalV(1);
                            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) {
                                timpy.setXNormalV(-1);
                            } else {
                                timpy.setXNormalV(0);
                            }
                        } else if(e.type == SDL_JOYDEVICEADDED ) {
                            loadController();
                        } else if (e.type == SDL_JOYDEVICEREMOVED) {
                            controller = nullptr;
                        } else if( e.type == SDL_JOYBUTTONDOWN ) {
                            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y) == 1) {
                                timpy.changeWeapon();
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                                timpy.useAbility(state);
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1) {
                                state.menu = pause;
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) == 1) {
                                timpy.getWeapon()->forceReload();
                            }
                        }
                    }

                    if(state.teleportSelection) {
                        pauseEnemy = true;
                        state.teleportCursorX += state.tcVx*dt*scale(300);
                        state.teleportCursorY += state.tcVy*dt*scale(300);
                        if((current-state.startSelection)/2000.0f > 1) {
                            state.teleportSelection = false;
                            timpy.getEntity()->setPosition(state.teleportCursorX, state.teleportCursorY);
                            pauseEnemy = false;
                        }
                    }

                    if(timpy.getWeapon()->isReloadable()) {
                        updateTimeToShoot(scalePlayerUI(timpy.getWeapon()->reload(dt)));
                    } else {
                        updateTimeToShoot(scalePlayerUI(75));
                    }

                    updateTimeToAbility(scalePlayerUI(timpy.charge( state)));
                    if(timpy.getWeapon()->wasJustReloaded()) {
                        SDL_GameControllerRumble( controller, 0xFFFF * 1 / 2, 0xFFFF * 1 / 2, 50 );
                    }

                    SDL_RenderClear(gameRenderer);

                    //background.render(0,0);
                    SDL_SetRenderDrawColor(gameRenderer,55,55,55,105);
                    SDL_RenderFillRect(gameRenderer,&backgroundRect);

                    if(state.developerMode) {
                        for(auto spawn : allSpawns) {
                            if(!spawn->getOccupied()) {
                                SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                            } else {
                                SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                            }
                            SDL_RenderDrawRect(gameRenderer, &spawn->getRect());
                        }
                    } else {
                        for(auto spawn : allSpawns) {
                            spawn->render(gameRenderer);
                        }
                    }

                    for(auto& teleport : teleports) {
                        SDL_SetRenderDrawColor(gameRenderer, 106, 13, 173, 200);
                        SDL_RenderFillRect(gameRenderer, &teleport);
                    }

                    bool robotAlive = false;
                    bool playerAlive = true;

                    //Render/Move Bullets
                    for (auto it = bullets.begin(); it != bullets.end();) {
                        if(it->move(dt, platforms, state.developerMode)) {
                            eBullets.erase(it->getIterator());
                            it = bullets.erase(it);
                        } else {
                            it->render();
                            ++it;
                        }
                    }

                    //Render/Move/Collision Enemies
                    bool playerDamaged = false;
                    SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                    int enemiesAlive = 0;
                    for (auto it =  waveSetEnemies[waveSetLocation].begin(); it !=  waveSetEnemies[waveSetLocation].end();++it) {
                        bool firstLoop = false;
                        bool abilityDamgage = false;
                        if(!(*it)->getEntity()->isSpawned()) {
                            (*it)->getEntity()->spawn(state.enemiesAlive <= 5);
                            firstLoop = true;
                        }
                        if((*it)->getEntity()->isAlive() && (*it)->getEntity()->isSpawned()) {
                            if(!firstLoop && waveStarted && !pauseEnemy && state.menu != weaponUpgrade) {
                                (*it)->move(dt, platforms,state);
                            }
                            (*it)->render();
                            if(!(*it)->didAlreadyCollide() && timpy.getWeapon()->getType() == Weapon_Type::knife && Entity::isColliding((*it)->getEntity()->getRect(),timpy.getWeaponRect())) {
                                (*it)->getEntity()->damage(timpy.getWeapon()->getDamage());
                                (*it)->knifeColliding();
                            } else {
                                if(timpy.getWeapon()->getType() == Weapon_Type::knife && !Entity::isColliding((*it)->getEntity()->getRect(),timpy.getWeaponRect())) {
                                    (*it)->knifeNotColliding();
                                }
                                if( Entity::isColliding((*it)->getEntity()->getRect(),timpy.getHitRect())) {
                                    if(timpy.getEntity()->getRect().y + (timpy.getEntity()->getRect().h-(*it)->getEntity()->getRect().h) < (*it)->getEntity()->getRect().y
                                        && timpy.getAbility() == Ability::bounce && timpy.isCharged()) {
                                            timpy.getEntity()->setYVelocity(-1800);
                                            (*it)->getEntity()->damage(5);
                                            timpy.setInvincible(true);
                                            abilityDamgage = true;
                                        } else if(!timpy.isInvincible()) {
                                            int randomNumber = rand() % 100;
                                            int dodgeChance = state.playerLevels[dodge] == 0 ? 0 : state.playerProperties[dodge][state.playerLevels[dodge]-1][1];
                                            if(randomNumber >= dodgeChance) {
                                                if(timpy.damage(state)) {
                                                    playerAlive = false;
                                                    waveNumber = 0;
                                                }
                                                SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                                                updateInGameText(timpy.getCombo(),waveNumber,timpy.getXP(),timpy.getHealth(), timpy.getShield());
                                                playerDamaged = true;
                                            }
                                            (*it)->getEntity()->damage(5);
                                        }

                                }
                            }
                            for(auto bit = bullets.begin(); bit != bullets.end();) {
                                if(state.developerMode) {
                                    SDL_SetRenderDrawColor(gameRenderer,0,0,255,255);
                                    SDL_Rect temp = bit->getTrailingRect();
                                    SDL_RenderFillRect(gameRenderer, &temp);
                                }
                                if(Entity::isColliding((*it)->getEntity()->getRect(),bit->getTrailingRect())) {
                                    explosion.play();
                                    if(bit->decreaseStrength()) {
                                        eBullets.erase(bit->getIterator());
                                        bit = bullets.erase(bit);
                                    } else {
                                        ++bit;
                                    }
                                    (*it)->getEntity()->damage(timpy.getWeapon()->getDamage());
                                    break;
                                }
                                ++bit;
                            }
                            if(state.developerMode) {
                                SDL_RenderDrawRect(gameRenderer,&(*it)->getEntity()->getRect());
                            }
                            if(state.c4Exploded) {
                                int c4x = timpy.getC4Entity()->getRect().x;
                                int c4y = timpy.getC4Entity()->getRect().y;
                                if(pow(pow(c4x - (*it)->getEntity()->getRect().x,2)+pow(c4y - (*it)->getEntity()->getRect().y,2),0.5) < scale(200)) {
                                    (*it)->getEntity()->damage(state.abilityProperties[c4][state.abilityLevels[c4]-1][2]);
                                    abilityDamgage = true;
                                }
                            }
                            if(!(*it)->getEntity()->isAlive()) {
                                explosions.emplace_back((*it)->getEntity()->getRect().x+(*it)->getEntity()->getRect().w/2,(*it)->getEntity()->getRect().y+(*it)->getEntity()->getRect().h/2,gameRenderer);
                                if(!playerDamaged) {
                                    timpy.changeXP((*it)->getDifficulty());
                                    timpy.killEnemy(state);
                                    if(!abilityDamgage) {
                                        if(state.abilitiesKills < state.abilityProperties[timpy.getAbility()][state.abilityLevels[timpy.getAbility()]][1] && timpy.getAbility() != none && !timpy.isCharged())
                                            state.abilitiesKills++;
                                    }
                                }
                                updateInGameText(timpy.getCombo(),waveNumber, timpy.getXP(),timpy.getHealth(), timpy.getShield());
                            } else {
                                for(auto& teleport : teleports) {
                                    if(Entity::isColliding((*it)->getEntity()->getRect(),teleport)) {
                                        (*it)->getEntity()->despawn();
                                    }
                                }

                            }
                        }
                        if((*it)->getEntity()->isAlive()) {
                            robotAlive = true;
                            enemiesAlive++;
                        }
                    }

                    state.enemiesAlive = enemiesAlive;

                    for(auto it = explosions.begin();it != explosions.end();) {
                        if(it->finished()) {
                            it = explosions.erase(it);
                        } else {
                            it->render();
                            it->advance();
                            ++it;
                        }
                    }

                    state.c4Exploded = false;

                    if(!playerAlive || waveOverride) {
                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                        inWave = false;
                        waveOverride = false;
                    } else {
                        inWave = robotAlive;
                    }

                    if(timpy.getEntity()->isSpawned()) {
                        if(waveStarted) {
                            timpy.move(dt, platforms,state);
                        }
                        if(timpy.isInvincible()) {
                            SDL_SetTextureColorMod(timpy.getEntity()->getTexture()->getTexture(),0,150,255);
                            timpy.tickInvicibilty(dt);
                        } else {
                            SDL_SetTextureColorMod(timpy.getEntity()->getTexture()->getTexture(),255,255,255);
                        }
                        timpy.render();
                        state.playerX = timpy.getEntity()->getRect().x;
                        state.playerTileX = state.playerX/TILE_SIZE_SCALED;
                        state.playerTileY = (timpy.getEntity()->getRect().y-state.camY)/TILE_SIZE_SCALED;

                        if(state.developerMode) {
                            SDL_Rect playerTile = {(state.playerTileX)*TILE_SIZE_SCALED, state.playerTileY*TILE_SIZE_SCALED+state.camY,TILE_SIZE_SCALED,TILE_SIZE_SCALED};
                            SDL_SetRenderDrawColor(gameRenderer, 225, 225, 0, 255);
                            SDL_RenderDrawRect(gameRenderer, &playerTile);
                        }

                        for(auto& teleport : teleports) {
                            if(Entity::isColliding(timpy.getEntity()->getRect(),teleport)) {
                                timpy.getEntity()->forceSpawn();
                            }
                        }
                    }

                    if((timpy.getEntity()->getRect().y >= scale(195) && state.camY > -1*(scale(state.levelHeight)-WINDOW_HEIGHT)) || (timpy.getEntity()->getRect().y < 0 && state.camY < 0)) {
                        moveCamera(allCharacterEntities,platforms,allSpawns, dt);
                    }

                    SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                    if(state.developerMode) {
                        SDL_RenderDrawRect(gameRenderer,&timpy.getEntity()->getRect());
                        SDL_RenderDrawRect(gameRenderer, timpy.getWheelRect());
                        SDL_Rect tempHitRect = timpy.getHitRect();
                        SDL_RenderDrawRect(gameRenderer, &tempHitRect);
                    }

                    checkIfSpawnsAreOnScreen(enemySpawns);

                    checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                    renderPlatforms(platforms);

                    renderInGameText(state.developerMode, lastFPS, waveStarted);

                    renderPlayerUI(&timpy);

                    if(state.teleportSelection) {
                        SDL_SetTextureColorMod(teleportCursor.getTexture(),0,150,255);
                        teleportCursor.render(state.teleportCursorX,state.teleportCursorY);
                    }

                    SDL_SetRenderDrawColor(gameRenderer, 16, 16, 16, 255);
                    SDL_RenderPresent(gameRenderer);

                    Uint64 end = SDL_GetPerformanceCounter();

                    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
                    lastFPS = (1.0f / elapsed);

                    if(controller != nullptr && state.menu == pause) {
                        controllerEvent(state,MENU_CONTROL::connect);
                    }

                    while(state.menu == pause && !state.quit && state.started) {
                        pauseScreen();
                        Uint32 current = SDL_GetTicks();
                        lastUpdate = current;
                    }

                }
            }
        }

    }
    close();
    return 0;
}

void resetState() {
    state.c4Placed = false;

    state.upgradeIncreaseFactor = 0;

    for(auto& enemy : state.setEnemies) {
        enemy = 0;
    }

    for(auto& level : state.weaponLevels) {
        level = 0;
    }
    state.weaponLevels[0] = 1;

    for(auto& level : state.abilityLevels) {
        level = 0;
    }

    for(auto& level : state.playerLevels) {
        level = 0;
    }
    timpyPointer->reset(state);
    loadUpgradeMenu(state);
}

void checkIfSpawnsAreOnScreen(std::vector<Spawn>& enemySpawns) {
    for (auto sit = enemySpawns.begin(); sit != enemySpawns.end(); ++sit) {
        if(sit->getRect().x > 0 && sit->getRect().x < WINDOW_WIDTH
            && sit->getRect().y >= 0 && sit->getRect().y+sit->getRect().h <= WINDOW_HEIGHT) {
            sit->setOnScreen(true);
        } else {
            sit->setOnScreen(false);
        }
    }
}

void renderPlatforms(std::list<Platform*>& platforms) {
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 255, 255);
    for (auto it = platforms.begin(); it != platforms.end(); ++it) {
        (*it)->render();
        if(state.developerMode) {
            SDL_RenderDrawRect(gameRenderer,&(*it)->getPlatformRect());
        }
    }
}

void moveCamera(std::list<Entity*>& allCharacterEntities, std::list<Platform*>& platforms, std::vector<Spawn*>& allSpawns, float dt) {
    int vector = (scale(195)-timpyPointer->getEntity()->getRect().y)*scale(2)*dt;

    if(state.camY+vector > 0) {
        vector = state.camY*-1;
    } else if(state.camY+vector < -1*(scale(state.levelHeight)-WINDOW_HEIGHT)) {
        vector = -1*(scale(state.levelHeight)-WINDOW_HEIGHT)-state.camY;
    }

    state.camY += vector;

    timpyPointer->getC4Entity()->setPosition(timpyPointer->getC4Entity()->getRect().x, timpyPointer->getC4Entity()->getRect().y+vector);

    for (auto entites : allCharacterEntities) {
        entites->setPosition(entites->getRect().x,entites->getRect().y+vector);
    }
    timpyPointer->updateWheelRect();
    for (auto platform : platforms) {
        platform->setPosition(platform->getPlatformRect().x,platform->getPlatformRect().y+vector);
    }
    for(auto spawn : allSpawns) {
        spawn->setPosition(spawn->getRect().x,spawn->getRect().y+vector);
    }
}

void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::list<Entity*>& allCharacterEntities) {
    for (auto sit = allSpawns.begin(); sit != allSpawns.end(); ++sit) {
        (*sit)->setOccupied(false);
        for (auto it = allCharacterEntities.begin(); it != allCharacterEntities.end(); ++it) {
            if(Entity::isColliding((*sit)->getRect(),(*it)->getRect())) {
                (*sit)->setOccupied(true);
            }
        }
    }
}

void getWaveEnemyEntities(int waveNumber, std::vector<Spawn>* spawns, std::vector<std::unique_ptr<Entity>>& entities, std::vector<std::unique_ptr<Enemy>>& enemies) {
    int totalDifficulty = 0;
    while(totalDifficulty < waveNumber) {
        int enemyType = rand() % 100 + 1;
        if(enemyType <= (*state.currentWeight)[0] && totalDifficulty+Robor::difficulty <= waveNumber) {
            totalDifficulty += Robor::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, gameRenderer, Robor::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Robor(entities.back().get())));
        } else if(enemyType > (*state.currentWeight)[0] && enemyType <= (*state.currentWeight)[1] && totalDifficulty+Roborto::difficulty <= waveNumber) {
            totalDifficulty += Roborto::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, gameRenderer, Roborto::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Roborto(entities.back().get())));
        } else if(enemyType > (*state.currentWeight)[1] && enemyType <= (*state.currentWeight)[2] && totalDifficulty+Robro::difficulty <= waveNumber) {
            totalDifficulty += Robro::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, gameRenderer, Robro::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Robro(entities.back().get())));
        } else if(enemyType > (*state.currentWeight)[2] && enemyType <= (*state.currentWeight)[3] && totalDifficulty+Romo::difficulty <= waveNumber) {
            totalDifficulty += Romo::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, gameRenderer, Romo::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Romo(entities.back().get())));
        } else if(enemyType > (*state.currentWeight)[3] && enemyType <= (*state.currentWeight)[4] && totalDifficulty+Roo::difficulty <= waveNumber) {
            totalDifficulty += Roo::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, gameRenderer, Roo::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Roo(entities.back().get())));
        }
    }
}

void loadLevelFromCSV(std::string& filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns, std::vector<SDL_Rect>& teleports) {
    std::ifstream file((filePath).c_str());
    if (!file.is_open()) {
        SDL_Log("Could not load level file!");
    }
    constexpr int MAX_ROWS = 100;
    constexpr int MAX_COLS = 16;
    std::string data[MAX_ROWS][MAX_COLS];
    std::string line;
    int row = 0;
    // Store the CSV data from the CSV file to the 2D array
    while (getline(file, line) && row < MAX_ROWS) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (getline(ss, cell, ',') && col < MAX_COLS) {
            data[row][col] = cell;
            col++;
        }
        row++;
    }
    file.close();
    state.levelHeight = row*TILE_SIZE;

    if(!state.levelMap.empty()) {
        state.levelMap.clear();
    }

    for (int i = 0; i < row; i++) {
        std::vector<int> mapRow;
        for(int j = 0; j < MAX_COLS; j++) {
            int multiplier = j;
            if(std::stoi(data[i][j]) == 0) {
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(0);
            } else if(std::stoi(data[i][j]) == 3) {
                playerSpawns.emplace_back(scale(multiplier*TILE_SIZE-25),scale(i*TILE_SIZE+(TILE_SIZE-17-60)),scale(50),scale(60),1);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(1);
            } else if(std::stoi(data[i][j]) == 2) {
                enemySpawns.emplace_back(scale(multiplier*TILE_SIZE+(TILE_SIZE-50)/2),scale(i*TILE_SIZE+(TILE_SIZE-17-50)),scale(50),scale(50),2);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(2);
            }else if(std::stoi(data[i][j]) == 4) {
                SDL_Rect tempRect = {scale(multiplier*TILE_SIZE+(TILE_SIZE-50)/2),scale(i*TILE_SIZE+(TILE_SIZE-17-50)),scale(50),scale(50)};
                teleports.emplace_back(tempRect);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(3);
            } else {
                mapRow.push_back(-1);
            }
        }
        state.levelMap.push_back(mapRow);
    }
}

void loadController() {
    controller = SDL_GameControllerOpen(SDL_NumJoysticks()-1);
    if(controller == nullptr) {
        state.controller = false;
    } else {
        SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
        state.controller = true;
    }
}

bool init() {
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO ) < 0 ) {
        SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else if (TTF_Init() < 0) {
        SDL_Log( "TTF could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        gameWindow = SDL_CreateWindow("Hyper_Gravity 0.4.1 (Unstable)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (gameWindow == nullptr) {
            SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            gameRenderer = SDL_CreateRenderer(gameWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gameRenderer == nullptr) {
                SDL_Log("Game renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
                success = false;
            }
        }
        //SDL_SetWindowFullscreen(gameWindow,SDL_WINDOW_FULLSCREEN);

        loadController();
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

    }

    return success;
}

void close() {
    SDL_DestroyRenderer(gameRenderer);
    gameRenderer = nullptr;

    SDL_GameControllerClose(controller);
    controller = nullptr;

    SDL_DestroyWindow(gameWindow);
    gameWindow = nullptr;

    Mix_CloseAudio();

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}
