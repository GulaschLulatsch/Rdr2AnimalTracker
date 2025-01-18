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
std::map<Hash, const char*> animalsNames;
CSimpleIniA LangIni;
bool showBirds;
bool showExcellentQuality;
bool showMediumQuality;
bool showPoorQuality;

void removeOrModifyBlip(bool showQuality, Blip *animalBlip, Hash hash) {
	if (showQuality) {
		MAP::BLIP_ADD_MODIFIER(*animalBlip, hash);
		return;
	}

	MAP::REMOVE_BLIP(animalBlip);
}

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

		if (!showBirds && ENTITY::_GET_IS_BIRD(peds[i])) {
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

		Blip animalBlip = MAP::BLIP_ADD_FOR_ENTITY((Hash)1664425300, peds[i]);
		blips[peds[i]] = animalBlip;
		currentBlips[peds[i]] = animalBlip;

		switch (PED::_GET_PED_QUALITY(peds[i])) {
		case ePedQuality::PQ_INVALID:
		case ePedQuality::PQ_LOW:
			removeOrModifyBlip(showPoorQuality, &animalBlip, (Hash)0xA2814CC7);
			break;
		case ePedQuality::PQ_MEDIUM:
			removeOrModifyBlip(showMediumQuality, &animalBlip, (Hash)0xF91DD38D);
			break;
		case ePedQuality::PQ_HIGH:
			removeOrModifyBlip(showExcellentQuality, &animalBlip, (Hash)0xA5C4F725);
			break;
		}

		
		MAP::SET_BLIP_SPRITE(animalBlip, (Hash)-1646261997, true);
		MAP::SET_BLIP_SCALE(animalBlip, 0.8);
		MAP::_SET_BLIP_NAME(animalBlip, animalsNames[ENTITY::_GET_PED_ANIMAL_TYPE(peds[i])]);
		//MAP::_SET_BLIP_NAME(animalBli, "Привет, проверка");
	}

	for (auto& pair : blips) {
		if (currentBlips.find(pair.first) == currentBlips.end()) {
			MAP::REMOVE_BLIP(&blips[pair.first]);
		}
	}
}
void loadLang(const CSimpleIniA &generalIni) {
	const char* langFilePath;
	char path[32];
	char folderPath[15] = "AnimalsFinder/";

	langFilePath = generalIni.GetValue("GENERAL", "langFilePath", "undefinded");

	sprintf_s(path, "%s%s", folderPath, langFilePath);

	LangIni.LoadFile(path);

	// Get all keys in a section
	CSimpleIniA::TNamesDepend keys;
	LangIni.GetAllKeys("LANG", keys);


	//std::ofstream out;          // поток для записи
	//out.open("hello.txt");      // открываем файл для записи

	//if (out.is_open())
	//{



	// Iterate through keys and get their values
	for (const auto& key : keys) {
		const char* value = LangIni.GetValue("LANG", key.pItem, "undefinded");

		animalsNames[typetoHash[key.pItem]] = value;

		//out << value << std::endl;
		// Process the key-value pair
		// key.pItem is the key name, value is the corresponding value
	}
}

bool getBoolIniValue(const char* value) {
	return (value != nullptr && std::string(value) == "true");
}

void loadIni() 
{
	CSimpleIniA generalIni;
	

	// get the value of a key that doesn't exist

	SI_Error siError = generalIni.LoadFile("AnimalsFinder/AnimalsFinder.ini");
	if (siError < 0) throw "Error!Could not find the ini file";

	
	loadLang(generalIni);

	showBirds = getBoolIniValue(generalIni.GetValue("GENERAL", "showBirds", false));
	showExcellentQuality = getBoolIniValue(generalIni.GetValue("GENERAL", "showExcellentQuality", false));
	showMediumQuality = getBoolIniValue(generalIni.GetValue("GENERAL", "showMediumQuality", false));
	showPoorQuality = getBoolIniValue(generalIni.GetValue("GENERAL", "showPoorQuality", false));

	//}


}



void main()
{	

	loadIni();


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
