#ifndef BUTTONS_H
#define BUTTONS_H
#include "Button.h"

class AugButton : public Button {

public:

	AugButton(GlobalGameState& ggs, int x, int y, const Augment& augment, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0);
	void render() const override;
	static int getWidth() { return width; }

private:

	static constexpr int width = 200*1.5*UI_SCALE_FACTOR;
	static constexpr int height = 48*1.5*UI_SCALE_FACTOR;

	Texture texture;

	Texture imageTexture;
	Texture titleTexture;
	Texture descriptionLine1;
	Texture descriptionLine2;

};

class InventoryButton : public Button {
public:

	InventoryButton(GlobalGameState& ggs, int x, int y, std::string path, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0);
	void render() const override;
	static int getWidth() { return width; }

	static constexpr int width = 120*UI_SCALE_FACTOR;
	// TODO: The height is not reconized by the mouse event function
	static constexpr int height = 120*UI_SCALE_FACTOR;

private:

	Texture imageTexture;
	Texture texture;
	Texture hoverTexture;
	Texture activeTexture;

};

#endif //BUTTONS_H
