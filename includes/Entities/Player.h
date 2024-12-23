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

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    [[nodiscard]] bool getDirection() const { return playerDirection; }

    [[nodiscard]] Entity* getEntity() const { return playerEntity; }

    Weapon* getWeapon() { return currentWeapon; }
    void setPrimaryWeapon(Weapon* priWeapon) { primaryWeapon = priWeapon; }
    void setSecondaryWeapon(Weapon* secWeapon) { secondaryWeapon = secWeapon; currentWeapon = primaryWeapon; }
    Weapon* getSecondaryWeapon() { return secondaryWeapon; }
    Weapon* getPrimaryWeapon() { return primaryWeapon; }
    void changeWeapon();

    bool damage();
    [[nodiscard]] int getHealth() const { return health; }
    [[nodiscard]] double getHealthPercentage() const { return health/static_cast<double>(maxHealth); }
    [[nodiscard]] int getShield() const { return shield; }
    [[nodiscard]] double getShieldPercentage() const { return shield/static_cast<double>(maxShield); }

    void fullHealth() { health = maxHealth; }
    void fillShield() { shield = maxShield; }
    void increaseShield(const int increaseAmount) {shield += increaseAmount;}

    void killEnemy();
    void zeroCombo() { combo = 0;}
    int getCombo() const { return combo; }

    Weapon& getWeapon(int weapon);

    void changeXP(int _xp) { xp += _xp; }
    void setXP(int _xp ) { xp = _xp; }
    int getXP() const { return xp; }

    [[nodiscard]] bool isInvincible() const { return invincible; }
    void setInvincible(bool _invincible) { invincible = _invincible; }
    void tickInvicibilty(float dt);

    void setXNormalV(int vx) { xNormalVelocity = vx; }

    SDL_Rect getHitRect() { return playerHitRect; }
    SDL_Rect* getWheelRect() { return &wheelRect; }
    void updateWheelRect() {
        wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
        wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;
    }

    [[nodiscard]] int getAbilityKills() const { return abilitiesKills; }

    int abilitiesKills = 0;

private:
    const int playerWidth = scale(50);
    const int playerHeight = scale(60);

    GlobalGameState& ggs;

    Weapon revolver;
    Weapon rifle;
    Weapon shotgun;
    Weapon laserPistol;

    bool playerDirection = false;

    Entity* playerEntity;

    SDL_Rect playerHitRect;
    SDL_Rect wheelRect;

    Texture playerTexture;

    Weapon* currentWeapon = nullptr;
    Weapon* primaryWeapon = nullptr;
    Weapon* secondaryWeapon = nullptr;

    float postDamageInvincibleTime = 0;
    bool invicibleFromDeath = false;

    int health = 200;
    const int maxHealth = 200;
    double shield = 0;
    const int maxShield = 200;

    int xp = 0;

    int combo = 0;

    const int defaultXSpeed = scale(350);
    int xNormalVelocity = 0;

    bool charged = true;
    bool justCharged = false;

    bool invincible = false;

    float timeSinceShot;

    SDL_Rect weaponRect;

    Sound damageSound;

    int numberOfPlayerUpgrades = 4;

};

#endif //PLAYER_H
