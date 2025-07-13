#pragma once

#include "MenuItemDefault.h"

#include "EMenuItemType.h"

#include <RDR2ScriptHook/types.h>

#include <map>
#include <string>

class MenuItemFlush : public MenuItemDefault {
public:
	MenuItemFlush(
		std::string const& caption,
		std::map<Hash, std::string> oldAnimalNames,
		std::map<Hash, std::string>* currentAnimalNames,
		std::map<Hash, std::string>* selectedAnimalNames
	);

	EMenuItemType GetClass() const override;

	void OnFrame() const override;
	void OnSelect() override;
	void OnRight() override;

private:
	std::map<Hash, std::string> m_oldAnimalNames;
	std::map<Hash, std::string>* m_currentAnimalNames;
	std::map<Hash, std::string>* m_selectedAnimalNames;

};