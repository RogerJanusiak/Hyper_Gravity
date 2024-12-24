#include "../../includes/TEng/Menu.h"

int UI_Menu::addButton(int x, int y, const std::string &text,
    const int above, const int below, const int left, const int right, void (*action)(GlobalGameState& state, int attr1, int attr2),GlobalGameState& state, int attribute, int attribute2) {
        buttons.emplace_back(state, x, y, text, action, attribute, attribute2);
        if(above != -1) {
            buttons.back().link(RELATIVE_DIRECTION::above, &buttons[above]);
            buttons[above].link(RELATIVE_DIRECTION::below, &buttons.back());
        }
        if(below != -1) {
            buttons.back().link(RELATIVE_DIRECTION::below, &buttons[below]);
            buttons[below].link(RELATIVE_DIRECTION::above, &buttons.back());
        }
        if(left != -1) {
            buttons.back().link(RELATIVE_DIRECTION::left, &buttons[left]);
            buttons[left].link(RELATIVE_DIRECTION::right, &buttons.back());
        }
        if(right != -1) {
            buttons.back().link(RELATIVE_DIRECTION::right, &buttons[right]);
            buttons[right].link(RELATIVE_DIRECTION::left, &buttons.back());
        }
        return static_cast<int>(buttons.size())-1;
    }

void UI_Menu::render() const {
    titleTexture.render(titleX,titleY);
    for(auto& button : buttons) {
        button.render();
    }
}

void UI_Menu::loadMenu() {
    for(auto& button : buttons) {
        button.deselect();
    }
    buttons[0].select();
    currentButton = &buttons[0];
}