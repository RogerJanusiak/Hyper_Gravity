#ifndef STATE_H
#define STATE_H

#include <SDL_gamecontroller.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_ttf.h>

enum class RunState {
    inWave,
    deathScreen,
    inMenu
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
    std::string descriptionLine1;
    std::string descriptionLine2;
    std::string image;
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

    RunState currentRunState = RunState::inWave;

    int level = 0;
    bool controllerStickReset = false;

    float dt = 0;
    float fps = 0;

    int playerTileX = 0;
    int playerTileY = 0;
    int playerX = 0;

    bool updateText = false;

    bool weaponSelected = false;
    int selectedWeapon = 0;

    bool augmentRemoved = false;
    int removedAugment = 0;

    //Developer Options
    bool developerMode = false;
    bool pauseEnemies = false;

    //Global Resources
    SDL_Color white = {255,255,255};
    SDL_Color black = { 0, 0, 0 };

    TTF_Font *buttonFont{};
    TTF_Font *verySmall{};
    TTF_Font *title{};

    TTF_Font *augTitle{};
    TTF_Font *augDescription{};

    //Augments
    Augment damage1 = {"Damage 1", "Passive | All Weapons. Increase the ","damage of the weapon by 1.","Explosion-5.png",AID::damage1,false,{true,true,true,true}};
    Augment clipIncrease1 = {"Clip Increase 1", "Passive | All Weapons. Increase the","amount of ammo in clip by 1.","Timpy.png",AID::clipIncrease1,false,{true,true,true,true}};

};

#endif //STATE_H
