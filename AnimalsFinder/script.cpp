/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "maps.h"


DWORD	vehUpdateTime;
DWORD	pedUpdateTime;

std::map<Ped, Blip> blips;
std::map<Hash, const char* > animalsNames;

void update()
{
	// player
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;
	
	//// get all vehicles
	const int ARR_SIZE = 1024;

	// get all peds
	Ped peds[ARR_SIZE];
	int count = worldGetAllPeds(peds, ARR_SIZE);
	
	// let's track all animals

	std::map<Ped, Blip> currentBlips;
	for (int i = 0; i < count; i++)
	{
		//HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", "fdfdfd"), 0.3, 0.3);
		if (PED::IS_PED_HUMAN(peds[i]))
			continue;

		//horse hash
		if (ENTITY::_GET_PED_ANIMAL_TYPE(peds[i]) == 4141559444) {
			continue;
		}

		if (ENTITY::_GET_IS_BIRD(peds[i])) {
			continue;
		}

		
		Vector3 animCords = ENTITY::GET_ENTITY_COORDS(peds[i], TRUE, FALSE);
		Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		float dist = MISC::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, animCords.x, animCords.y, animCords.z, FALSE);
		
		//if (dist >= 100) {

		//	if ( blips.find(peds[i]) == blips.end()  ) {
		//		continue;
		//	}

		//	MAP::REMOVE_BLIP(&blips[peds[i]]);

		//	blips.erase(peds[i]);
		//	continue;
		//}

		

		if (blips.find(peds[i]) != blips.end()) {
			MAP::SET_BLIP_COORDS(blips[peds[i]], animCords.x, animCords.y, animCords.z);
			currentBlips[peds[i]] = blips[peds[i]];
			continue;
		}

		Blip animalBli = MAP::BLIP_ADD_FOR_ENTITY((Hash)1664425300, peds[i]);
		blips[peds[i]] = animalBli;
		currentBlips[peds[i]] = animalBli;

		if (PED::_GET_PED_QUALITY(peds[i]) == ePedQuality::PQ_MEDIUM) {
			MAP::BLIP_ADD_MODIFIER(animalBli, (Hash)0xF91DD38D);
		}
		else if (PED::_GET_PED_QUALITY(peds[i]) == ePedQuality::PQ_HIGH) {
			MAP::BLIP_ADD_MODIFIER(animalBli, (Hash)0xA5C4F725);
		}
		else if (PED::_GET_PED_QUALITY(peds[i]) == ePedQuality::PQ_MAX) {
			MAP::BLIP_ADD_MODIFIER(animalBli, (Hash)0xF89D75CE);
		}

		
		MAP::SET_BLIP_SPRITE(animalBli, (Hash)-1646261997, true);
		MAP::SET_BLIP_SCALE(animalBli, 0.8);
		MAP::_SET_BLIP_NAME(animalBli, animalsNames[ENTITY::_GET_PED_ANIMAL_TYPE(peds[i])]);
	}

	for (auto& pair : blips) {
		if (currentBlips.find(pair.first) == currentBlips.end()) {
			MAP::REMOVE_BLIP(&blips[pair.first]);
		}
	}
}

void loadLang() 
{
	CSimpleIniA GeneralIni;
	CSimpleIniA LangIni;
	const char* pv;
	char path[32];
	char folderPath[15] = "AnimalsFinder/";

	// get the value of a key that doesn't exist

	SI_Error rc = GeneralIni.LoadFile("AnimalsFinder/AnimalsFinder.ini");
	if (rc < 0) { std::wcout << L"Idiot." << std::endl; return; };


	pv = GeneralIni.GetValue("GENERAL", "langFilePath", "");

	sprintf_s(path, "%s%s", folderPath, pv);

	LangIni.LoadFile(path);

	// Get all keys in a section
	CSimpleIniA::TNamesDepend keys;
	LangIni.GetAllKeys("LANG", keys);

	// Iterate through keys and get their values
	for (const auto& key : keys) {
		const char* value = LangIni.GetValue("LANG", key.pItem, "");

		animalsNames[typetoHash[key.pItem]] = value;
		// Process the key-value pair
		// key.pItem is the key name, value is the corresponding value
	}
}

void main()
{	

	loadLang();


	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
