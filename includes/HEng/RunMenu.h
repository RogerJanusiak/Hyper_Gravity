#ifndef RUNMENU_H
#define RUNMENU_H

#include "Weapon.h"
#include "../../includes/TEng/Menu.h"
#include "../../includes/TEng/Buttons.h"
#include "../../includes/TEng/Texture.h"
#include "../../includes/Entities/Player.h"

// TODO: Move Pause menu
enum class RunMenus {
	pause,
	inventory
};

enum inventoryScreen {
	move,
	newAug,
	view,
};

class RunMenu {

public:

	explicit RunMenu(GlobalGameState& ggs, Player& player);

	void render() const;
	void run();
	void changeMenu(RunMenus menu);

	void readInput();

	bool newAugment = false;

private:

	GlobalGameState& ggs;
	Player& player;

	UI_Menu inventoryMenu = UI_Menu(6);

	Texture inventoryMenuTitle;
	Texture moveTitle;
	Texture newTitle;

	Texture revolverTexture;
	Texture shotgunTexture;
	Texture rifleTexture;
	Texture laserPistolTexture;

	Texture aButton;
	Texture xButton;

	Texture leftClickText;
	Texture rightClickText;

	Texture moveAugText;
	Texture removeAugText;
	Texture selectSlotText;

	UI_Menu* currentMenu = nullptr;
	RunMenus currentMenuState = RunMenus::inventory;
	inventoryScreen currentScreen;

	void initBaseInventoryMenu();
	void initMoveInventoryMenu();
	void initNewInventoryMenu();

	int verticalSpacing = scaleUI(20);
	int column1 = (WINDOW_WIDTH-AugButton::getStaticWidth()*2-scaleUI(20))/2;
	int column2 = column1+scaleUI(20)+AugButton::getStaticWidth();

	int height = 0;
	int row1;
	int row2;
	int row3;
	int row4;

};

#endif //RUNMENU_H
