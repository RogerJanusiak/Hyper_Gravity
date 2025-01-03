#include "../../includes/HEng/Weapon.h"
#include "../../includes/Utils/GlobalConstants.h"
#include "../../includes/HEng/State.h"

void Weapon::setDefaultProperties() {
  switch (type) {
      default: {
          clipSize = 4;
          reloadSpeed = 3;
          bulletDurability = 1;
          bulletStrength = 1;
          bulletDamage = 1;
      } break;
      case rifle: {
          clipSize = 2;
          reloadSpeed = 3;
          bulletDurability = 1;
          bulletStrength = 1;
          bulletDamage = 2;
      }break;
      case shotgun: {
          clipSize = 1;
          reloadSpeed = 2;
          bulletDurability = 1;
          bulletStrength = 1;
          bulletDamage = 1;
          bulletsPerShot = 3;
      }break;
      case laserPistol: {
          clipSize = 2;
          reloadSpeed = 3;
          coolFireRate = 0.5;
          bulletsInClip = 0;
      } break;
  }
}

Weapon::Weapon(GlobalGameState& ggs, const Weapon_Type type, SDL_Renderer* renderer) : ggs(ggs), renderer(renderer), type(type) {
  setDefaultProperties();
  switch (type) {
      default: {
          texture.setup(scale(14),scale(7),renderer);
          if(!texture.loadFromFile("revolver.png")) {
            SDL_Log("Could not load revolver texture!");
          }

          relXRight = scale(15);
          relXLeft = -scale(12);
          relY = scale(10);

          bulletRelXRight = scale(29);
          bulletRelXLeft = -scale(13);
          bulletRelY = scale(10);

          fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
          reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
          emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
      } break;
      case rifle: {
          texture.setup(scale(32*2),scale(12*2),renderer);
          if(!texture.loadFromFile("rifle.png")) {
            SDL_Log("Could not load revolver texture!");
          }

          relXRight = scale(20);
          relXLeft = -scale(30);
          relY = scale(15);

          bulletRelXRight = scale(19);
          bulletRelXLeft = 0;
          bulletRelY = scale(19);

          //TODO: Add unique sounds
          fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
          reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
          emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
      }break;
      case shotgun: {
          texture.setup(scale(19),scale(6),renderer);
          if(!texture.loadFromFile("shotgun.png")) {
            SDL_Log("Could not load revolver texture!");
          }

          relXRight = scale(9);
          relXLeft = -scale(10);
          relY = scale(12);

          bulletRelXRight = scale(29);
          bulletRelXLeft = -scale(11);
          bulletRelY = scale(12);

          //TODO: Add unique sounds
          fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
          reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
          emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
      }break;
      case laserPistol: {
          texture.setup(scale(42),scale(21),renderer);
          if(!texture.loadFromFile("laserPistol.png")) {
            SDL_Log("Could not load laserPistol texture!");
          }

          relXRight = scale(30);
          relXLeft = -scale(22);
          relY = scale(12);

          bulletRelXRight = scale(30);
          bulletRelXLeft = 0;
          bulletRelY = scale(17);
          bulletType = laser;

          timeSinceShot = reloadSpeed;
          fireSound.init("resources/sounds/laserPistol-shoot.wav",0,-1);
          reloadSound.init("resources/sounds/laserPistol-reload.wav", 0,-1);
          emptySound.init("resources/sounds/laserPistol-empty.wav", 0,-1);
      } break;
  }
  reset();
}

void Weapon::render(const int playerX, const int playerY, const bool playerDirection) const {

  if(playerDirection) {
    texture.render(playerX+relXRight,playerY+relY);
  } else {
    texture.render(playerX+relXLeft,playerY+relY, SDL_FLIP_HORIZONTAL);
  }

}

int Weapon::reload(float dt) {
  if(!reloaded) {
      if (timeSinceShot >= reloadSpeed) {
        timeSinceShot = 0;
        reloaded = true;
        if(type == laserPistol) {
          bulletsInClip = 0;
        } else {
          bulletsInClip = clipSize;
        }
        justReloaded = true;
        return 75;
      }
      timeSinceShot += dt;
      return 75*timeSinceShot*(1/reloadSpeed)-2;
  }
  timeSinceShot += dt;
  return 75;
}

void Weapon::forceReload() {
  timeSinceShot = 0;
  reloaded = false;
  bulletsInClip = 0;
}

bool Weapon::wasJustReloaded() {
  if(justReloaded) {
    justReloaded = false;
    reloadSound.play();
    return true;
  }
  return false;
}

bool Weapon::shoot(std::list<Entity>& eBullets, std::list<Bullet>& bullets, bool direction, int playerX, int playerY) {
  if(reloaded) {
    fireSound.play();

    for(int i = 0; i < bulletsPerShot; i++) {
      if(direction) {
        eBullets.emplace_back(playerX+bulletRelXRight,playerY+bulletRelY,bulletSpeed+(i)*75,-150*i,renderer);
      } else {
        eBullets.emplace_back(playerX+bulletRelXLeft,playerY+bulletRelY,-bulletSpeed+(i)*75,-150*i,renderer);
      }
      bullets.emplace_back(&eBullets.back(), bulletType, bulletDurability, bulletStrength, bulletDamage);
      bullets.back().setIterator(--eBullets.end());
    }

    if(type == laserPistol) {
      if(timeSinceShot <= coolFireRate) {
        bulletsInClip++;
        if(bulletsInClip >= clipSize) {
          reloaded = false;
        }
      }
    } else {
      bulletsInClip--;
      if(bulletsInClip <= 0) {
        reloaded = false;
      }
    }


    timeSinceShot = 0;
    return true;
  }
  emptySound.play();
  return false;
}

void Weapon::reset() {
  timeSinceShot = 0;
  reloaded = true;
  justReloaded = false;
  if(type == laserPistol) {
    bulletsInClip = 0;
  } else {
    bulletsInClip = clipSize;
  }
}

void Weapon::applyUpgrade(Augment* augment) {
    switch(augment->id) {
      case damage:
        bulletDamage += augment->level;
        break;
      case clipIncrease:
        clipSize += augment->level;
        break;
      case AID::reload:
        reloadSpeed = reloadSpeed*(1-0.25*augment->level);
        break;
      case strength:
        bulletStrength += augment->level;
        break;
      case durability:
        bulletDurability += augment->level;
        break;
    }
}

void Weapon::upgrade() {
  setDefaultProperties();
  if(primaryAugment != nullptr) {
    applyUpgrade(primaryAugment);
  }
  if(secondaryAugment != nullptr) {
    applyUpgrade(secondaryAugment);
  }
}