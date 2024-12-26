#include "../../includes/TEng/Buttons.h"

AugButton::AugButton(GlobalGameState& ggs, int x, int y, const Augment& augment, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1, int attr2) :
Button(ggs, x, y, augment.name, action, attr1, attr2) {


}
