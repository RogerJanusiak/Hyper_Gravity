#ifndef UPGRADEMENU_H
#define UPGRADEMENU_H

#include "../TEng/Menu.h"
#include "../TEng/Texture.h"

class UpgradeMenu {
public:

	UpgradeMenu(GlobalGameState& ggs);

	void render();

	[[nodiscard]] UI_Menu* getMenu() { return &upgradeMenu; }

private:

	GlobalGameState& ggs;

	Sound buttonSound;

	Texture weaponText;

	Texture statTitle;
	Texture augmentTitle;

	Texture upgrade1Description;
	Texture upgrade2Description;
	Texture upgrade3Description;
	Texture upgrade4Description;
	Texture upgrade5Description;

	Texture upgrade1Value;
	Texture upgrade2Value;
	Texture upgrade3Value;
	Texture upgrade4Value;
	Texture upgrade5Value;

	Texture challenge1Description;
	Texture challenge2Description;

	Texture challenge1Value;
	Texture challenge2Value;

	UI_Menu upgradeMenu = UI_Menu(5);

	int statsHeight = 250;
	int statsCenter = 275;

};

#endif //UPGRADEMENU_H
