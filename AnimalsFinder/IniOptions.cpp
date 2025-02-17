#include "IniOptions.h"
#include "namesInfo.h"

IniOptions::IniOptions() 
{
	this->showBirds = false;
	this->showExcellentQuality = false;
	this->showMediumQuality = false;
	this->showPoorQuality = false;

	this->loadOptions();
}

bool IniOptions::getBoolIniValue(const char* value) {
	return (value != nullptr && std::string(value) == "true");
}

std::map<Hash, const char*> IniOptions::getAnimalsNames()
{
	const char* langFilePath;
	char path[32];
	char folderPath[15] = "AnimalsFinder/";
	CSimpleIniA LangIni;

	langFilePath = this->generalIni.GetValue("GENERAL", "langFilePath", "undefinded");

	sprintf_s(path, "%s%s", folderPath, langFilePath);

	LangIni.LoadFile(path);

	// Get all keys in a section
	CSimpleIniA::TNamesDepend keys;
	LangIni.GetAllKeys("LANG", keys);


	//std::ofstream out;          // поток для записи
	//out.open("hello.txt");      // открываем файл для записи

	//if (out.is_open())
	//{

	std::map<Hash, const char*> animalsNames;

	// Iterate through keys and get their values
	for (const auto& key : keys) {
		const char* value = LangIni.GetValue("LANG", key.pItem, "undefinded");

		animalsNames[typetoHash[key.pItem]] = value;

		//out << value << std::endl;
		// Process the key-value pair
		// key.pItem is the key name, value is the corresponding value
	}

	return animalsNames;
}

IniOptions &IniOptions::loadOptions() 
{
	// get the value of a key that doesn't exist

	SI_Error siError = this->generalIni.LoadFile("AnimalsFinder/AnimalsFinder.ini");
	if (siError < 0) throw "Error!Could not find the ini file";

	this->showBirds = this->getBoolIniValue(this->generalIni.GetValue("GENERAL", "showBirds", false));
	this->showExcellentQuality = this->getBoolIniValue(this->generalIni.GetValue("GENERAL", "showExcellentQuality", false));
	this->showMediumQuality = this->getBoolIniValue(this->generalIni.GetValue("GENERAL", "showMediumQuality", false));
	this->showPoorQuality = this->getBoolIniValue(this->generalIni.GetValue("GENERAL", "showPoorQuality", false));

	return *this;
}

bool IniOptions::getShowBirds()
{
	return this->showBirds;
}

bool IniOptions::getShowExcellentQuality()
{
	return this->showExcellentQuality;
}

bool IniOptions::getShowMediumQuality()
{
	return this->showMediumQuality;
}

bool IniOptions::getShowPoorQuality()
{
	return this->showPoorQuality;
}

