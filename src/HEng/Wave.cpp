#include "../../includes/HEng/Wave.h"
#include "../../includes/Entities/Robor.h"
#include "../../includes/Entities/Roborto.h"
#include "../../includes/Entities/Robro.h"
#include "../../includes/Entities//Romo.h"
#include "../../includes/Entities/Roo.h"

Wave::Wave(GlobalGameState& ggs, Player& timpy, Level& level, const int waveNumber) : ggs(ggs), player(timpy), level(level), waveNumber(waveNumber) {
    createEnemies();

    allCharacterEntities.push_back(timpy.getEntity());
    for(auto& entity : entities) {
        allCharacterEntities.push_back(entity.get());
    }

}

void Wave::render() {
	level.render();
    for (auto & bullet : bullets) {
        bullet.render();
        if(ggs.developerMode) {
            SDL_SetRenderDrawColor(ggs.renderer,0,0,255,255);
            SDL_Rect temp = bullet.getTrailingRect();
            SDL_RenderFillRect(ggs.renderer, &temp);
        }
    }

    for (const auto & enemy : enemies) {
        if(enemy->getEntity()->isAlive() && enemy->getEntity()->isSpawned()) {
            enemy->render();
            if(ggs.developerMode) {
                SDL_SetRenderDrawColor(ggs.renderer,255,0,0,255);
                SDL_Rect temp = enemy->getEntity()->getRect();
                SDL_RenderDrawRect(ggs.renderer, &temp);
            }
        }
    }

    for(auto it = explosions.begin();it != explosions.end();) {
        if(it->finished()) {
            it = explosions.erase(it);
        } else {
            it->render();
            it->advance();
            ++it;
        }
    }

    //wes->render();
}

float normalizeAngle(float angle) {
    while (angle < 0) {
        angle += 360.0f;
    }
    while (angle >= 360) {
        angle -= 360.0f;
    }
    return angle;
}

bool Wave::runWave() {
    bool playerDamaged = false;
    enemiesAlive = 0;

    timeSinceLoad += ggs.dt;
    if(timeSinceLoad > 1) {
        waveStarted = true;
    }

    /*for(auto& bullet : bullets) {
        bullet.move(ggs.dt, level.getPlatforms(), ggs.developerMode);
    }*/

    for (auto it = bullets.begin(); it != bullets.end();) {
        if(it->move(ggs.dt,level.getPlatforms(),ggs.developerMode)) {
            eBullets.erase(it->getIterator());
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    level.updateSpawns(allCharacterEntities);

    // TODO: Clean up this loop
    // TODO: Entity rect variable | can I put some of the code in functions?
    for (const auto & enemy : enemies) {
        bool firstLoop = false;
        if(!enemy->getEntity()->isSpawned()) {
            enemy->getEntity()->spawn(enemiesAlive <= 5);
            firstLoop = true;
        }
        if(enemy->getEntity()->isAlive() && enemy->getEntity()->isSpawned()) {
            bool abilityDamgage = false;

            if(!firstLoop && !ggs.pauseEnemies) {
                enemy->move(ggs, level.getPlatforms(),level);
            }
            if( Entity::isColliding(enemy->getEntity()->getRect(),player.getHitRect())) {
                if(!player.isInvincible()) {

                    bool avoidedAttack = false;

                    if(player.isShieldActive()) {
                        int centerX1 = enemy->getEntity()->getRect().x + enemy->getEntity()->getRect().w / 2;
                        int centerY1 = enemy->getEntity()->getRect().y + enemy->getEntity()->getRect().h / 2;

                        int centerX2 = player.getEntity()->getRect().x + player.getEntity()->getRect().w / 2;
                        int centerY2 = player.getEntity()->getRect().y + player.getEntity()->getRect().h / 2;

                        int dx = centerX2 - centerX1;
                        int dy = centerY2 - centerY1;

                        float angleRadians = std::atan2(dy, dx);
                        float angleDegrees = angleRadians * 180.0f / M_PI;

                        if (angleDegrees < 0) {
                            angleDegrees += 360.0f;
                        }

                        double shieldAngle = player.shieldAngle;

                        angleDegrees = normalizeAngle(angleDegrees);
                        shieldAngle = normalizeAngle(shieldAngle);

                        float difference = angleDegrees - shieldAngle;
                        if (difference > 180.0f) {
                            difference -= 360.0f;
                        } else if (difference < -180.0f) {
                            difference += 360.0f;
                        }

                        if(std::abs(difference) <= 90.0f) {
                            avoidedAttack = true;

                            angleDegrees += 180;
                            angleDegrees -= difference;

                            while (angleDegrees < 0) {
                                angleDegrees += 360.0f;
                            }
                            while (angleDegrees >= 360) {
                                angleDegrees -= 360.0f;
                            }

                            float Vx = -cos(angleDegrees* M_PI / 180.0)*scale(2000);
                            float Vy = -sin(angleDegrees* M_PI / 180.0)*scale(2000);

                            player.getEntity()->setXVelocity(Vx);
                            player.getEntity()->setYVelocity(Vy);

                            if(Vy < -scale(1000)) {
                                player.inShieldJump = true;
                            }

                            abilityDamgage = true;
                        }
                    }

                    if(!avoidedAttack) {
                        //TODO: Figure out if I need this:
                        if(player.damage()) {
                            //playerAlive = false;
                            waveNumber = 0;
                        }
                        SDL_GameControllerRumble( ggs.controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                        ggs.updateText = true;
                        playerDamaged = true;
                    }
                    enemy->getEntity()->damage(5);
                } else {
                    if(player.isShieldActive()) {
                        int centerX1 = enemy->getEntity()->getRect().x + enemy->getEntity()->getRect().w / 2;
                        int centerY1 = enemy->getEntity()->getRect().y + enemy->getEntity()->getRect().h / 2;

                        int centerX2 = player.getEntity()->getRect().x + player.getEntity()->getRect().w / 2;
                        int centerY2 = player.getEntity()->getRect().y + player.getEntity()->getRect().h / 2;

                        int dx = centerX2 - centerX1;
                        int dy = centerY2 - centerY1;

                        float angleRadians = std::atan2(dy, dx);
                        float angleDegrees = angleRadians * 180.0f / M_PI;

                        if (angleDegrees < 0) {
                            angleDegrees += 360.0f;
                        }

                        double shieldAngle = player.shieldAngle;

                        angleDegrees = normalizeAngle(angleDegrees);
                        shieldAngle = normalizeAngle(shieldAngle);

                        float difference = angleDegrees - shieldAngle;
                        if (difference > 180.0f) {
                            difference -= 360.0f;
                        } else if (difference < -180.0f) {
                            difference += 360.0f;
                        }

                        if(std::abs(difference) <= 90.0f) {

                            angleDegrees += 180;
                            angleDegrees -= difference;

                            while (angleDegrees < 0) {
                                angleDegrees += 360.0f;
                            }
                            while (angleDegrees >= 360) {
                                angleDegrees -= 360.0f;
                            }

                            float Vx = -cos(angleDegrees* M_PI / 180.0)*scale(2000);
                            float Vy = -sin(angleDegrees* M_PI / 180.0)*scale(2000);

                            player.getEntity()->setXVelocity(Vx);
                            player.getEntity()->setYVelocity(Vy);

                            if(Vy < -scale(1000)) {
                                player.inShieldJump = true;
                            }

                            abilityDamgage = true;
                            enemy->getEntity()->damage(maxEnemyHealth);
                        }

                    }
                }
            }
            for(auto bit = bullets.begin(); bit != bullets.end();) {
                if(Entity::isColliding(enemy->getEntity()->getRect(),bit->getTrailingRect())) {
                    if(bit->decreaseStrength()) {
                        eBullets.erase(bit->getIterator());
                        bit = bullets.erase(bit);
                    } else {
                        ++bit;
                    }
                    enemy->getEntity()->damage(player.getWeapon()->getDamage());
                    break;
                }
                ++bit;
            }
            if(player.hasGroundPounded()) {
                if(enemy->getEntity()->getRect().x < player.getEntity()->getRect().w + player.getEntity()->getRect().x + groundPoundRadius &&
                    enemy->getEntity()->getRect().x > player.getEntity()->getRect().x - groundPoundRadius &&
                    enemy->getEntity()->getRect().y < player.getEntity()->getRect().h + player.getEntity()->getRect().y &&
                    enemy->getEntity()->getRect().y > player.getEntity()->getRect().y) {
                    enemy->getEntity()->damage(maxEnemyHealth);
                    abilityDamgage = true;
                }
            }
            if(!enemy->getEntity()->isAlive()) {
                explosions.emplace_back(enemy->getEntity()->getRect().x+enemy->getEntity()->getRect().w/2,enemy->getEntity()->getRect().y+enemy->getEntity()->getRect().h/2,ggs.renderer);
                if(!playerDamaged) {
                    player.changeXP(enemy->getDifficulty());
                    if(!abilityDamgage) {
                        player.killEnemy();
                    }
                }
                ggs.updateText = true;
            } else {
                for(auto& teleport : level.getTeleports()) {
                    if(Entity::isColliding(enemy->getEntity()->getRect(),teleport)) {
                        enemy->getEntity()->despawn();
                    }
                }

            }
        }
        if(enemy->getEntity()->isAlive()) {
            enemiesAlive++;
        }
    }

    if(player.hasGroundPounded()) {
        player.executedGroundPount();
    }

    if(enemiesAlive == 0) {
        return false;
    }

    return true;
}

void Wave::createEnemies() {
	int totalDifficulty = -50;
    int (*weights)[5] = level.getSpawnWeights();
    std::vector<Spawn>* spawns = level.getEnemySpawns();
    while(totalDifficulty < waveNumber) {
        int enemyType = rand() % 100 + 1;
        totalDifficulty += Robor::difficulty;
        entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Robor::health));
        enemies.emplace_back(std::unique_ptr<Enemy>(new Robor(entities.back().get())));
        /*if(enemyType <= (*weights)[0] && totalDifficulty+Robor::difficulty <= waveNumber) {
            totalDifficulty += Robor::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Robor::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Robor(entities.back().get())));
        } else if(enemyType > (*weights)[0] && enemyType <= (*weights)[1] && totalDifficulty+Roborto::difficulty <= waveNumber) {
            totalDifficulty += Roborto::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Roborto::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Roborto(entities.back().get())));
        } else if(enemyType > (*weights)[1] && enemyType <= (*weights)[2] && totalDifficulty+Robro::difficulty <= waveNumber) {
            totalDifficulty += Robro::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Robro::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Robro(entities.back().get())));
        } else if(enemyType > (*weights)[2] && enemyType <= (*weights)[3] && totalDifficulty+Romo::difficulty <= waveNumber) {
            totalDifficulty += Romo::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Romo::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Romo(entities.back().get())));
        } else if(enemyType > (*weights)[3] && enemyType <= (*weights)[4] && totalDifficulty+Roo::difficulty <= waveNumber) {
            totalDifficulty += Roo::difficulty;
            entities.emplace_back(std::make_unique<Entity>(spawns, ggs.renderer, Roo::health));
            enemies.emplace_back(std::unique_ptr<Enemy>(new Roo(entities.back().get())));
        }*/
    }
}
