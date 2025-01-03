#include <memory>
#include "../../includes/HEng/WaveController.h"
#include "../../includes/Utils/Input.h"

WaveController::WaveController(GlobalGameState& ggs, Run& run) : ggs(ggs), run(run), player(run.getPlayer()) {
    timeToShootBack.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToShootBack.y = WINDOW_HEIGHT-scalePlayerUI(50);
    timeToShootBack.w = scalePlayerUI(75);
    timeToShootBack.h = scalePlayerUI(15);

    timeToShoot.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToShoot.y = WINDOW_HEIGHT-scalePlayerUI(50);
    timeToShoot.w = scalePlayerUI(75);
    timeToShoot.h = scalePlayerUI(15);

    timeToAbilityBack.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToAbilityBack.y = WINDOW_HEIGHT-scalePlayerUI(75);
    timeToAbilityBack.w = scalePlayerUI(75);
    timeToAbilityBack.h = scalePlayerUI(15);

    timeToAbility.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToAbility.y = WINDOW_HEIGHT-scalePlayerUI(75);
    timeToAbility.w = scalePlayerUI(75);
    timeToAbility.h = scalePlayerUI(15);

    healthBackRect.x = scalePlayerUI(10);
    healthBackRect.y = WINDOW_HEIGHT-scalePlayerUI(20);
    healthBackRect.w = scalePlayerUI(75);
    healthBackRect.h = scalePlayerUI(15);

    healthRect.x = scalePlayerUI(10);
    healthRect.y = WINDOW_HEIGHT-scalePlayerUI(20);
    healthRect.w = scalePlayerUI(75);
    healthRect.h = scalePlayerUI(15);

    powerBackRect.w = scalePlayerUI(100);
    powerBackRect.h = scalePlayerUI(15);
    powerBackRect.x = (WINDOW_WIDTH-powerBackRect.w)/2;
    powerBackRect.y = scalePlayerUI(10);

    powerRect.w = scalePlayerUI(0);
    powerRect.h = powerBackRect.h;
    powerRect.x = powerBackRect.x;
    powerRect.y = powerBackRect.y;

    healthText.setup(ggs.renderer);
    healthText.loadFromRenderedText("200", ggs.black,ggs.verySmall);

    waveNumberText.setup(ggs.renderer);
    waveNumberTitle.setup(ggs.renderer);
    comboNumberText.setup(ggs.renderer);
    playerXPText.setup(ggs.renderer);
    fpsText.setup(ggs.renderer);

    startWave();
    startLevel();
    updateWaveText();
}

void WaveController::operate() {
	if(currentWave != nullptr) {
		currentWave->render();
	    operatePlayer();
	    renderPlayerUI();
	    renderWaveText();
		if(!currentWave->runWave()) {
		    run.nextWave();
		    startWave();
		}
	}
    if(ggs.updateText) {
        updatePlayerUIText();
        ggs.updateText = false;
    }

}

void WaveController::operatePlayer() {
    player.move(ggs, run.getLevel().getPlatforms(), run.getLevel().getTeleports());
    player.runShield();
    player.tickInvicibilty(ggs.dt);
    updateTimeToShoot(scalePlayerUI(player.getWeapon()->reload(ggs.dt)));
    player.getWeapon()->wasJustReloaded();
    player.render();
}

void WaveController::updateTimeToShoot(const double width) {
    timeToShoot.w = width;
}

void WaveController::startLevel() const {
    player.getEntity()->setSpawns(run.getLevel().getPlayerSpawns());
    player.getEntity()->forceSpawn();
}

void WaveController::startWave() {
	inWave = true;
    if(run.getWaveNumber() % 1 == 0 || run.getWaveNumber() != 1) {
        augmentFound = true;
        stopMovement();
        rightMovement = false;
        leftMovement = false;
    }
	currentWave = std::make_unique<Wave>(ggs,run.getPlayer(), run.getLevel(), run.getWaveNumber());
}

void WaveController::readInput() {
	SDL_Event e;

    // TODO: Roborto pathing is having them go left out of a spawn at tileX = 0...
	while(SDL_PollEvent(&e) != 0) {
		if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_1) {
                ggs.developerMode = !ggs.developerMode;
            } else if(e.key.keysym.sym == SDLK_2 && ggs.developerMode) {
                ggs.pauseEnemies = !ggs.pauseEnemies;
            } else if(e.key.keysym.sym == SDLK_ESCAPE) {
                ggs.toPauseMenu = true;
            } else if(e.key.keysym.sym == SDLK_d) {
                player.setXNormalV(1);
                rightMovement = true;
            } else if(e.key.keysym.sym == SDLK_a) {
                player.setXNormalV(-1);
                leftMovement = true;
            } else if(e.key.keysym.sym == SDLK_l) {
                player.setDirection(true);
            } else if(e.key.keysym.sym == SDLK_j) {
                player.setDirection(false);
            } else if(e.key.keysym.sym == SDLK_w) {
                player.changeWeapon();
            } else if(e.key.keysym.sym == SDLK_r) {
                player.getWeapon()->forceReload();
            } else if(e.key.keysym.sym == SDLK_e) {
                openInventory = true;
                rightMovement = false;
                leftMovement = false;
            } else if(e.key.keysym.sym == SDLK_SPACE) {
                if(shootingReset && currentWave != nullptr) {
                    player.getWeapon()->shoot(currentWave->getBulletEntities(),currentWave->getBullets(),player.getDirection(),player.getEntity()->getRect().x,player.getEntity()->getRect().y);
                    shootingReset = false;
                }
            }
        } else if(e.type == SDL_KEYUP) {
            if(e.key.keysym.sym == SDLK_d)
                rightMovement = false;
            if(e.key.keysym.sym == SDLK_a)
                leftMovement = false;
            if(!leftMovement && !rightMovement) {
                player.setXNormalV(0);
            } else if (leftMovement) {
                player.setXNormalV(-1);
            } else {
                player.setXNormalV(1);
            }

            if (e.key.keysym.sym == SDLK_SPACE) {
                shootingReset = true;
            }
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > JOYSTICK_DEAD_ZONE) {
                if(shootingReset && currentWave != nullptr) {
                    player.getWeapon()->shoot(currentWave->getBulletEntities(),currentWave->getBullets(),player.getDirection(),player.getEntity()->getRect().x,player.getEntity()->getRect().y);
                    shootingReset = false;
                }
            } else {
                shootingReset = true;
            }

            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT) > JOYSTICK_DEAD_ZONE) {
                if(player.getPower() > 0 && !player.isShieldActive() && !player.isDoingGroundPound()) {
                    player.activateShield();
                }
            } else {
                player.deactivateShield();
            }

            Sint16 x = SDL_GameControllerGetAxis(ggs.controller, SDL_CONTROLLER_AXIS_RIGHTX);
            Sint16 y = SDL_GameControllerGetAxis(ggs.controller, SDL_CONTROLLER_AXIS_RIGHTY);

            const float maxAxisValue = 32768.0f; // SDL joystick axis range
            float normX = x / maxAxisValue;
            float normY = y / maxAxisValue;

            if (std::abs(normX) > 0.1f || std::abs(normY) > 0.1f) {
                // Calculate the angle in radians (atan2 handles Y-up coordinate systems)
                float angleRadians = std::atan2(-normY, -normX); // In SDL, Y-axis is inverted
                float angleDegrees = angleRadians * 180.0f / M_PI;

                // Convert angle to a 0-360 range (optional)
                if (angleDegrees < 0) {
                    angleDegrees += 360.0f;
                }

                player.shieldAngle = angleDegrees;
            }

            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) {
                rightMovement = true;
            } else {
                rightMovement = false;
            }

            if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) {
               leftMovement = true;
            } else {
                leftMovement = false;
            }

            if(!leftMovement && !rightMovement) {
                player.setXNormalV(0);
            } else if (leftMovement) {
                player.setXNormalV(-1);
            } else {
                player.setXNormalV(1);
            }

            // TODO: Move wave input to an function that is called by an enum so that code doesn't have to be replcated. Look at the menu code for example.
            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) > JOYSTICK_DEAD_ZONE) {
                player.setDirection(true);
            } else if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) < -JOYSTICK_DEAD_ZONE) {
                player.setDirection(false);
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            ggs.controller = nullptr;
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y) == 1) {
                player.changeWeapon();
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1) {
                ggs.toPauseMenu = true;
                stopMovement();
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) == 1) {
                player.getWeapon()->forceReload();
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK) == 1) {
                openInventory = true;
                stopMovement();
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                if(player.isOnPlatform() && ggs.playerTileY < run.getLevel().getMap().size()-1 && player.getPower() >= 25) {
                    player.changePower(-25);
                    player.getEntity()->setPosition(player.getEntity()->getRect().x, player.getEntity()->getRect().y+TILE_SIZE_SCALED);
                } else if(!player.isOnPlatform()) {
                    player.startGroundPound();
                }
            }
        }
    }
}

void WaveController::updatePlayerUIText() {
    updateWaveText();
    healthText.loadFromRenderedText(std::to_string(player.getHealth()), ggs.black, ggs.verySmall);
}

void WaveController::renderPlayerUI() {
    int bulletsInClip = player.getWeapon()->getBulletsInClip();
    SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(ggs.renderer,&timeToShootBack);

    SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ggs.renderer,&timeToShoot);

    healthRect.w = player.getHealthPercentage()*healthBackRect.w;
    powerRect.w = player.getPowerPercentage()*powerBackRect.w;

    SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(ggs.renderer,&healthBackRect);
    SDL_RenderFillRect(ggs.renderer, &powerBackRect);

    SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ggs.renderer,&healthRect);

    SDL_SetRenderDrawColor(ggs.renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(ggs.renderer,&powerRect);

    healthText.render(scalePlayerUI(12),WINDOW_HEIGHT-scalePlayerUI(19));

    for(int i = 0; i < player.getWeapon()->getClipSize(); i++) {
        if(bulletsInClip>i) {
            SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
        }
        SDL_Rect tempRect;
        tempRect.x = WINDOW_WIDTH-scalePlayerUI(30)-scalePlayerUI(20*i);
        tempRect.y = WINDOW_HEIGHT-scalePlayerUI(25);
        tempRect.w = scalePlayerUI(15);
        tempRect.h = scalePlayerUI(15);
        SDL_RenderFillRect(ggs.renderer,&tempRect);
    }

}

void WaveController::updateWaveText() {
    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(player.getCombo()), ggs.white, ggs.buttonFont);
    waveNumberText.loadFromRenderedText("" + std::to_string(run.getWaveNumber()), ggs.white, ggs.title);
    waveNumberTitle.loadFromRenderedText("Wave " + std::to_string(run.getWaveNumber()) + " Start!", ggs.white, ggs.title);
    playerXPText.loadFromRenderedText("XP: " + std::to_string(player.getXP()), ggs.white, ggs.buttonFont);
}

void WaveController::renderWaveText() {
    waveNumberText.render(scaleUI(10),scaleUI(5));
    //comboNumberText.render(scaleUI(10),scaleUI(30));
    //playerXPText.render(scaleUI(10),scaleUI(55));
    if(ggs.developerMode) {
        fpsText.loadFromRenderedText("FPS: " + std::to_string(ggs.fps), ggs.white, ggs.buttonFont);
        fpsText.render(scaleUI(10),scaleUI(80));
    }
    if(!currentWave->hasWaveStarted()) {
        waveNumberTitle.render((WINDOW_WIDTH-waveNumberTitle.getWidth())/2,scaleUI(200));
    }
}

