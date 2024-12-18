#include "../includes/Player.h"
#include "../includes/Entity.h"
#include "../includes/Bullet.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Player::Player(Entity* entity, Weapon* primaryWeapon) : playerEntity(entity), primaryWeapon(primaryWeapon) {

    currentWeapon = primaryWeapon;

    damageSound.init("resources/sounds/playerDamage.wav",0,-1);

    wheelRect.w = scale(13);
    wheelRect.h = scale(13);

    weaponRect.x = 0;
    weaponRect.y = 0;
    weaponRect.w = scale(42);
    weaponRect.h = scale(10);

    playerEntity->setDimensions(playerWidth,playerHeight);
    playerTexture.setup(playerWidth,playerHeight,playerEntity->getRenderer());

    c4Texture.setup(scale(32),scale(32),playerEntity->getRenderer());
    detinatorTexture.setup(scale(32),scale(32),playerEntity->getRenderer());

    if(!playerTexture.loadFromFile("Timpy.png")) {
        SDL_Log("Could not load Timpy texture!");
    }

    if(!c4Texture.loadFromFile("C4.png")) {
        SDL_Log("Could not load c4 texture!");
    }
    if(!detinatorTexture.loadFromFile("Detinator.png")) {
        SDL_Log("Could not load detinator texture!");
    }

    c4Entity.setTexture(c4Texture);
    c4Entity.setDimensions(scale(32),scale(32));
    c4Entity.setSource(32,32);
    playerEntity->setTexture(playerTexture);
    playerEntity->setSource(26,32);

}

void Player::render() const {
    if(c4Placed) {
        c4Entity.render(0,0);
    }

    playerEntity->render(0,0, false, playerDirection);

    if(!c4Placed) {
        currentWeapon->render(playerEntity->getRect().x,playerEntity->getRect().y,playerDirection);
    } else {
        if(playerDirection) {
            detinatorTexture.render(playerEntity->getRect().x+scale(35),playerEntity->getRect().y+scale(12));
        } else {
            detinatorTexture.render(playerEntity->getRect().x-scale(17),playerEntity->getRect().y+scale(12),SDL_FLIP_HORIZONTAL);
        }
    }
}

int Player::move(float dt,const std::list<Platform*> &platforms, State& state) {
    double speedIncrease = state.playerLevels[speed] == 0 ? 0 : defaultXSpeed*state.playerProperties[speed][state.playerLevels[speed]-1][1]/100;
    getEntity()->setXVelocity(xNormalVelocity*(defaultXSpeed+speedIncrease));

    if(c4Placed) {
        c4Entity.move(dt,platforms);
    }

    int amountFallen = 0;
    if(!playerEntity->move(dt,platforms,&amountFallen,&wheelRect) && invincible && !invicibleFromDeath) {
        invincible = false;
        charged = false;
    }

    if(playerDirection) {
        weaponRect.x = playerEntity->getRect().x+scale(40);
    } else {
        weaponRect.x = playerEntity->getRect().x-scale(27);
    }
    weaponRect.y = playerEntity->getRect().y+scale(20);

    if(playerEntity->getRect().x >= WINDOW_WIDTH) {
        playerEntity->setPosition(-scale(40),playerEntity->getRect().y);
    } else if(playerEntity->getRect().x < -scale(40)) {
        playerEntity->setPosition(WINDOW_WIDTH, playerEntity->getRect().y);
    }

    if(playerEntity->getRect().y > WINDOW_HEIGHT) {
        playerEntity->forceSpawn();
    }

    wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
    wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;

    playerHitRect.x = getEntity()->getRect().x+scale(12);
    playerHitRect.y = getEntity()->getRect().y;
    playerHitRect.w = getEntity()->getRect().w-scale(25);
    playerHitRect.h = getEntity()->getRect().h;

    return amountFallen;
}

void Player::setDirection(bool direction) {
    playerDirection = direction;
}

void Player::killEnemy(State& state) {
    combo++;
    double mutiplier = state.playerLevels[PlayerUpgrades::shield] == 0 ? 1 : state.playerProperties[PlayerUpgrades::shield][state.playerLevels[PlayerUpgrades::shield]-1][1];
    shield += combo*mutiplier;
    shield = shield >= maxShield ? maxShield : shield;
}

int Player::charge(State& state) {
    int abilityLevel = state.abilityLevels[currentAbility] == 0 ? 0 : state.abilityLevels[currentAbility] - 1;
    int abilityReloadSpeed = state.abilityProperties[currentAbility][abilityLevel][1];
    if (state.abilitiesKills >= abilityReloadSpeed) {
        state.abilitiesKills = 0;
        charged = true;
        justCharged = true;
        return 75;
    }
    if(!charged) {
        return 75*state.abilitiesKills/abilityReloadSpeed;
    }
    return 75;
}

void Player::useAbility(State& state) {

    if(c4Placed) {
        state.c4Exploded = true;
        c4Placed = false;
        state.c4Placed = false;
    } else if (charged) {
        switch(currentAbility) {
            case teleport: {
                state.teleportSelection = !state.teleportSelection;
                state.teleportCursorX = getEntity()->getRect().x;
                state.teleportCursorY = getEntity()->getRect().y;
                state.startSelection = SDL_GetTicks();
            } break;
            case c4: {
                if (!c4Placed) {
                    c4Entity.setPhysics(playerEntity->getRect().x,playerEntity->getRect().y+playerEntity->getRect().h - scale(32),0,0);
                    c4Placed = true;
                    state.c4Placed = true;
                }
            } break;
            default:
                break;
        }
        charged = false;
    }

}

void Player::tickInvicibilty(float dt) {
    postDamageInvincibleTime += dt;
    if(postDamageInvincibleTime > 3 && invicibleFromDeath) {
        invicibleFromDeath = false;
        setInvincible(false);
    }
}


bool Player::damage(State& state) {

    zeroCombo();
    charge(state);
    setInvincible(true);
    invicibleFromDeath = true;
    postDamageInvincibleTime = 0;

    shield -= state.playerLevels[armor] == 0 ? 50 : 50 - 50*state.playerProperties[armor][state.playerLevels[armor]-1][1]/100;
    if(shield <= 0) {
        health += shield;
        if (health <= 0) {
            health = maxHealth;
            combo = 0;
            c4Placed = false;
            return true;
        }
        shield = 0;
    }
    damageSound.play();
    return false;
}

void Player::changeWeapon() {

    if(currentWeapon == primaryWeapon && secondaryWeapon != nullptr) {
        currentWeapon = secondaryWeapon;
    } else if(currentWeapon == secondaryWeapon) {
        currentWeapon = primaryWeapon;
    }

}

void Player::reset(State& state) {
    health = 200;
    shield = 0;
    xp = 50;
    setAbility(none);
    state.weapon1 = 0;
    state.weapon2 = -1;
    state.abilitiesKills = 0;
    charge(state);
}
