#pragma once

#include "IniOptions.h"

#include "../inc/types.h"
#include <map>
#include <string>

class AnimalsFinder
{
public:
	AnimalsFinder();
	void run();

private:
	void update();

	bool qualityMatchesIni(int quality);

	static void removeOrModifyBlip(bool showQuality, Blip animalBlip, Hash hash);

	static std::string const iniFilePath;

	IniOptions iniOptions;
	std::map<Ped, Blip> blips{};
	std::map<Hash, std::string> animalsNames;
};


void ScriptMain();