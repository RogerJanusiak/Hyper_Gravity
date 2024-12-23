#include "../../includes/HEng/UpgradeMenu.h"

#include <algorithm>

void upgradeWeapon(GlobalGameState& ggs, int attr1, int attr2) {

}

std::string trimStart(const std::string& str) {
	auto it = std::find_if_not(str.begin(), str.end(), [](char c) {
		return c == '0';
	});
	return std::string(it, str.end());
}

std::string removeTrailingZeros(double i) {
	std::string result = std::to_string(i);
	result.erase(result.find_last_not_of('0') + 1, std::string::npos); // Remove trailing zeros
	if (result.back() == '.') {
		result.pop_back();  // Remove the decimal point if it's at the end
	}
	result = trimStart(result);
	return result;
}

UpgradeMenu::UpgradeMenu(GlobalGameState& ggs, Weapon& weapon) : ggs(ggs), weapon(weapon) {

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

	upgrade1Value.loadFromRenderedText(std::to_string(weapon.getClipSize()), ggs.white, ggs.buttonFont);
	upgrade2Value.loadFromRenderedText(std::to_string(weapon.getReloadSpeed()), ggs.white, ggs.buttonFont);

	upgradeMenu.setup(ggs.renderer, &buttonSound);
	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
	upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+30), "-1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);

	if(weapon.getType() == laserPistol) {
		weaponText.loadFromRenderedText("Laser Pistol", ggs.white, ggs.title);
		upgrade1Description.loadFromRenderedText("Overheat Buffer: ", ggs.white, ggs.buttonFont);
		upgrade2Description.loadFromRenderedText("Cool Off Time: ", ggs.white, ggs.buttonFont);
		upgrade3Description.loadFromRenderedText("Cool Fire Rate: ", ggs.white, ggs.buttonFont);
		upgrade3Value.loadFromRenderedText(removeTrailingZeros(weapon.getCoolFireRate()), ggs.white, ggs.buttonFont);
		upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+60), "-.1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
	} else {
		upgrade1Description.loadFromRenderedText("Clip Size: ", ggs.white, ggs.buttonFont);
		upgrade2Description.loadFromRenderedText("Reload Speed: ", ggs.white, ggs.buttonFont);
		upgrade3Description.loadFromRenderedText("Bullet Damage: ", ggs.white, ggs.buttonFont);
		upgrade4Description.loadFromRenderedText("Bullet Durability: ", ggs.white, ggs.buttonFont);

		upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+60), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
		upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+90), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);
		upgradeMenu.addButton(scaleUI(statsCenter+25), scaleUI(statsHeight+120), "+1", &ggs.white, ggs.buttonFont, -1, -1, -1, -1, &upgradeWeapon, ggs, 1);

		upgrade3Value.loadFromRenderedText(std::to_string(weapon.getDamage()), ggs.white, ggs.buttonFont);
		upgrade4Value.loadFromRenderedText(std::to_string(weapon.getDurability()), ggs.white, ggs.buttonFont);

		if(weapon.getType() == shotgun) {
			weaponText.loadFromRenderedText("Shotgun", ggs.white, ggs.title);
			upgrade5Description.loadFromRenderedText("Bullets Per Shot: ", ggs.white, ggs.buttonFont);
			upgrade5Value.loadFromRenderedText(std::to_string(weapon.getBulletsPerShot()), ggs.white, ggs.buttonFont);
		} else {
			if(weapon.getType() == rifle) {
				weaponText.loadFromRenderedText("Rifle", ggs.white, ggs.title);
			} else {
				weaponText.loadFromRenderedText("Revolver", ggs.white, ggs.title);
			}
			upgrade5Description.loadFromRenderedText("Bullet Strenth: ", ggs.white, ggs.buttonFont);
			upgrade5Value.loadFromRenderedText(std::to_string(weapon.getStrength()), ggs.white, ggs.buttonFont);
		}
	}


	challenge1Description.loadFromRenderedText("Die by landing on enemies: ", ggs.white, ggs.buttonFont);
	challenge2Description.loadFromRenderedText("Kill enemies who are on different platforms: ", ggs.white, ggs.buttonFont);

	challenge1Value.loadFromRenderedText("3/10", ggs.white, ggs.buttonFont);
	challenge2Value.loadFromRenderedText("4/10", ggs.white, ggs.buttonFont);

	statTitle.loadFromRenderedText("Stats:", ggs.white, ggs.title);
	augmentTitle.loadFromRenderedText("Augements:", ggs.white, ggs.title);
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
