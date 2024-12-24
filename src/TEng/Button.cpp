#include "../../includes/TEng/Button.h"

/*void Button::setType(int type, SDL_Renderer* _renderer) {
    renderer = _renderer;
    if(type == 0) {
        width = width;
        h = height;

        texture.setup(width,h,renderer);
        texture.loadFromFile("button.png");

        hoverTexture.setup(width,h,renderer);
        hoverTexture.loadFromFile("button1.png");
    } else if(type == 1) {
        width = smallWidth;
        h = smallHeight;

        texture.setup(width,h,renderer);
        texture.loadFromFile("ui/small.png");

        hoverTexture.setup(width,h,renderer);
        hoverTexture.loadFromFile("ui/small-selected.png");

        activeTexture.setup(width,h,renderer);
        activeTexture.loadFromFile("ui/small-active.png");
    } else if(type == 2) {
        width = weaponWidth;
        h = weaponWidth;

        texture.setup(width,h,renderer);
        texture.loadFromFile("ui/upgrade-button.png");

        hoverTexture.setup(width,h,renderer);
        hoverTexture.loadFromFile("ui/upgrade-selected.png");

        activeTexture.setup(width,h,renderer);
        activeTexture.loadFromFile("ui/upgrade-active.png");
    }
}*/

Button::Button(GlobalGameState& ggs, int x, int y, const std::string& text, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1, int attr2) :
x(x), y(y), action(action), state(ggs), attribute(attr1), attribute2(attr2) {

    textTexture.setup(0,0,ggs.renderer);
    textTexture.loadFromRenderedText(text, ggs.white, ggs.buttonFont);

    texture.setup(width,h,ggs.renderer);
    texture.loadFromFile("button.png");

    hoverTexture.setup(width,h,ggs.renderer);
    hoverTexture.loadFromFile("button1.png");
}

bool Button::mouseEvent(const int mouseX, const int mouseY) const {
    if(mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+h) {
        return true;
    }
    return false;
}

void Button::render() const {
    texture.render(x,y);
    if(active) {
        activeTexture.render(x,y);
    }
    if(selected) {
        hoverTexture.render(x,y);
    }
    textTexture.render(x+(width-textTexture.getWidth())/2,y+(h-textTexture.getHeight())/2);
}

Button* Button::move(const MENU_CONTROL action) const {
    switch(action) {
    case MENU_CONTROL::up: {
        if (buttonAbove != nullptr) {
            return buttonAbove;
        }
        return nullptr;
    }
    case MENU_CONTROL::down: {
        if (buttonBelow != nullptr) {
            return buttonBelow;
        }
        return nullptr;
    }
    case MENU_CONTROL::left: {
        if (buttonLeft != nullptr) {
            return buttonLeft;
        }
        return nullptr;
    }
    case MENU_CONTROL::right: {
        if (buttonRight != nullptr) {
            return buttonRight;
        }
        return nullptr;
    }
    default:
        return nullptr;
    }
}

void Button::link(const RELATIVE_DIRECTION direction, Button* button) {
    switch(direction) {
        case RELATIVE_DIRECTION::above:
            buttonAbove = button;
            break;
        case RELATIVE_DIRECTION::below:
            buttonBelow = button;
            break;
        case RELATIVE_DIRECTION::left:
            buttonLeft = button;
            break;
        case RELATIVE_DIRECTION::right:
            buttonRight = button;
            break;
    }
}

void Button::click() {
    action(state, attribute, attribute2);
    buttonClick.play();
}