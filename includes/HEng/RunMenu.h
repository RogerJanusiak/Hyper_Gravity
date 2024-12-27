#ifndef RUNMENU_H
#define RUNMENU_H

#include "Weapon.h"
#include "../../includes/TEng/Menu.h"
#include "../../includes/TEng/Texture.h"
#include "../../includes/Entities/Player.h"

enum class RunMenus {
	pause,
	inventory,
	moveAugment,
	newAugment
};

class RunMenu {

public:

	explicit RunMenu(GlobalGameState& ggs, Player& player);

	void render();
	void run();
	void changeMenu(RunMenus menu);

	void readInput();

	void loadWeaponAugments(Weapon_Type weapon);

private:

	GlobalGameState& ggs;
	Player& player;

	UI_Menu inventoryMenu = UI_Menu(4);
	Texture inventoryMenuTitle;

	UI_Menu* currentMenu = nullptr;
	RunMenus currentMenuState = RunMenus::inventory;

	void initBaseInventoryMenu();

	int height = 0;
	int spacing = 0;
	int bt1 = 0, bt2 = 0, bt3 = 0, bt4 = 0;

};

#endif //RUNMENU_H
