#include "AnimalTracker.h"

#include "AnimalInfo.h"
#include "CategoryMenu.h"
#include "IMenuItem.h"
#include "InputAction.h"
#include "Keyboard.h"
#include "MenuController.h"
#include "MenuItemTitle.h"
#include "StringComparator.h"

#include <ScriptHookRDR2/enums.h>
#include <ScriptHookRDR2/main.h>
#include <ScriptHookRDR2/natives.h>
#include <ScriptHookRDR2/types.h>

#include <spdlog/common.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <filesystem>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <Windows.h>

DWORD	vehUpdateTime;
DWORD	pedUpdateTime;

std::filesystem::path const AnimalTracker::iniFilePath{ "AnimalTracker/AnimalTracker.ini" };

AnimalTracker::AnimalTracker() :
	m_menuController{},
	m_iniOptions{ iniFilePath },
	m_categories{ m_iniOptions.LoadInfo() },
	m_animalInfos{},
	m_mainMenu{ nullptr },
	m_buttonMappings{ m_iniOptions.LoadButtonMappings() }
{
	spdlog::info("Reading and Writing to ini finished. Internal state valid, building gui from {} elements", m_categories.size());
	std::vector<std::unique_ptr<IMenuItem>> menuEntries{};
	for (auto const& entry : m_categories) {
		spdlog::debug("Info entry {} with key {}", entry->GetName(), entry->GetKey() );

		menuEntries.emplace_back(entry->CreateMenuItem());
		spdlog::debug("Menu item created for {}", entry->GetName());

		auto infos{ entry->GetAllAnimalInfos() };
		spdlog::debug("Received {} animal infos from entry {}", infos.size(), entry->GetName());

		for (auto const& info : infos) {
			m_animalInfos.insert({ info->GetHash(), info });
		}
	}

	//sort entries
	StringComparator comp{ m_iniOptions.GetLocale() };
	spdlog::debug("Sorting {} menu entries", menuEntries.size());
	std::sort(menuEntries.begin(), menuEntries.end(), [&comp](const std::unique_ptr<IMenuItem>& a, const std::unique_ptr<IMenuItem>& b) {
		return comp(a->GetCaption(), b->GetCaption());
	});

	auto mainMenu{ std::make_unique<CategoryMenu>(std::make_unique<MenuItemTitle>("ANIMAL TRACKER")) };
	spdlog::debug("Adding {} items to main menu", menuEntries.size());
	for (std::unique_ptr<IMenuItem>& item : menuEntries) {
		mainMenu->AddItem(std::move(item));
	}
	m_mainMenu = std::move(mainMenu);
	spdlog::info("Initialization finished");
}

void AnimalTracker::RemoveOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash) {
	if (showQuality) {
		MAP::BLIP_ADD_MODIFIER(animalBlip, hash);
	}else{
		MAP::REMOVE_BLIP(animalBlip);
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
			MAP::REMOVE_BLIP(pair.second);
			m_blips.erase(pair.first);
		}
	}
}

void SetBlipQualityModifier(Blip blip, ePedQuality quality) {
	MAP::BLIP_REMOVE_MODIFIER(blip, 0); // removes all modifiers
	if (quality <= ePedQuality::PQ_LOW) {
		static const Hash blipModifierArea_solidWhite{ 0xA2814CC7 };
		MAP::BLIP_ADD_MODIFIER(blip, blipModifierArea_solidWhite);
	}
	else if (quality == ePedQuality::PQ_MEDIUM) {
		static const Hash blipModifierDebugBlue_solidBlue{ 0xF91DD38D };
		MAP::BLIP_ADD_MODIFIER(blip, blipModifierDebugBlue_solidBlue);
	}
	else {
		static const Hash blipModifierDebugYellow_solidYellow{ 0xA5C4F725 };
		MAP::BLIP_ADD_MODIFIER(blip, blipModifierDebugYellow_solidYellow);
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

	AnimalInfo const& animalInfo{ *animalInfoIterator->second };


	ePedQuality quality = static_cast<ePedQuality>(PED::_GET_PED_QUALITY(ped));
	bool qualityMatches = animalInfo.QualityMatches(quality);

	auto iterator = m_blips.find(ped);
	if (iterator != m_blips.end()) { // Blip already exists for Ped
		if(!qualityMatches || ENTITY::IS_ENTITY_DEAD(ped)) { // Remove Blip (This happens when animal quality changes between tick, ie. after being shot)
			MAP::REMOVE_BLIP(iterator->second);
			return;
		}
		if (qualityMatches && MAP::_IS_BLIP_ATTACHED_TO_ANY_ENTITY(iterator->second)) { // Update coordinates
			Vector3 animCords = ENTITY::GET_ENTITY_COORDS(ped, TRUE, FALSE);
			MAP::SET_BLIP_COORDS(iterator->second, animCords.x, animCords.y, animCords.z);
			SetBlipQualityModifier(iterator->second, quality);
			currentBlips.insert(ped);
			return;
		}
	}
	if (!qualityMatches || MAP::_DOES_ENTITY_HAVE_BLIP(ped) || ENTITY::IS_ENTITY_DEAD(ped)) {
		return;
	}
	static const Hash unknownBlipHash{ 0x63351D54 };
	Blip animalBlip = MAP::BLIP_ADD_FOR_ENTITY(unknownBlipHash, ped);
	m_blips.insert({ ped, animalBlip });
	currentBlips.insert(ped);

	SetBlipQualityModifier(animalBlip, quality);

	static const Hash animalTexture{ MISC::GET_HASH_KEY("blip_animal") };
	//static const Hash fishTexture{ MISC::GET_HASH_KEY("blip_mg_fishing") };	// although theses textures would fit for fishing / birds, their black backgrounds make them not usable
	//static const Hash birdTexture{ MISC::GET_HASH_KEY("blip_event_riggs_camp") }; 
	//if (ENTITY::_GET_IS_BIRD(ped)) {
	//	MAP::SET_BLIP_SPRITE(animalBlip, birdTexture, false);
	//}
	//else if (PED::_IS_META_PED_FISH(ped)) {
	//	MAP::SET_BLIP_SPRITE(animalBlip, fishTexture, false);
	//}
	//else {
		MAP::SET_BLIP_SPRITE(animalBlip, animalTexture, true);
	//}
	MAP::SET_BLIP_SCALE(animalBlip, 0.8f);

	//MAP::_SET_BLIP_NAME(animalBlip, HUD::GET_STRING_FROM_HASH_KEY(animalType));
	MAP::_SET_BLIP_NAME(animalBlip, animalInfo.GetName().c_str());
}

void AnimalTracker::Run()
{
	while (true)
	{
		std::set<InputAction> input{ Keyboard::GetKeyboardState().GetMenuInputOnFrame(m_buttonMappings) };
		if (!m_menuController.HasActiveMenu() && input.contains(InputAction::MENU))
		{
			spdlog::info("Menu Opened");
			input.erase(InputAction::MENU);
			m_menuController.PushMenu(m_mainMenu.get());
		}
		m_menuController.Update(input);

		Update();
		WAIT(0);
	}
}

void ScriptMain()
{
	auto logCleanup{ std::unique_ptr<void, std::function<void(void*)>>{ nullptr, [](void*) { spdlog::shutdown(); }} };
	try
	{
		spdlog::set_default_logger(spdlog::rotating_logger_mt("AnimalTrackerLogger", "AnimalTracker/log.txt", 1024ull * 1024ull * 3ull, 2ull));

		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
		spdlog::set_level(spdlog::level::debug);
		spdlog::flush_on(spdlog::level::info);

		spdlog::info("AnimalTracker starting - log initialized");
	}
	catch (...)
	{
		/*Do nothing if log init fails*/
	}
	AnimalTracker animalTracker;
	animalTracker.Run();
}
