#include "../../includes/TEng/Button.h"

Button::Button(GlobalGameState& ggs, int x, int y, const std::string& text, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1, int attr2, void (*secondaryAction)(GlobalGameState& ggs, int attr1, int attr2), int secondaryAttr1 , int secodaryAttr2) :
x(x), y(y), attribute(attr1), attribute2(attr2), state(ggs), action(action), secondaryAction(secondaryAction), secondaryAttr1(secondaryAttr1), secondaryAttr2(secodaryAttr2) {

    textTexture.setup(0,0,ggs.renderer);
    textTexture.loadFromRenderedText(text, ggs.white, ggs.buttonFont);

    texture.setup(width,height,ggs.renderer);
    texture.loadFromFile("button.png");

    hoverTexture.setup(width,height,ggs.renderer);
    hoverTexture.loadFromFile("button1.png");

    buttonClick.init("resources/sounds/buttonClick.wav",0,-1);
}

bool Button::mouseEvent(const int mouseX, const int mouseY) const {
    if(mouseX >= x && mouseX <= x+getWidth() && mouseY >= y && mouseY <= y+getHeight()) {
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
    textTexture.render(x+(width-textTexture.getWidth())/2,y+(height-textTexture.getHeight())/2);
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

void Button::secondaryClick() {
    if(secondaryAction != nullptr) {
        secondaryAction(state, secondaryAttr1, secondaryAttr2);
        buttonClick.play();
    }
}