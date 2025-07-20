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

	std::unique_ptr<IMenu> CreateMenus(MenuController& controller);

	void Update();

	void UpdateBlipForPed(Ped ped, std::unordered_set<Blip>& currentBlips);

	static void RemoveOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash);

	static std::string const iniFilePath;

	IniOptions m_iniOptions;
	std::unordered_map<Ped, Blip> m_blips{};
	std::vector<std::unique_ptr<IContainingInfo>> m_categories;
	std::unordered_map<Hash, AnimalInfo*> m_animalInfos{};

};


void ScriptMain();