#ifndef MENU_H
#define MENU_H

#include "Sound.h"
#include "../HEng/State.h"
#include "Texture.h"
#include "Button.h"

#include <SDL_render.h>
#include <vector>

class UI_Menu {
public:

  explicit UI_Menu(const int numberOfButtons) { buttons.reserve(numberOfButtons); };

  int addButton(int x, int y, const std::string& text,
    int above, int below, int left, int right, void (*action)(GlobalGameState& state, int attr1, int attr2),GlobalGameState& state, int attribute = 0,int attribute2 = 0);

  void reset() { buttons.clear(); };

  void loadMenu();

  void setup(SDL_Renderer* _renderer, Sound* _buttonSound) { renderer = _renderer;buttonSound = _buttonSound; }

  void addTitle(const int x, const int y, const Texture & texture) { titleX = x, titleY = y, titleTexture = texture; }

  void render() const;

  std::vector<Button>* getButtons() { return &buttons; }

  //TODO: Make functions surrounding this!
  Button* currentButton = nullptr;

private:

  TTF_Font* font = nullptr;
  SDL_Color* color = nullptr;

  Sound* buttonSound;

  SDL_Renderer* renderer = nullptr;
  Texture titleTexture;
  int titleX = 0,titleY = 0;
  std::vector<Button> buttons;

};

#endif //MENU_H