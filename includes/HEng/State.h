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
    damage,
    clipIncrease,
    reload,
    strength,
    durability,
  };

struct Augment {

    std::string name;
    std::string descriptionLine1;
    std::string descriptionLine2;
    std::string image;
    AID id;

    bool active;
    bool compadableWith[4];

    int level = 0;

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
    bool addedAugment = false;
    bool fromNew = false;

    Augment* augmentToMove = nullptr;
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
    Augment damage1 = {"Damage 1", "Passive | All Weapons. Increase the ","damage of the weapon by 1.","Explosion-5.png",AID::damage,false,{true,true,true,true},1};
    Augment damage2 = {"Damage 2", "Passive | All Weapons. Increase the ","damage of the weapon by 2.","Explosion-5.png",AID::damage,false,{true,true,true,true},2};
    Augment damage3 = {"Damage 3", "Passive | All Weapons. Increase the ","damage of the weapon by 3.","Explosion-5.png",AID::damage,false,{true,true,true,true},3};


    Augment clipIncrease1 = {"Clip Increase 1", "Passive | All Weapons. Increase the","amount of ammo in clip by 1.","Explosion-5.png",AID::clipIncrease,false,{true,true,true,true},1};
    Augment clipIncrease2 = {"Clip Increase 2", "Passive | All Weapons. Increase the","amount of ammo in clip by 2.","Explosion-5.png",AID::clipIncrease,false,{true,true,true,true},2};
    Augment clipIncrease3 = {"Clip Increase 3", "Passive | All Weapons. Increase the","amount of ammo in clip by 3.","Explosion-5.png",AID::clipIncrease,false,{true,true,true,true},3};

    Augment reload1 = {"Reload Speed 1", "Passive | All Weapons. Decrease the","time it takes to reload be 25%.","Explosion-5.png",AID::reload,false,{true,true,true,true},1};
    Augment reload2 = {"Reload Speed 2", "Passive | All Weapons. Decrease the","time it takes to reload be 50%.","Explosion-5.png",AID::reload,false,{true,true,true,true},2};
    Augment reload3 = {"Reload Speed 3", "Passive | All Weapons. Decrease the","time it takes to reload be 75%.","Explosion-5.png",AID::reload,false,{true,true,true,true},3};

    Augment strength1 = {"Strength 1", "Passive | All Weapons. Bullets continue","through 1 addition enemy.","Explosion-5.png",AID::strength,false,{true,true,true,true},1};
    Augment strength2 = {"Strength 2", "Passive | All Weapons. Bullets continue","through 2 addition enemies.","Explosion-5.png",AID::strength,false,{true,true,true,true},2};
    Augment strength3 = {"Strength 3", "Passive | All Weapons. Bullets continue","through 3 addition enemies.","Explosion-5.png",AID::strength,false,{true,true,true,true},3};

    Augment durability1 = {"Bullet Durability 1", "Passive | All Weapons. Bullets can","slide along 1 addition platform.","Explosion-5.png",AID::durability,false,{true,true,true,true},1};
    Augment durability2 = {"Bullet Durability 2", "Passive | All Weapons. Bullets can","slide along 3 addition platform2.","Explosion-5.png",AID::durability,false,{true,true,true,true},2};
    Augment durability3 = {"Bullet Durability 3", "Passive | All Weapons. Bullets can","slide along 4 addition platform2.","Explosion-5.png",AID::durability,false,{true,true,true,true},3};


};

#endif //STATE_H
