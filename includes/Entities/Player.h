#ifndef PLAYER_H
#define PLAYER_H
#include <list>

#include "../HEng/Platform.h"
#include "../TEng/Texture.h"

#include "Entity.h"
#include "../TEng/Sound.h"
#include "../HEng/Weapon.h"

class Player {
public:
    Player(Entity* entity, GlobalGameState& ggs);

    int move(GlobalGameState& ggs ,const std::list<Platform> &platforms, std::vector<SDL_Rect>& teleports);
    void render() const;

    [[nodiscard]] const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    [[nodiscard]] bool getDirection() const { return playerDirection; }

    [[nodiscard]] Entity* getEntity() const { return playerEntity; }

    [[nodiscard]] Weapon* getWeapon() const { return currentWeapon; }
    void setPrimaryWeapon(Weapon* priWeapon) { primaryWeapon = priWeapon; }
    void setSecondaryWeapon(Weapon* secWeapon) { secondaryWeapon = secWeapon; currentWeapon = primaryWeapon; }
    [[nodiscard]] Weapon* getSecondaryWeapon() const { return secondaryWeapon; }
    [[nodiscard]] Weapon* getPrimaryWeapon() const { return primaryWeapon; }
    void changeWeapon();

    bool damage();
    [[nodiscard]] int getHealth() const { return health; }
    [[nodiscard]] double getHealthPercentage() const { return health/static_cast<double>(maxHealth); }

    [[nodiscard]] int getPower() const { return power; }
    [[nodiscard]] double getPowerPercentage() const { return power/static_cast<double>(maxPower); }
    void changePower(const int change) { power += change; }

    void fullHealth() { health = maxHealth; }

    void killEnemy();
    void zeroCombo() { combo = 0;}
    [[nodiscard]] int getCombo() const { return combo; }

    Weapon& getWeapon(int weapon);

    void changeXP(int _xp) { xp += _xp; }
    void setXP(int _xp ) { xp = _xp; }
    [[nodiscard]] int getXP() const { return xp; }

    [[nodiscard]] bool isInvincible() const { return invincible; }
    void setInvincible(bool _invincible) { invincible = _invincible; }
    void tickInvicibilty(float dt);

    void setXNormalV(int vx) { xNormalVelocity = vx; }

    [[nodiscard]] SDL_Rect getHitRect() const { return playerHitRect; }
    SDL_Rect* getWheelRect() { return &wheelRect; }
    void updateWheelRect() {
        wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
        wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;
    }

    [[nodiscard]] int getAbilityKills() const { return abilitiesKills; }

    [[nodiscard]] bool isOnPlatform() const { return onPlatform; }
    void startGroundPound();
    [[nodiscard]] bool hasGroundPounded() const { return groundPounded; }
    void executedGroundPount();

    void activateShield();
    void runShield();
    void deactivateShield() { shieldActive = false;}
    [[nodiscard]] bool isShieldActive() const { return shieldActive; }

    int abilitiesKills = 0;

    Weapon revolver;
    Weapon rifle;
    Weapon shotgun;
    Weapon laserPistol;


    bool inShieldJump = false;
    double shieldAngle = 360;

private:
    const int width = 18;
    const int height = 23;

    const int playerWidth = scale(width);
    const int playerHeight = scale(height);

    bool shieldActive = false;
    float shieldActiveTimer = 0;

    GlobalGameState& ggs;

    bool playerDirection = false;

    Entity* playerEntity;

    SDL_Rect playerHitRect;
    SDL_Rect wheelRect;

    Texture playerTexture;
    Texture shieldTexture;

    Weapon* currentWeapon = nullptr;
    Weapon* primaryWeapon = nullptr;
    Weapon* secondaryWeapon = nullptr;

    float postDamageInvincibleTime = 0;
    bool invicibleFromDeath = false;

    int health = 200;
    const int maxHealth = 200;
    double power = 100;
    const int maxPower = 200;

    bool onPlatform = true;
    bool doingGroundPound = false;
    bool groundPounded = false;


    int xp = 0;

    int combo = 0;

    const int defaultXSpeed = scale(175);
    int xNormalVelocity = 0;

    Texture wheelTexture;

    bool charged = true;
    bool justCharged = false;

    bool invincible = false;

    float timeSinceShot;

    SDL_Rect weaponRect;

    Sound damageSound;
    Sound groundPoundEnd;

    int numberOfPlayerUpgrades = 4;

};

#endif //PLAYER_H
