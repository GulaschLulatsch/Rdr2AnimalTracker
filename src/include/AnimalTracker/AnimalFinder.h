#pragma once

#include "IniOptions.h"
#include "AnimalInfo.h"

#include <RDR2ScriptHook/types.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

class AnimalFinder
{
public:
	AnimalFinder();
	void run();

private:
	void update();

	void updateBlipForPed(Ped ped, std::unordered_set<Blip>& currentBlips);

	bool qualityMatchesIni(int quality) const;

	static void removeOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash);

	static std::string const iniFilePath;

	IniOptions iniOptions;
	std::unordered_map<Ped, Blip> blips{};
	std::unordered_map<Hash, AnimalInfo> animalInfos;
};


void ScriptMain();