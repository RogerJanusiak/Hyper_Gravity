#include <list>
#include <vector>

#include "../../includes/Entities/Entity.h"
#include "../../includes/Entities/Robor.h"
#include "../../includes/Utils/GlobalConstants.h"
#include "../../includes/HEng/Platform.h"

Entity::Entity(const int x,const int y,const int Vx,const int Vy,SDL_Renderer *tempGameRenderer) {
    entityRect.x = x;
    entityRect.y = y;
    entityRect.w = 0;
    entityRect.h = 0;
    xVelocity = Vx;
    yVelocity = Vy;
    spawns = nullptr;
    gameRender = tempGameRenderer;
}

void Entity::render(int spriteX, int spriteY, bool faceVelocity, bool direction, double texturePercentage) {
    double hideTop = 1 - texturePercentage;

    int sourceHeight = std::round(srcHeight*texturePercentage);
    int displayHeight = std::round(entityRect.h*texturePercentage);

    int sourceY = std::round(spriteY*srcHeight+srcHeight*hideTop);
    int displayY = std::round(entityRect.y+entityRect.h*hideTop);

    entityTexture.setDimensions(entityRect.w,displayHeight);
    SDL_Rect sourceRect = {spriteX*srcWidth,sourceY,srcWidth,sourceHeight};

    if(faceVelocity) {
        if(xVelocity < 0) {
            entityTexture.render(entityRect.x,displayY,SDL_FLIP_HORIZONTAL,&sourceRect);
        } else {
            entityTexture.render(entityRect.x,displayY,SDL_FLIP_NONE,&sourceRect);
        }
    } else {
        if(direction) {
            entityTexture.render(entityRect.x,displayY,SDL_FLIP_NONE,&sourceRect);
        } else {
            entityTexture.render(entityRect.x,displayY,SDL_FLIP_HORIZONTAL,&sourceRect);
        }
    }

}

bool Entity::move(float dt,const std::list<Platform> &platforms, int* amountFallen, SDL_Rect* movementHitBox) {
    int startY = entityRect.y;
    getRect().x += getXVelocity()*dt;
    float nextYPosition = entityRect.y + yVelocity*dt;
    offPlatform = true;

    SDL_Rect movementBox;

    if(movementHitBox == nullptr) {
        movementBox.x = entityRect.x;
        movementBox.w = entityRect.w;
    } else {
        movementBox.x = movementHitBox->x;
        movementBox.w = movementHitBox->w;
    }

    movementBox.y = entityRect.y;
    movementBox.h = entityRect.h + (nextYPosition-entityRect.y);

    bool foundPlatform = false;
    for(auto& platform : platforms) {
        SDL_Rect platformRect = platform.getPlatformRect();
        if(entityRect.y+entityRect.h <= platformRect.y) {
            if(isColliding(platformRect,movementBox)) {
                offPlatform = false;
                entityRect.y = platformRect.y-entityRect.h;
                yVelocity = 0;
                foundPlatform = true;
            }
        }
    }

    if(!foundPlatform) {
        entityRect.y = nextYPosition;
        if(yVelocity <= TERMINAL_VELOCITY) {
            yVelocity += ACCELERATION*dt;
        }
    }

    if(amountFallen != nullptr) {
        *amountFallen = startY - entityRect.y;
    }

    if(getRect().x >= WINDOW_WIDTH) {
        setPosition(scale(10),getRect().y);
    } else if(getRect().x <= 0) {
        setPosition(WINDOW_WIDTH-scale(10), getRect().y);
    }

    return offPlatform;
}

// TODO: When something spawns make it choose a random direction, maybe except for edge of screen tiles
// TODO: Clean up this code
void Entity::spawn(bool spawnOnScreen) {
    int test2 =  rand() % spawns->size();
    auto currentSpawnIt = spawns->begin() + test2;
    if(!currentSpawnIt->getOccupied() && (!spawnOnScreen || currentSpawnIt->isOnScreen())) {
        offPlatform = false;
        spawned = true;
        justSpawned = true;
        setPosition(currentSpawnIt->getX(),currentSpawnIt->getY()+currentSpawnIt->getRect().h-entityRect.h);
        setYVelocity(0);
        currentSpawnIt->setOccupied(true);
    }
}

void Entity::forceSpawn() {
    if(spawns != nullptr) {
        for(auto it = spawns->begin(); it != spawns->end(); it++) {
            offPlatform = false;
            spawned = true;
            setPosition(it->getX(),it->getY()+it->getRect().h-entityRect.h);
            setYVelocity(0);
            it->setOccupied(true);
            break;
        }
    } else {
        SDL_Log("Spawns not set for entity. forceSpawn()");
    }
}

bool Entity::isColliding(SDL_Rect& rectA, const SDL_Rect& rectB) {
    if (rectA.y + rectA.h >= rectB.y && rectA.y <= rectB.y + rectB.h && rectA.x + rectA.w >= rectB.x && rectA.x <= rectB.x + rectB.w) {
        return true;
    }
    return false;
}

bool Entity::damage(int damageAmount) {
    hp -= damageAmount;
    if(hp <= 0) {
        alive = false;
        return true;
    }
    return false;
}

void Entity::despawn() {
    spawned = false;
    justSpawned = true;
    entityRect.x = -1000;
    entityRect.y = -1000;
}




