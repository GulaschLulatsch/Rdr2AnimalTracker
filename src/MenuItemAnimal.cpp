#include "MenuItemAnimal.h"

#include "AnimalInfo.h"
#include "EMenuItemType.h"
#include "MenuItemBase.h"
#include "MenuItemDefault.h"
#include "IMenu.h"

#include <functional>
#include <utility>

MenuItemAnimal::MenuItemAnimal(
	AnimalInfo& animalInfo,
	std::function<void(AnimalInfo const&)> saveFunction
) :
	MenuItemDefault{ animalInfo.GetName()},
	m_animalInfo{ animalInfo },
	m_saveFunction{ std::move(saveFunction) }
{}

const AnimalInfo& MenuItemAnimal::GetAnimalInfo() const
{
	return m_animalInfo;
}

void MenuItemAnimal::OnSelect() {
	m_animalInfo.RotateQuality();
	m_saveFunction(m_animalInfo);
}

EMenuItemType MenuItemAnimal::GetClass() const
{
	return EMenuItemType::Animal;
}

void MenuItemAnimal::OnDraw(float lineTop, float lineLeft, bool active) const
{
	MenuItemBase::OnDraw(lineTop, lineLeft, active); 
	DrawQuality(m_animalInfo, lineTop, lineLeft);
}

void MenuItemAnimal::OnRight(MenuController*)
{ 
	OnSelect();
}
