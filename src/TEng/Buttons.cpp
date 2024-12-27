#include "../../includes/TEng/Buttons.h"

AugButton::AugButton(GlobalGameState& ggs, int x, int y, const Augment& augment, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1, int attr2) :
Button(ggs, x, y, augment.name, action, attr1, attr2) {

	texture.setup(width,height,ggs.renderer);
	texture.loadFromFile("ui/aug-button.png");

	hoverTexture.setup(width, height, ggs.renderer);
	hoverTexture.loadFromFile("ui/aug-button-selected.png");

	imageTexture.setup(scaleUI(32)*1.5,scaleUI(32)*1.5,ggs.renderer);
	imageTexture.loadFromFile(augment.image);

	titleTexture.setup(ggs.renderer);
	titleTexture.loadFromRenderedText(augment.name,ggs.white, ggs.augTitle);

	descriptionLine1.setup(ggs.renderer);
	descriptionLine1.loadFromRenderedText(augment.descriptionLine1,ggs.white, ggs.augDescription);

	descriptionLine2.setup(ggs.renderer);
	descriptionLine2.loadFromRenderedText(augment.descriptionLine2,ggs.white, ggs.augDescription);

}

AugButton::AugButton(GlobalGameState& ggs, int x, int y, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1, int attr2) :
Button(ggs, x, y, "No Augment Equiped in Slot", action, attr1, attr2) {

	texture.setup(width,height,ggs.renderer);
	texture.loadFromFile("ui/aug-button.png");

	hoverTexture.setup(width, height, ggs.renderer);
	hoverTexture.loadFromFile("ui/aug-button-selected.png");

	isBlank = true;

}


void AugButton::render() const {
	texture.render(x,y);
	if(active) {
		activeTexture.render(x,y);
	}
	if(selected) {
		hoverTexture.render(x,y);
	}

	if(!isBlank) {
		imageTexture.render(x+scaleButton(8),y+scaleButton(8));
		titleTexture.render(x+scaleButton(8+8)+imageTexture.getWidth(),y+scaleButton(6));
		descriptionLine1.render(x+scaleButton(8+8)+imageTexture.getWidth(),y+scaleButton(6)+titleTexture.getHeight());
		descriptionLine2.render(x+scaleButton(8+8)+imageTexture.getWidth(),y+scaleButton(6)+titleTexture.getHeight()+descriptionLine1.getHeight());
	} else {
		textTexture.render(x+(width-textTexture.getWidth())/2,y+(height-textTexture.getHeight())/2);
	}

}

InventoryButton::InventoryButton(GlobalGameState& ggs, int x, int y, std::string path, void (*action)(GlobalGameState& ggs, int attr1, int attr2), int attr1, int attr2) :
Button(ggs, x, y, "a", action, attr1, attr2) {

	imageTexture.setup(width,height,ggs.renderer);
	imageTexture.loadFromFile(path);

	texture.setup(width,height,ggs.renderer);
	texture.loadFromFile("ui/upgrade-button.png");

	hoverTexture.setup(width,height,ggs.renderer);
	hoverTexture.loadFromFile("ui/upgrade-selected.png");

	activeTexture.setup(width,height,ggs.renderer);
	activeTexture.loadFromFile("ui/upgrade-active.png");


}

void InventoryButton::render() const {
	texture.render(x,y);
	if(active) {
		activeTexture.render(x,y);
	}
	if(selected) {
		hoverTexture.render(x,y);
	}

	imageTexture.render(x,y);
}
