#ifndef STATE_H
#define STATE_H

#include <SDL_gamecontroller.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_ttf.h>

enum class RunState {
    inWave,
    deathScreen,
    inventoryScreen,
};

enum Menu {
    head,
    level,
    settings,
    pause,
    weaponUpgrade,
    abilityUpgrade,
    playerUpgrade,
    notInMenu,
};

enum class AID {
    damage1,
    clipIncrease1,
  };

struct Augment {

    std::string name;
    std::string description;
    AID id;

    bool active;
    bool compadableWith[4];

};

struct GlobalGameState {
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_GameController *controller{};

    bool quit = false;

    bool inMainMenu = true;
    bool toMainMenu = false;
    bool inPauseMenu = false;
    bool toPauseMenu = false;
    bool inRun = false;

    RunState currentRunState = RunState::inventoryScreen;

    int level = 0;
    bool controllerStickReset = false;

    float dt = 0;
    float fps = 0;

    int playerTileX = 0;
    int playerTileY = 0;
    int playerX = 0;

    bool updateText = false;

    //Developer Options
    bool developerMode = false;
    bool pauseEnemies = false;

    //Global Resources
    SDL_Color white = {255,255,255};
    SDL_Color black = { 0, 0, 0 };

    TTF_Font *buttonFont{};
    TTF_Font *verySmall{};
    TTF_Font *title{};

    //Augments
    Augment damage1 = {"Damage 1", "Passive. Increase the damage of the weapon by 1.",AID::damage1,false,{true,true,true,true}};
    Augment clipIncrease1 = {"Clip Increase 1", "Passive. Increase the amount of ammo that can be shot before reloading by 1.",AID::clipIncrease1,false,{true,true,true,true}};

};

#endif //STATE_H
