#ifndef RUNCONTROLLER_H
#define RUNCONTROLLER_H
#include "Run.h"
#include "State.h"
#include "WaveController.h"


class RunController {

public:

	explicit RunController(GlobalGameState& ggs);

	void run();
	void render();

private:

	void readInput();

	GlobalGameState& ggs;

	std::unique_ptr<Run> currentRun;
	std::unique_ptr<WaveController> waveController;

	bool runEnded = false;

	Texture gameOverText;
	Texture continueText;
	Texture grave;

};



#endif //RUNCONTROLLER_H
