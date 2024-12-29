#include "../../includes/TEng/ConfirmPopup.h"

ConfirmPopup::ConfirmPopup(GlobalGameState& ggs, std::string& prompt, void (*action)(GlobalGameState& ggs, int attr1, int attr2)) : ggs(ggs), prompt(prompt), action(action) {

}

void ConfirmPopup::render() const {

	SDL_SetRenderDrawColor(ggs.renderer, 25,25,25,180);
	SDL_SetRenderDrawBlendMode(ggs.renderer,SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(ggs.renderer, &screen);

	SDL_SetRenderDrawColor(ggs.renderer, 50,50,50,255);
	SDL_RenderFillRect(ggs.renderer, &popup);

}

