#ifndef RUNCONTROLLER_H
#define RUNCONTROLLER_H
#include "Run.h"
#include "RunMenu.h"
#include "State.h"
#include "WaveController.h"
#include "../TEng/Menu.h"

class RunController {

public:

	explicit RunController(GlobalGameState& ggs);
	void run();

private:

	void renderDeathScreen() const;
	void readInput();

	GlobalGameState& ggs;

	std::unique_ptr<Run> currentRun;
	std::unique_ptr<WaveController> waveController;

	bool runEnded = false;

	Texture gameOverText;
	Texture continueText;
	Texture grave;

	std::unique_ptr<RunMenu> runMenu;

};

#endif //RUNCONTROLLER_H
