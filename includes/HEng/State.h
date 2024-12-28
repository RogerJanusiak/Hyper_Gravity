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

enum AID {
    damage1,
    damage2,
    damage3,
    clipIncrease1,
    clipIncrease2,
    clipIncrease3,
    reload1,
    reload2,
    reload3,
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

    int weaponChanged = 0;
    int slotChanged = 0;

    int weaponMovedTo = 0;
    int slotMovedTo = 0;

    bool augmentRemoved = false;
    bool startMoveAugment = false;
    bool moveAugment = false;

    Augment* newAugment = nullptr;

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
    Augment damage2 = {"Damage 2", "Passive | All Weapons. Increase the ","damage of the weapon by 2.","Explosion-5.png",AID::damage2,false,{true,true,true,true}};
    Augment damage3 = {"Damage 3", "Passive | All Weapons. Increase the ","damage of the weapon by 3.","Explosion-5.png",AID::damage3,false,{true,true,true,true}};


    Augment clipIncrease1 = {"Clip Increase 1", "Passive | All Weapons. Increase the","amount of ammo in clip by 1.","Explosion-5.png",AID::clipIncrease1,false,{true,true,true,true}};
    Augment clipIncrease2 = {"Clip Increase 2", "Passive | All Weapons. Increase the","amount of ammo in clip by 2.","Explosion-5.png",AID::clipIncrease2,false,{true,true,true,true}};
    Augment clipIncrease3 = {"Clip Increase 3", "Passive | All Weapons. Increase the","amount of ammo in clip by 3.","Explosion-5.png",AID::clipIncrease3,false,{true,true,true,true}};

    Augment reload1 = {"Reload Speed 1", "Passive | All Weapons. Decrease the","time it takes to reload be 25%.","Explosion-5.png",AID::reload1,false,{true,true,true,true}};
    Augment reload2 = {"Reload Speed 2", "Passive | All Weapons. Decrease the","time it takes to reload be 50%.","Explosion-5.png",AID::reload2,false,{true,true,true,true}};
    Augment reload3 = {"Reload Speed 3", "Passive | All Weapons. Decrease the","time it takes to reload be 75%.","Explosion-5.png",AID::reload3,false,{true,true,true,true}};


};

#endif //STATE_H
