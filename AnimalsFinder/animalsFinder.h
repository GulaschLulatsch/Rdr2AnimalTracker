#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include "..\inc\natives.h"
#include "..\inc\enums.h"
#include "..\inc\main.h"
#include "IniOptions.h"

class AnimalsFinder
{
protected:
	std::map<Ped, Blip> blips;
	std::map<Hash, std::string> animalsNames;
	IniOptions iniOptions{};
	AnimalsFinder& removeOrModifyBlip(bool showQuality, Blip* animalBlip, Hash hash);
	AnimalsFinder& update();
public:
	AnimalsFinder();
	void run();
};


void ScriptMain();