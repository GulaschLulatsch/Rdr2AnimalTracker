#pragma once

#include "AnimalInfo.h"
#include "CategoryInfo.h"
#include "IMenu.h"
#include "IniOptions.h"
#include "MenuController.h"

#include <RDR2ScriptHook/types.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

class AnimalTracker
{
public:
	AnimalTracker();
	void Run();

private:

	std::unique_ptr<IMenu> CreateMainMenu(MenuController& controller);

	void Update();

	void UpdateBlipForPed(Ped ped, std::unordered_set<Blip>& currentBlips);

	static void RemoveOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash);

	static std::string const iniFilePath;

	IniOptions m_iniOptions;
	std::unordered_map<Ped, Blip> m_blips{};
	std::unordered_map<Hash, AnimalInfo> m_animalInfos;

	//TEMP for testing TODO remove
	CategoryInfo m_fishCategory;
	CategoryInfo m_otherCategory;
};


void ScriptMain();