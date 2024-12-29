#ifndef CONFIRMPOPUP_H
#define CONFIRMPOPUP_H
#include <string>
#include "../../includes/HEng/State.h"
#include "../Utils/GlobalConstants.h"

class ConfirmPopup {
public:

	ConfirmPopup(GlobalGameState& ggs, std::string& prompt, void (*action)(GlobalGameState& ggs, int attr1, int attr2));

	void render() const;

private:

	GlobalGameState& ggs;
	std::string& prompt;
	void (*action)(GlobalGameState& ggs, int attr1, int attr2);

	SDL_Rect screen = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

	int popupWidth = scaleUI(280);
	int popupHeight = scaleUI(220);

	SDL_Rect popup = {(WINDOW_WIDTH-popupWidth)/2,(WINDOW_HEIGHT-popupHeight)/2, popupWidth, popupHeight};

};

#endif //CONFIRMPOPUP_H
