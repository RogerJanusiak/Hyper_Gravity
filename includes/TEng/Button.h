#ifndef BUTTON_H
#define BUTTON_H

#include "../Utils/GlobalConstants.h"
#include "Sound.h"
#include "../HEng/State.h"
#include "Texture.h"

#include <SDL_render.h>
#include <vector>

enum class MENU_CONTROL {
	connect,
	disconnect,
	up,
	down,
	select,
	back,
	left,
	right,
  };

enum class RELATIVE_DIRECTION {
	above,
	below,
	left,
	right,
  };

class Button {

public:

	Button(GlobalGameState& ggs, int x, int y, const std::string& text, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0);

	void render() const;
	[[nodiscard]] int getWidth() const { return width; }

	[[nodiscard]] Button* move(MENU_CONTROL action) const;
	void linkButtons(Button* above, Button* below,Button* left, Button* right) { buttonAbove = above; buttonBelow = below; buttonLeft = left; buttonRight = right; }
	void link(RELATIVE_DIRECTION direction, Button* button);

	void select() {selected = true;}
	void deselect() {selected = false;}

	void activate() { active = true; }
	void deactivate() { active = false; }
	[[nodiscard]] bool isActive() const { return active; }

	[[nodiscard]] bool isSelected() const {return selected;}

	void setType(int type , SDL_Renderer* _renderer);

	static constexpr int width = 160*1.5*UI_SCALE_FACTOR;
	static constexpr int h = 32*1.5*UI_SCALE_FACTOR;

	void click();

	[[nodiscard]] bool mouseEvent(int mouseX, int mouseY) const;

protected:

	int x, y;


	int attribute;
	int attribute2;

	int type;

	GlobalGameState& state;
	void (*action)(GlobalGameState& ggs, int attr1, int attr2);

	Button* buttonAbove = nullptr;
	Button* buttonBelow = nullptr;
	Button* buttonRight = nullptr;
	Button* buttonLeft = nullptr;

	Texture texture;
	Texture hoverTexture;
	Texture activeTexture;

	Texture imageTexture;
	Texture textTexture;

	Sound buttonClick;

	bool selected = false;
	bool secondaryImage = false;

	bool active = false;
};

#endif //BUTTON_H