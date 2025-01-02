#include <list>

#include "../../includes/Entities/Player.h"
#include "../../includes/Entities/Entity.h"
#include "../../includes/Utils/GlobalConstants.h"
#include "../../includes/HEng/Platform.h"

Player::Player(Entity* entity, GlobalGameState& ggs) : ggs(ggs), revolver(ggs, Weapon_Type::revolver,ggs.renderer), rifle(ggs, Weapon_Type::rifle,ggs.renderer), shotgun(ggs, Weapon_Type::shotgun,ggs.renderer), laserPistol(ggs, Weapon_Type::laserPistol,ggs.renderer), playerEntity(entity) {

    currentWeapon = &revolver;
    primaryWeapon = currentWeapon;
    secondaryWeapon = &shotgun;
    // TODO: Ability to chang weapon and stuff

    damageSound.init("resources/sounds/playerDamage.wav",0,-1);
    groundPoundEnd.init("resources/sounds/ground-pound-end.wav",0,-1);

    shieldTexture.setup(scale(44),scale(44),ggs.renderer);
    shieldTexture.loadFromFile("Shield.png");

    wheelRect.w = scale(6);
    wheelRect.h = scale(6);

    playerEntity->setDimensions(playerWidth,playerHeight);
    playerTexture.setup(playerWidth, playerHeight, ggs.renderer);

    if(!playerTexture.loadFromFile("timpy.png")) {
        SDL_Log("Could not load Timpy texture!");
    }

    playerEntity->setTexture(playerTexture);
    playerEntity->setSource(width,height);

    wheelTexture.setup(scale(8),scale(8),ggs.renderer);
    wheelTexture.loadFromFile("wheel.png");

}

void Player::render() const {
    if(isInvincible()) {
        SDL_SetTextureColorMod(getEntity()->getTexture()->getTexture(),159,150,255);
    } else {
        SDL_SetTextureColorMod(getEntity()->getTexture()->getTexture(),255,255,255);
    }

    int sprite = playerDirection ? 0 : 2;
    playerEntity->render(sprite,0);


    if(shieldActive)
        shieldTexture.render(playerEntity->getRect().x-scale(1.875*9), playerEntity->getRect().y-scale(1.875*6),SDL_FLIP_NONE,nullptr,shieldAngle);

    currentWeapon->render(playerEntity->getRect().x,playerEntity->getRect().y,playerDirection);
    SDL_Rect srcRect = {0,0,8,8};

    if(playerDirection) {
        wheelTexture.render(playerEntity->getRect().x+scale(5), playerEntity->getRect().y+scale(22),SDL_FLIP_NONE,&srcRect);
    } else {
        wheelTexture.render(playerEntity->getRect().x+scale(5), playerEntity->getRect().y+scale(22),SDL_FLIP_HORIZONTAL,&srcRect);
    }

}

int Player::move(GlobalGameState& ggs, const std::list<Platform> &platforms, std::vector<SDL_Rect>& teleports) {

    for(auto& teleport : teleports) {
        if(Entity::isColliding(playerEntity->getRect(),teleport)) {
            playerEntity->forceSpawn();
            return 0;
        }
    }

    getEntity()->setXVelocity(xNormalVelocity*defaultXSpeed);

    int amountFallen = 0;
    onPlatform = !playerEntity->move(ggs.dt,platforms,&amountFallen,&wheelRect);
    if(onPlatform && invincible && !invicibleFromDeath && !doingGroundPound) {
        invincible = false;
        charged = false;
    }

    if(onPlatform && inShieldJump && !doingGroundPound) {
        inShieldJump = false;
    }

    if(onPlatform && doingGroundPound) {
        groundPounded = true;
        doingGroundPound = false;
    }

    if(playerDirection) {
        weaponRect.x = playerEntity->getRect().x+scale(40);
    } else {
        weaponRect.x = playerEntity->getRect().x-scale(27);
    }
    weaponRect.y = playerEntity->getRect().y+scale(20);

    if(playerEntity->getRect().y > WINDOW_HEIGHT) {
        playerEntity->forceSpawn();
    }

    wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
    wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;

    playerHitRect.x = getEntity()->getRect().x+scale(12);
    playerHitRect.y = getEntity()->getRect().y;
    playerHitRect.w = getEntity()->getRect().w-scale(25);
    playerHitRect.h = getEntity()->getRect().h;

    ggs.playerX = playerEntity->getRect().x;
    ggs.playerTileX = ggs.playerX/TILE_SIZE_SCALED;
    ggs.playerTileY = playerEntity->getRect().y/TILE_SIZE_SCALED;

    if(ggs.developerMode) {
        SDL_SetRenderDrawColor(ggs.renderer,0,255,0,255);
        SDL_RenderDrawRect(ggs.renderer, &playerHitRect);
        SDL_RenderDrawRect(ggs.renderer, &wheelRect);
        SDL_RenderDrawRect(ggs.renderer, &getEntity()->getRect());
    }

    return amountFallen;
}

void Player::setDirection(bool direction) {
    playerDirection = direction;
}

void Player::killEnemy() {
    combo++;
    power += 10;
}

void Player::tickInvicibilty(float dt) {
    postDamageInvincibleTime += dt;
    if(postDamageInvincibleTime > 3 && invicibleFromDeath) {
        invicibleFromDeath = false;
        setInvincible(false);
    }
}

bool Player::damage() {

    zeroCombo();
    setInvincible(true);
    invicibleFromDeath = true;
    postDamageInvincibleTime = 0;

    health -= 50;
    if(health <= 0) {
       ggs.currentRunState = RunState::deathScreen;
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

Weapon& Player::getWeapon(const int weapon) {
    switch (weapon) {
        case Weapon_Type::revolver:
            return revolver;
            break;
        case Weapon_Type::rifle:
            return rifle;
            break;
        case Weapon_Type::shotgun:
            return shotgun;
            break;
        case Weapon_Type::laserPistol:
            return laserPistol;
            break;
        default:
            return revolver;
            break;
    }
}

void Player::startGroundPound() {
    if(inShieldJump && power >= 25) {
        power -= 25;
        doingGroundPound = true;
        invincible = true;
        getEntity()->setYVelocity(1500);
    }
}

void Player::executedGroundPount() {
    //groundPoundEnd.play();
    groundPounded = false;
    invincible = false;
}

void Player::activateShield() {
    if(getPower() >= 10) {
        shieldActive = true;
        shieldActiveTimer = 0;
        changePower(-10);
    }
}

void Player::runShield() {
    if(shieldActive) {
        shieldActiveTimer += ggs.dt;
        if(shieldActiveTimer >= 0.1) {
            shieldActiveTimer = 0;
            changePower(-1);
        }
        if(getPower() <= 0) {
            deactivateShield();
        }
    }
}


