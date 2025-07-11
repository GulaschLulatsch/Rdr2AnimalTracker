#pragma once

#include "IniOptions.h"

#include "../inc/types.h"

#include <experimental/filesystem>
#include <map>
#include <string>

class AnimalsFinder
{
public:
	AnimalsFinder();
	void run();

private:
	AnimalsFinder& removeOrModifyBlip(bool showQuality, Blip* animalBlip, Hash hash);
	AnimalsFinder& update();

	static std::experimental::filesystem::path const iniFilePath;

	std::map<Ped, Blip> blips{};
	std::map<Hash, std::string> animalsNames{};
	IniOptions iniOptions;
};


void ScriptMain();