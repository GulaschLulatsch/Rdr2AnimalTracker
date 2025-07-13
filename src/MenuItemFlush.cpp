#include "MenuItemFlush.h"

#include "MenuItemBase.h"

#include <RDR2ScriptHook/types.h>

#include <map>
#include <string>

MenuItemFlush::MenuItemFlush(
	std::string const& caption, 
	std::map<Hash, std::string> oldAnimalNames, 
	std::map<Hash, std::string>* currentAnimalNames,
	std::map<Hash, std::string>* selectedAnimalNames
) :
	MenuItemDefault{ caption },
	m_oldAnimalNames{ std::move(oldAnimalNames) },
	m_currentAnimalNames{ currentAnimalNames },
	m_selectedAnimalNames{ selectedAnimalNames }
{}


void MenuItemFlush::OnSelect() {
	m_selectedAnimalNames->clear();
	(*m_currentAnimalNames) = m_oldAnimalNames;
}

EMenuItemType MenuItemFlush::GetClass() const
{
	return EMenuItemType::Flush;
}

void MenuItemFlush::OnRight()
{ /*Do nothing*/ }
