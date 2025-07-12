#include "animalsfinder.h"

#include "scriptmenu.h"

#include "../inc/enums.h"
#include "../inc/main.h"
#include "../inc/natives.h"
#include "../inc/types.h"

#include <memory>
#include <set>
#include <string>
#include <vector>

DWORD	vehUpdateTime;
DWORD	pedUpdateTime;

std::string const AnimalsFinder::iniFilePath{ "AnimalsFinder/AnimalsFinder.ini" };

AnimalsFinder::AnimalsFinder() :
	iniOptions{ iniFilePath },
	animalsNames{ iniOptions.getAnimalsNames() }
{}

void AnimalsFinder::removeOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash) {
	if (showQuality) {
		MAP::BLIP_ADD_MODIFIER(animalBlip, hash);
	}else{
		MAP::REMOVE_BLIP(&animalBlip);
	}
}

void AnimalsFinder::update()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	const int ARR_SIZE = 1024;

	std::vector<Ped> peds{};
	peds.resize(ARR_SIZE) ;
	int count = worldGetAllPeds(peds.data(), ARR_SIZE);
	peds.resize(count);

	static const Hash unknownAnimalTypeMagicValue{ 4141559444 };

	std::set<Ped> currentBlips;
	for (Ped ped: peds)
	{
		if (PED::IS_PED_HUMAN(ped))
			continue;

		Hash animalType{ ENTITY::_GET_PED_ANIMAL_TYPE(ped) };
		if (animalType == unknownAnimalTypeMagicValue) { // what the fuck is this magic value??
			continue;
		}

		bool isBird{ static_cast<bool>(ENTITY::_GET_IS_BIRD(ped)) };
		if (!iniOptions.getShowBirds() && isBird) {
			continue;
		}

		if (animalsNames.find(animalType) == animalsNames.end()) {
			continue;
		}

		Vector3 animCords = ENTITY::GET_ENTITY_COORDS(ped, TRUE, FALSE);
		Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, animCords.x, animCords.y, animCords.z, FALSE);

		int quality = PED::_GET_PED_QUALITY(ped);
		bool qualityMatches = qualityMatchesIni(quality);

		auto iterator = blips.find(ped);
		if (iterator != blips.end()) {
			if (qualityMatches) {
				MAP::SET_BLIP_COORDS(iterator->second, animCords.x, animCords.y, animCords.z);
				currentBlips.insert(ped);
			}
			else {
				MAP::REMOVE_BLIP(&iterator->second); // TODO also remove from blips?
			}
			continue;
		}
		if (!qualityMatches) {
			continue;
		}
		static const Hash unknownBlipHash{ 0x63351D54 };
		Blip animalBlip = MAP::BLIP_ADD_FOR_ENTITY(unknownBlipHash, ped);
		blips.insert({ ped, animalBlip });
		currentBlips.insert(ped);

		if (quality <= ePedQuality::PQ_LOW ) {
			static const Hash blipModifierArea_solidWhite{ 0xA2814CC7 };
			MAP::BLIP_ADD_MODIFIER(animalBlip, blipModifierArea_solidWhite);
		}
		else if (quality == ePedQuality::PQ_MEDIUM) {
			static const Hash blipModifierDebugBlue_solidBlue{ 0xF91DD38D };
			MAP::BLIP_ADD_MODIFIER(animalBlip, blipModifierDebugBlue_solidBlue);
		}
		else {
			static const Hash blipModifierDebugYellow_solidYellow{ 0xA5C4F725 };
			MAP::BLIP_ADD_MODIFIER(animalBlip, blipModifierDebugYellow_solidYellow);
		}

		static const Hash animalTexture{ 0x9DE00913 };
		static const Hash fishTexture{ 0xA216510E };
		static const Hash smallDotTexture{ 0x000A9056 };
		MAP::SET_BLIP_SPRITE(animalBlip, isBird ? smallDotTexture : animalTexture, true);
		MAP::SET_BLIP_SCALE(animalBlip, 0.8);

		//MAP::_SET_BLIP_NAME(animalBlip, HUD::GET_STRING_FROM_HASH_KEY(animalType));
		MAP::_SET_BLIP_NAME(animalBlip, animalsNames.at(animalType).c_str()); 
	}


	for (auto& pair : blips) {
		auto currentIterator = currentBlips.find(pair.first);
		if (currentIterator == currentBlips.end()) {
			MAP::REMOVE_BLIP(&pair.second);
			blips.erase(pair.first);
		}
	}
}

bool AnimalsFinder::qualityMatchesIni(int quality)
{
	if ((quality <= ePedQuality::PQ_LOW) && iniOptions.getShowPoorQuality()) {
		return true;
	}
	if ((quality == ePedQuality::PQ_MEDIUM) && iniOptions.getShowMediumQuality()) {
		return true;
	}
	if ((quality >= ePedQuality::PQ_HIGH) && iniOptions.getShowExcellentQuality()) {
		return true;
	}
	return false;
}

std::unique_ptr<MenuBase> CreateMainMenu(MenuController& controller, map<Hash, std::string>& animalsNames, map<Hash, std::string>& selectedAnimals)
{
	auto menu = std::make_unique<MenuBase>(new MenuItemTitle("ANIMALS FINDER"));
	controller.RegisterMenu(menu.get());

	menu->AddItem(new MenuItemFlush("Flush", animalsNames, &animalsNames, &selectedAnimals));

	std::vector<const std::pair<const Hash, std::string>*> sortedNames; // use vector of pointers to avoid unnecesssary copy
	sortedNames.reserve(animalsNames.size());
	for (const auto& entry : animalsNames) {
		sortedNames.push_back(&entry); // not sorted yet
	}

	std::sort(sortedNames.begin(), sortedNames.end(), [](const auto* a, const auto* b) {
		return a->second < b->second;
	});

	for (const auto& animal : sortedNames) {
		menu->AddItem(new MenuItemAnimals(animal->second, animal->first, &animalsNames, &selectedAnimals));
	}

	return menu;
}

void AnimalsFinder::run()
{
	MenuController menuController{};
	map<Hash, std::string> selectedAnimals{};
	auto mainMenu = CreateMainMenu(menuController, animalsNames, selectedAnimals);

	while (true)
	{
		if (!menuController.HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			MenuInput::MenuInputBeep();
			menuController.PushMenu(mainMenu.get());
		}
		menuController.Update();

		update();
		WAIT(0);
	}
}

void ScriptMain()
{
	AnimalsFinder animalsFinder;
	animalsFinder.run();
}
