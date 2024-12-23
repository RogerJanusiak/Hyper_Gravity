#include "../../includes/HEng/UpgradeMenu.h"

void upgradeWeapon(GlobalGameState& ggs, int attr1, int attr2) {

}

UpgradeMenu::UpgradeMenu(GlobalGameState& ggs) : ggs(ggs) {

	buttonSound.init("resources/sounds/buttonClick.wav", 0,-1);

	weaponText.setup(ggs.renderer);

	upgrade1Description.setup(ggs.renderer);
	upgrade2Description.setup(ggs.renderer);
	upgrade3Description.setup(ggs.renderer);
	upgrade4Description.setup(ggs.renderer);
	upgrade5Description.setup(ggs.renderer);

	upgrade1Value.setup(ggs.renderer);
	upgrade2Value.setup(ggs.renderer);
	upgrade3Value.setup(ggs.renderer);
	upgrade4Value.setup(ggs.renderer);
	upgrade5Value.setup(ggs.renderer);

	challenge1Description.setup(ggs.renderer);
	challenge2Description.setup(ggs.renderer);

	challenge1Value.setup(ggs.renderer);
	challenge2Value.setup(ggs.renderer);

	statTitle.setup(ggs.renderer);
	augmentTitle.setup(ggs.renderer);

	weaponText.loadFromRenderedText("Revolver", ggs.white, ggs.title);

	upgrade1Description.loadFromRenderedText("Clip Size: ", ggs.white, ggs.buttonFont);
	upgrade2Description.loadFromRenderedText("Reload Speed: ", ggs.white, ggs.buttonFont);
	upgrade3Description.loadFromRenderedText("Bullet Damage: ", ggs.white, ggs.buttonFont);
	upgrade4Description.loadFromRenderedText("Bullet Durability: ", ggs.white, ggs.buttonFont);
	upgrade5Description.loadFromRenderedText("Bullet Strenth: ", ggs.white, ggs.buttonFont);

	upgrade1Value.loadFromRenderedText("4", ggs.white, ggs.buttonFont);
	upgrade2Value.loadFromRenderedText("3", ggs.white, ggs.buttonFont);
	upgrade3Value.loadFromRenderedText("1", ggs.white, ggs.buttonFont);
	upgrade4Value.loadFromRenderedText("1", ggs.white, ggs.buttonFont);
	upgrade5Value.loadFromRenderedText("1", ggs.white, ggs.buttonFont);

	challenge1Description.loadFromRenderedText("Die by landing on enemies: ", ggs.white, ggs.buttonFont);
	challenge2Description.loadFromRenderedText("Kill enemies who are on different platforms: ", ggs.white, ggs.buttonFont);

	challenge1Value.loadFromRenderedText("3/10", ggs.white, ggs.buttonFont);
	challenge2Value.loadFromRenderedText("4/10", ggs.white, ggs.buttonFont);

	statTitle.loadFromRenderedText("Stats:", ggs.white, ggs.title);
	augmentTitle.loadFromRenderedText("Augements:", ggs.white, ggs.title);

	upgradeMenu.setup(ggs.renderer, &buttonSound);

	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+30), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+60), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+90), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+120), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
}

void UpgradeMenu::render() {

	upgrade1Description.render(scaleUI(statsCenter)-upgrade1Description.getWidth(), scaleUI(statsHeight));
	upgrade2Description.render(scaleUI(statsCenter)-upgrade2Description.getWidth(), scaleUI(statsHeight + 30));
	upgrade3Description.render(scaleUI(statsCenter)-upgrade3Description.getWidth(), scaleUI(statsHeight + 60));
	upgrade4Description.render(scaleUI(statsCenter)-upgrade4Description.getWidth(), scaleUI(statsHeight + 90));
	upgrade5Description.render(scaleUI(statsCenter)-upgrade5Description.getWidth(), scaleUI(statsHeight + 120));

	upgrade1Value.render(scaleUI(statsCenter+5), scaleUI(statsHeight));
	upgrade2Value.render(scaleUI(statsCenter+5), scaleUI(statsHeight + 30));
	upgrade3Value.render(scaleUI(statsCenter+5), scaleUI(statsHeight + 60));
	upgrade4Value.render(scaleUI(statsCenter+5), scaleUI(statsHeight + 90));
	upgrade5Value.render(scaleUI(statsCenter+5), scaleUI(statsHeight + 120));

	statTitle.render(scaleUI(statsCenter)-statTitle.getWidth(),scaleUI(statsHeight-70));
	augmentTitle.render(scaleUI(450),scaleUI(statsHeight-70));

	upgradeMenu.render();

}
