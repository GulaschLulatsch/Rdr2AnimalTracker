/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

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
	std::map<Hash, const char*> animalsNames;
	IniOptions iniOptions{};
	AnimalsFinder& removeOrModifyBlip(bool showQuality, Blip* animalBlip, Hash hash);
	AnimalsFinder& update();
public:
	AnimalsFinder();
	void run();
};


void ScriptMain();