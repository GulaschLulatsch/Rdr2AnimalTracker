#include "AnimalFinder.h"

#include "AnimalInfo.h"
#include "MenuBase.h"
#include "MenuController.h"
#include "MenuInput.h"
#include "MenuItemAnimal.h"
#include "MenuItemTitle.h"

#include <RDR2ScriptHook/enums.h>
#include <RDR2ScriptHook/main.h>
#include <RDR2ScriptHook/natives.h>
#include <RDR2ScriptHook/types.h>

#include <algorithm>
#include <map>
#include <memory>
#include <minwindef.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

DWORD	vehUpdateTime;
DWORD	pedUpdateTime;

std::string const AnimalFinder::iniFilePath{ "AnimalsFinder/AnimalsFinder.ini" };

AnimalFinder::AnimalFinder() :
	iniOptions{ iniFilePath },
	animalInfos{ iniOptions.getAnimalNames() }
{}

void AnimalFinder::removeOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash) {
	if (showQuality) {
		MAP::BLIP_ADD_MODIFIER(animalBlip, hash);
	}else{
		MAP::REMOVE_BLIP(&animalBlip);
	}
}

void AnimalFinder::update()
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


	std::unordered_set<Blip> currentBlips;
	for (Ped ped: peds)
	{
		updateBlipForPed(ped, currentBlips);
	}


	for (auto& pair : blips) {
		auto currentIterator = currentBlips.find(pair.first);
		if (currentIterator == currentBlips.end()) {
			MAP::REMOVE_BLIP(&pair.second);
			blips.erase(pair.first);
		}
	}
}

void AnimalFinder::updateBlipForPed(Ped ped, std::unordered_set<Blip>& currentBlips)
{
	if (PED::IS_PED_HUMAN(ped))
		return;

	Hash animalType{ ENTITY::_GET_PED_ANIMAL_TYPE(ped) };

	static const Hash unknownAnimalTypeMagicValue{ 4141559444 };
	if (animalType == unknownAnimalTypeMagicValue) { // what the fuck is this magic value??
		return;
	}

	bool isBird{ static_cast<bool>(ENTITY::_GET_IS_BIRD(ped)) };
	if (!iniOptions.getShowBirds() && isBird) {
		return;
	}

	if (animalInfos.find(animalType) == animalInfos.end()) {
		return;
	}

	AnimalInfo const& animalInfo{ animalInfos.at(animalType) };
	ePedQuality quality = static_cast<ePedQuality>(PED::_GET_PED_QUALITY(ped));
	bool qualityMatches = animalInfo.QualityMatches(quality);

	auto iterator = blips.find(ped);
	if (iterator != blips.end()) { // Blip already exists for Ped
		if(!qualityMatches || ENTITY::IS_ENTITY_DEAD(ped)) { // Remove Blip (This happens when animal quality changes between tick, ie. after being shot)
			MAP::REMOVE_BLIP(&iterator->second);
		}
		if (qualityMatches) { // Update coordinates
			Vector3 animCords = ENTITY::GET_ENTITY_COORDS(ped, TRUE, FALSE);
			MAP::SET_BLIP_COORDS(iterator->second, animCords.x, animCords.y, animCords.z);
			currentBlips.insert(ped);
		}
		return;
	}
	if (!qualityMatches || ENTITY::IS_ENTITY_DEAD(ped)) {
		return;
	}
	static const Hash unknownBlipHash{ 0x63351D54 };
	Blip animalBlip = MAP::BLIP_ADD_FOR_ENTITY(unknownBlipHash, ped);
	blips.insert({ ped, animalBlip });
	currentBlips.insert(ped);


	if (quality <= ePedQuality::PQ_LOW) {
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
	static const Hash smallDotTexture{ 0x000A9056 }; // TODO load custom bird texture
	MAP::SET_BLIP_SPRITE(animalBlip, isBird ? smallDotTexture : animalInfo.IsFish() ? fishTexture : animalTexture, true);
	MAP::SET_BLIP_SCALE(animalBlip, 0.8);

	//MAP::_SET_BLIP_NAME(animalBlip, HUD::GET_STRING_FROM_HASH_KEY(animalType));
	MAP::_SET_BLIP_NAME(animalBlip, animalInfo.GetName().c_str());
}

bool AnimalFinder::qualityMatchesIni(int quality) const
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

std::unique_ptr<MenuBase> CreateMainMenu(MenuController& controller, std::unordered_map<Hash, AnimalInfo>& animalInfos)
{
	auto menu = std::make_unique<MenuBase>(std::make_unique<MenuItemTitle>("ANIMALS FINDER"));
	controller.RegisterMenu(menu.get());

	std::vector<std::pair<const Hash, AnimalInfo>*> sortedNames; // use vector of pointers to avoid unnecesssary copy
	sortedNames.reserve(animalInfos.size());
	for (auto& entry : animalInfos) {
		sortedNames.push_back(&entry); // not sorted yet
	}

	std::sort(sortedNames.begin(), sortedNames.end(), [](const auto* a, const auto* b) {
		return a->second.GetName() < b->second.GetName();
	});

	for (auto& animal : sortedNames) {
		AnimalInfo& animalInfo = animal->second;
		menu->AddItem(std::make_unique<MenuItemAnimal>(animalInfo));
	}

	return menu;
}

void AnimalFinder::run()
{
	MenuController menuController{};
	auto mainMenu = CreateMainMenu(menuController, animalInfos);

	while (true)
	{
		if (!menuController.HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			menuController.PushMenu(mainMenu.get());
		}
		menuController.Update();

		update();
		WAIT(0);
	}
}

void ScriptMain()
{
	AnimalFinder animalFinder;
	try {
		animalFinder.run();
	}
	catch (...) {

	}
}
