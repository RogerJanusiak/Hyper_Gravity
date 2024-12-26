#ifndef RUNMENU_H
#define RUNMENU_H

#include "../../includes/TEng/Menu.h"
#include "../../includes/TEng/Texture.h"

enum class RunMenus {
	pause,
	inventory,
	moveAugment,
	newAugment
};

class RunMenu {

public:

	explicit RunMenu(GlobalGameState& ggs);

	void render();
	void changeMenu(RunMenus menu);

	void readInput();

private:

	GlobalGameState& ggs;

	UI_Menu inventoryMenu = UI_Menu(4);
	Texture inventoryMenuTitle;

	UI_Menu* currentMenu = nullptr;
	RunMenus currentMenuState;

};

#endif //RUNMENU_H
