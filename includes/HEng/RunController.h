#ifndef RUNCONTROLLER_H
#define RUNCONTROLLER_H
#include "Run.h"
#include "State.h"
#include "WaveController.h"
#include "../TEng/Menu.h"


class RunController {

public:

	explicit RunController(GlobalGameState& ggs);

	void run();


private:

	void renderDeathScreen() const;
	void renderInventoryScreen();
	void readInput();

	GlobalGameState& ggs;

	std::unique_ptr<Run> currentRun;
	std::unique_ptr<WaveController> waveController;

	bool runEnded = false;

	Texture gameOverText;
	Texture continueText;
	Texture grave;

	Sound buttonSound;

	UI_Menu inventoryMenu = UI_Menu(4);

	UI_Menu revolverUpgradeMenu = UI_Menu(6);

	Texture inventoryMenuTitle;

	UI_Menu* currentMenu = nullptr;

};



#endif //RUNCONTROLLER_H
