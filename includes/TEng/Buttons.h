#ifndef BUTTONS_H
#define BUTTONS_H
#include "Button.h"

class AugButton : public Button {

public:

	AugButton(GlobalGameState& ggs, int x, int y, const Augment& augment, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1 = 0, int attr2 = 0);

private:

};

#endif //BUTTONS_H
