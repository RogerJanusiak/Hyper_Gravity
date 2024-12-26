#include "../../includes/TEng/Menu.h"

int UI_Menu::addButton(std::unique_ptr<Button> button, const int above, const int below, const int left, const int right) {
        buttons.emplace_back(std::move(button));
        if(above != -1) {
            buttons.back()->link(RELATIVE_DIRECTION::above, buttons[above].get());
            buttons[above]->link(RELATIVE_DIRECTION::below, buttons.back().get());
        }
        if(below != -1) {
            buttons.back()->link(RELATIVE_DIRECTION::below, buttons[below].get());
            buttons[below]->link(RELATIVE_DIRECTION::above, buttons.back().get());
        }
        if(left != -1) {
            buttons.back()->link(RELATIVE_DIRECTION::left, buttons[left].get());
            buttons[left]->link(RELATIVE_DIRECTION::right, buttons.back().get());
        }
        if(right != -1) {
            buttons.back()->link(RELATIVE_DIRECTION::right, buttons[right].get());
            buttons[right]->link(RELATIVE_DIRECTION::left, buttons.back().get());
        }
        return static_cast<int>(buttons.size())-1;
    }

void UI_Menu::render() const {
    titleTexture.render(titleX,titleY);
    for(auto& button : buttons) {
        button->render();
    }
}

void UI_Menu::loadMenu() {
    for(auto& button : buttons) {
        button->deselect();
    }
    buttons[0]->select();
    currentButton = buttons[0].get();
}