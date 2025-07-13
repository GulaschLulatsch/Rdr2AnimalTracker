#include "MenuItemAnimal.h"

MenuItemAnimal::MenuItemAnimal(
	std::string const& caption, 
	Hash hash,
	std::map<Hash, std::string>* animalNames, 
	std::map<Hash, std::string>* selectedAnimalNames
) :
	MenuItemDefault{ caption },
	m_hash{ hash },
	m_animalNames{ animalNames },
	m_selectedAnimalNames{ selectedAnimalNames }
{}

void MenuItemAnimal::OnSelect() {
	auto it = m_selectedAnimalNames->find(m_hash);

	// TODO represent state somehow
	if (it == m_selectedAnimalNames->end()) {
		// not present, add it to map
		m_selectedAnimalNames->insert({ m_hash, GetCaption() }); 
	}
	else {
		// if present, remove it
		m_selectedAnimalNames->erase(m_hash);
	}
	(*m_animalNames) = (*m_selectedAnimalNames);
}

EMenuItemType MenuItemAnimal::GetClass() const
{
	return EMenuItemType::Animal;
}

void MenuItemAnimal::OnRight()
{ /*Do nothing*/ }
