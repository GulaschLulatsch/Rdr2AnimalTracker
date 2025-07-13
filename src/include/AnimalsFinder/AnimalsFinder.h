#pragma once

#include "IniOptions.h"

#include "RDR2ScriptHook/types.h"

#include <map>
#include <set>
#include <string>

class AnimalsFinder
{
public:
	AnimalsFinder();
	void run();

private:
	void update();

	void updateBlipForPed(Ped ped, std::set<Blip>& currentBlips);

	bool qualityMatchesIni(int quality) const;

	static void removeOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash);

	static std::string const iniFilePath;

	IniOptions iniOptions;
	std::map<Ped, Blip> blips{};
	std::map<Hash, std::string> animalsNames;
};


void ScriptMain();