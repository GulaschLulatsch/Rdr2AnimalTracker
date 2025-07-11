#include "animalsfinder.h"
#include "scriptmenu.h"
#include "keyboard.h"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

DWORD	vehUpdateTime;
DWORD	pedUpdateTime;


std::experimental::filesystem::path const AnimalsFinder::iniFilePath{ "AnimalsFinder/AnimalsFinder.ini" };

AnimalsFinder::AnimalsFinder() :
	iniOptions{ iniFilePath }
{
	animalsNames = iniOptions.getAnimalsNames();
}

AnimalsFinder& AnimalsFinder::removeOrModifyBlip(bool showQuality, Blip* animalBlip, Hash hash) {
	if (showQuality) {
		MAP::BLIP_ADD_MODIFIER(*animalBlip, hash);
		return *this;
	}

	MAP::REMOVE_BLIP(animalBlip);
	return *this;
}

AnimalsFinder& AnimalsFinder::update()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return *this;

	const int ARR_SIZE = 1024;

	Ped peds[ARR_SIZE];
	int count = worldGetAllPeds(peds, ARR_SIZE);

	std::map<Ped, Blip> currentBlips;
	for (int i = 0; i < count; i++)
	{
		if (PED::IS_PED_HUMAN(peds[i]))
			continue;

		if (ENTITY::_GET_PED_ANIMAL_TYPE(peds[i]) == 4141559444) {
			continue;
		}

		if (!iniOptions.getShowBirds() && ENTITY::_GET_IS_BIRD(peds[i])) {
			continue;
		}

		if (animalsNames.find(ENTITY::_GET_PED_ANIMAL_TYPE(peds[i])) == animalsNames.end()) {
			continue;
		}


		Vector3 animCords = ENTITY::GET_ENTITY_COORDS(peds[i], TRUE, FALSE);
		Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, animCords.x, animCords.y, animCords.z, FALSE);

		if (blips.find(peds[i]) != blips.end()) {
			MAP::SET_BLIP_COORDS(blips[peds[i]], animCords.x, animCords.y, animCords.z);
			currentBlips[peds[i]] = blips[peds[i]];
			continue;
		}

		Blip animalBlip = MAP::BLIP_ADD_FOR_ENTITY((Hash)1664425300, peds[i]);
		blips[peds[i]] = animalBlip;
		currentBlips[peds[i]] = animalBlip;

		switch (PED::_GET_PED_QUALITY(peds[i])) {
		case ePedQuality::PQ_INVALID:
		case ePedQuality::PQ_LOW:
			removeOrModifyBlip(iniOptions.getShowPoorQuality(), &animalBlip, (Hash)0xA2814CC7);
			break;
		case ePedQuality::PQ_MEDIUM:
			removeOrModifyBlip(iniOptions.getShowMediumQuality(), &animalBlip, (Hash)0xF91DD38D);
			break;
		case ePedQuality::PQ_HIGH:
			removeOrModifyBlip(iniOptions.getShowExcellentQuality(), &animalBlip, (Hash)0xA5C4F725);
			break;
		}

		MAP::SET_BLIP_SPRITE(animalBlip, (Hash)-1646261997, true);
		MAP::SET_BLIP_SCALE(animalBlip, 0.8);
		MAP::_SET_BLIP_NAME(animalBlip, animalsNames[ENTITY::_GET_PED_ANIMAL_TYPE(peds[i])].c_str());
	}

	for (auto& pair : blips) {
		if (currentBlips.find(pair.first) == currentBlips.end()) {
			MAP::REMOVE_BLIP(&blips[pair.first]);
			blips.erase(pair.first);
		}
	}

	return *this;
}

MenuBase* CreateMainMenu(MenuController const& controller, map<Hash, std::string>* animalsNames, map<Hash, std::string>* selectedAnimalsNames)
{
	auto menu = new MenuBase(new MenuItemTitle("ANIMALS FINDER"));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemFlush("Flush", *animalsNames, animalsNames, selectedAnimalsNames));

	std::vector<const std::pair<const Hash, std::string>*> sortedNames; // use vector of pointers to avoid unnecesssary copy
	sortedNames.reserve(animalsNames->size());
	for (const auto& entry : *animalsNames) {
		sortedNames.push_back(&entry); // not sorted yet
	}

	std::sort(sortedNames.begin(), sortedNames.end(), [](const auto* a, const auto* b) {
		return a->second < b->second;
	});

	for (const auto& animal : sortedNames) {
		menu->AddItem(new MenuItemAnimals(animal->second, animal->first, animal->second, animalsNames, selectedAnimalsNames));
	}

	return menu;
}

void AnimalsFinder::run()
{
	MenuController menuController{};
	map<Hash, std::string> selectAnimalsNames;
	auto mainMenu = CreateMainMenu(menuController, &animalsNames, &selectAnimalsNames);

	while (true)
	{
		if (!menuController.HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			MenuInput::MenuInputBeep();
			menuController.PushMenu(mainMenu);
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
