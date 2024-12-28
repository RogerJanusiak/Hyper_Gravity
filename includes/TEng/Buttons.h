#ifndef BUTTONS_H
#define BUTTONS_H
#include "Button.h"

class AugButton : public Button {

public:

	AugButton(GlobalGameState& ggs, int x, int y, const Augment* augment, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0, void (*secondaryAction)(GlobalGameState& ggs, int attr1, int attr2) = nullptr, int sAttr1 = 0, int sAttr2 = 0);
	AugButton(GlobalGameState& ggs, int x, int y, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0, void (*secondaryAction)(GlobalGameState& ggs, int attr1, int attr2) = nullptr, int sAttr1 = 0, int sAttr2 = 0);

	void render() const override;

	[[nodiscard]] static int getStaticWidth() { return width; }
	[[nodiscard]] static int getStaticHeight() { return height; }
	[[nodiscard]] int getWidth() const override { return width; }
	[[nodiscard]] int getHeight() const override { return height; }

private:

	static constexpr int width = 200*1.5*UI_SCALE_FACTOR;
	static constexpr int height = 48*1.5*UI_SCALE_FACTOR;

	Texture texture;
	Texture hoverTexture;

	Texture imageTexture;
	Texture titleTexture;
	Texture descriptionLine1;
	Texture descriptionLine2;

	bool isBlank = false;

};

class InventoryButton : public Button {
public:

	InventoryButton(GlobalGameState& ggs, int x, int y, std::string path, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0);
	void render() const override;
	static int getStaticWidth() { return width; }

	[[nodiscard]] int getWidth() const override { return width; }

	int getHeight() const override { return height; }

private:

	Texture imageTexture;
	Texture texture;
	Texture hoverTexture;
	Texture activeTexture;

	static constexpr int width = 120*UI_SCALE_FACTOR;
	static constexpr int height = 120*UI_SCALE_FACTOR;

};

#endif //BUTTONS_H
