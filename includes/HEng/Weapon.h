#ifndef WEAPON_H
#define WEAPON_H
#include "../Entities/Bullet.h"
#include "../TEng/Sound.h"
#include "State.h"
#include "../TEng/Texture.h"
#include <list>

enum Weapon_Type {
  revolver,
  rifle,
  shotgun,
  laserPistol,
};

class Weapon {

public:

  explicit Weapon(GlobalGameState& ggs, Weapon_Type type, SDL_Renderer* renderer);

  void render(int playerX, int playerY, bool playerDirection) const;

  [[nodiscard]] Weapon_Type getType() const { return type; }
  Texture& getTexture() { return texture; }

  int reload(float dt);
  void forceReload();
  bool wasJustReloaded();

  bool shoot(std::list<Entity>& eBullets, std::list<Bullet>& bullets, bool direction, int playerX, int playerY);
  [[nodiscard]] int getBulletsInClip() const { return bulletsInClip; }
  [[nodiscard]] int getClipSize() const { return clipSize; }
  [[nodiscard]] int getDamage() const { return bulletDamage; }
  [[nodiscard]] int getReloadSpeed() const { return reloadSpeed; }

  [[nodiscard]] Augment* getPrimaryAugment() const { return primaryAugment; }
  [[nodiscard]] Augment* getSecondaryAugment() const { return secondaryAugment; }

  void removePrimaryAugment() {
    primaryAugment = nullptr;
    //if(secondaryAugment != nullptr) {
      //primaryAugment = secondaryAugment;
      //secondaryAugment = nullptr;
    //}
  }

  void removeSecondaryAugment() { secondaryAugment = nullptr; }

  void changePrimaryAugment(Augment* augment) { primaryAugment = augment; }
  void changeSecondaryAugment(Augment* augment) { secondaryAugment = augment; }

  void upgrade();

private:

  void setDefaultProperties();
  void applyUpgrade(Augment* augment);
  void reset();

  GlobalGameState& ggs;

  SDL_Renderer* renderer;

  Weapon_Type type;
  BULLET_TYPE bulletType = normal;

  int relXRight;
  int relXLeft;
  int relY;

  int bulletRelXRight = 0;
  int bulletRelXLeft = 0;
  int bulletRelY = 0;
  int bulletSpeed = scale(500);

  int clipSize = 0;
  int bulletDurability = 1;
  int bulletStrength = 1;
  int bulletDamage = 1;
  int bulletsPerShot = 1;
  double coolFireRate = 0.5;

  int bulletsInClip = 0;

  Texture texture;
  Sound fireSound;
  Sound reloadSound;
  Sound emptySound;

  double reloadSpeed = 0;

  bool reloaded = true;
  bool justReloaded = false;
  float timeSinceShot = 0;

  Augment* primaryAugment = nullptr;
  Augment* secondaryAugment = nullptr;

};

#endif //WEAPON_H
