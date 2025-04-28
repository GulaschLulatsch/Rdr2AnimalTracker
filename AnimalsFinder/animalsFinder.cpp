#include "animalsfinder.h"
#include "scriptmenu.h"
#include "keyboard.h"


DWORD	vehUpdateTime;
DWORD	pedUpdateTime;

AnimalsFinder::AnimalsFinder()
{
	this->animalsNames = this->iniOptions.getAnimalsNames();
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

		if (!this->iniOptions.getShowBirds() && ENTITY::_GET_IS_BIRD(peds[i])) {
			continue;
		}

		if (this->animalsNames.find(ENTITY::_GET_PED_ANIMAL_TYPE(peds[i])) == this->animalsNames.end()) {
			continue;
		}


		Vector3 animCords = ENTITY::GET_ENTITY_COORDS(peds[i], TRUE, FALSE);
		Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, animCords.x, animCords.y, animCords.z, FALSE);

		if (this->blips.find(peds[i]) != this->blips.end()) {
			MAP::SET_BLIP_COORDS(this->blips[peds[i]], animCords.x, animCords.y, animCords.z);
			currentBlips[peds[i]] = this->blips[peds[i]];
			continue;
		}

		Blip animalBlip = MAP::BLIP_ADD_FOR_ENTITY((Hash)1664425300, peds[i]);
		this->blips[peds[i]] = animalBlip;
		currentBlips[peds[i]] = animalBlip;

		switch (PED::_GET_PED_QUALITY(peds[i])) {
		case ePedQuality::PQ_INVALID:
		case ePedQuality::PQ_LOW:
			this->removeOrModifyBlip(this->iniOptions.getShowPoorQuality(), &animalBlip, (Hash)0xA2814CC7);
			break;
		case ePedQuality::PQ_MEDIUM:
			this->removeOrModifyBlip(this->iniOptions.getShowMediumQuality(), &animalBlip, (Hash)0xF91DD38D);
			break;
		case ePedQuality::PQ_HIGH:
			this->removeOrModifyBlip(this->iniOptions.getShowExcellentQuality(), &animalBlip, (Hash)0xA5C4F725);
			break;
		}

		MAP::SET_BLIP_SPRITE(animalBlip, (Hash)-1646261997, true);
		MAP::SET_BLIP_SCALE(animalBlip, 0.8);
		MAP::_SET_BLIP_NAME(animalBlip, this->animalsNames[ENTITY::_GET_PED_ANIMAL_TYPE(peds[i])]);
	}

	for (auto& pair : this->blips) {
		if (currentBlips.find(pair.first) == currentBlips.end()) {
			MAP::REMOVE_BLIP(&this->blips[pair.first]);
			this->blips.erase(pair.first);
		}
	}

	return *this;
}

MenuBase* CreateMainMenu(MenuController* controller, map<Hash, const char*>* animalsNames, map<Hash, const char*>* selectedAnimalsNames)
{
	auto menu = new MenuBase(new MenuItemTitle("ANIMALS FINDER"));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemFlush("Flush", *animalsNames, animalsNames, selectedAnimalsNames));

	for (const auto& animal : *animalsNames) {
		menu->AddItem(new MenuItemAnimals(animal.second, animal.first, animal.second, animalsNames, selectedAnimalsNames));
	}

	return menu;
}

void AnimalsFinder::run()
{
	auto menuController = new MenuController();
	map<Hash, const char*> selectAnimalsNames;
	auto mainMenu = CreateMainMenu(menuController, &this->animalsNames, &selectAnimalsNames);

	while (true)
	{
		if (!menuController->HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			MenuInput::MenuInputBeep();
			menuController->PushMenu(mainMenu);
		}
		menuController->Update();

		this->update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	AnimalsFinder animalsFinder;
	animalsFinder.run();
}
