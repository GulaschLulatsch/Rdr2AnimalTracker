#include "AnimalTracker.h"

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
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <Windows.h>

DWORD	vehUpdateTime;
DWORD	pedUpdateTime;

std::string const AnimalTracker::iniFilePath{ "AnimalTracker/AnimalTracker.ini" };

AnimalTracker::AnimalTracker() :
	m_iniOptions{ iniFilePath },
	m_animalInfos{ m_iniOptions.GetAnimalMap() }
{}

void AnimalTracker::RemoveOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash) {
	if (showQuality) {
		MAP::BLIP_ADD_MODIFIER(animalBlip, hash);
	}else{
		MAP::REMOVE_BLIP(&animalBlip);
	}
}

void AnimalTracker::Update()
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
		UpdateBlipForPed(ped, currentBlips);
	}


	for (auto& pair : m_blips) {
		auto currentIterator = currentBlips.find(pair.first);
		if (currentIterator == currentBlips.end()) {
			MAP::REMOVE_BLIP(&pair.second);
			m_blips.erase(pair.first);
		}
	}
}

void AnimalTracker::UpdateBlipForPed(Ped ped, std::unordered_set<Blip>& currentBlips)
{
	if (PED::IS_PED_HUMAN(ped))
		return;

	Hash animalType{ ENTITY::_GET_PED_ANIMAL_TYPE(ped) };

	static const Hash unknownAnimalTypeMagicValue{ 4141559444 };
	if (animalType == unknownAnimalTypeMagicValue) { // what the fuck is this magic value??
		return;
	}
	auto animalInfoIterator{ m_animalInfos.find(animalType) };
	if (animalInfoIterator == m_animalInfos.end()) {
		return;
	}

	AnimalInfo const& animalInfo{ animalInfoIterator->second };


	ePedQuality quality = static_cast<ePedQuality>(PED::_GET_PED_QUALITY(ped));
	bool qualityMatches = animalInfo.QualityMatches(quality);

	auto iterator = m_blips.find(ped);
	if (iterator != m_blips.end()) { // Blip already exists for Ped
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
	m_blips.insert({ ped, animalBlip });
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

	bool isBird{ static_cast<bool>(ENTITY::_GET_IS_BIRD(ped)) };

	static const Hash animalTexture{ MISC::GET_HASH_KEY("blip_animal") };
	//static const Hash fishTexture{ MISC::GET_HASH_KEY("blip_mg_fishing") };	// although theses textures would fit for fishing / birds, their black backgrounds make them not usable
	//static const Hash smallDotTexture{ MISC::GET_HASH_KEY("blip_event_riggs_camp") }; 
	MAP::SET_BLIP_SPRITE(animalBlip, animalTexture, true);
	MAP::SET_BLIP_SCALE(animalBlip, isBird || animalInfo.IsFish() ? 0.5f : 0.8f); // Make Fish & birds smaller icons
	MAP::SET_BLIP_ROTATION(animalBlip, animalInfo.IsFish() ? 180 : 0); // Not sure if i like this: for now draw fish icons upside down

	//MAP::_SET_BLIP_NAME(animalBlip, HUD::GET_STRING_FROM_HASH_KEY(animalType));
	MAP::_SET_BLIP_NAME(animalBlip, animalInfo.GetName().c_str());
}

std::unique_ptr<MenuBase> AnimalTracker::CreateMainMenu(MenuController& controller)
{
	auto menu = std::make_unique<MenuBase>(std::make_unique<MenuItemTitle>("ANIMAL TRACKER"));
	controller.RegisterMenu(menu.get());

	std::vector<std::pair<const Hash, AnimalInfo>*> sortedNames; // use vector of pointers to avoid unnecesssary copy
	sortedNames.reserve(m_animalInfos.size());
	for (auto& entry : m_animalInfos) {
		sortedNames.push_back(&entry); // not sorted yet
	}

	std::sort(sortedNames.begin(), sortedNames.end(), [](const auto* a, const auto* b) {
		return a->second.GetName() < b->second.GetName();
	});

	for (auto& animal : sortedNames) {
		AnimalInfo& animalInfo = animal->second;
		menu->AddItem(std::make_unique<MenuItemAnimal>(
			animalInfo, 
			[&ini = m_iniOptions](const AnimalInfo& info) { 
				ini.StoreAnimalInfos(std::vector<const AnimalInfo*>{ &info }); 
			}
		));
	}

	return menu;
}

void AnimalTracker::Run()
{
	MenuController menuController{};
	auto mainMenu = CreateMainMenu(menuController);

	while (true)
	{
		if (!menuController.HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			menuController.PushMenu(mainMenu.get());
		}
		menuController.Update();

		Update();
		WAIT(0);
	}
}

void ScriptMain()
{
	AnimalTracker animalTracker;
	try {
		animalTracker.Run();
	}
	catch (...) {

	}
}
