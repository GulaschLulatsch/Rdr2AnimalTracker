#pragma once

#include "AnimalInfo.h"
#include "IMenu.h"
#include "IniOptions.h"
#include "MenuController.h"
#include "IInfo.h"

#include <ScriptHookRDR2/types.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class AnimalTracker
{
public:
	AnimalTracker();
	void Run();

private:

	void Update();

	void UpdateBlipForPed(Ped ped, std::unordered_set<Blip>& currentBlips);

	static void RemoveOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash);

	static std::string const iniFilePath;

	MenuController m_menuController;
	IniOptions m_iniOptions;
	std::unordered_map<Ped, Blip> m_blips{};
	std::vector<std::unique_ptr<IInfo>> m_categories;
	std::unordered_map<Hash, AnimalInfo*> m_animalInfos;
	std::unique_ptr<IMenu> m_mainMenu;

};


void ScriptMain();