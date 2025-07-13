#pragma once

#include "MenuItemDefault.h"

#include "RDR2ScriptHook/natives.h"
#include "RDR2ScriptHook/types.h"

#include <string>
#include <map>

class MenuItemAnimal : public MenuItemDefault
{
public:

	MenuItemAnimal(
		std::string const& caption,
		Hash hash,
		std::map<Hash, std::string>* animalNames,
		std::map<Hash, std::string>* selectedAnimalNames
	);

	EMenuItemType GetClass() const override;
	void OnFrame() const override;
	void OnSelect() override;
	void OnRight() override;

private:
	Hash m_hash;
	std::map<Hash, std::string>* m_animalNames;
	std::map<Hash, std::string>* m_selectedAnimalNames;

};