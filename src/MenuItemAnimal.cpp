#include "MenuItemAnimal.h"

#include "AnimalInfo.h"
#include "EMenuItemType.h"
#include "IMenu.h"
#include "MenuItemBase.h"
#include "MenuItemDefault.h"

MenuItemAnimal::MenuItemAnimal(
	AnimalInfo& animalInfo
) :
	MenuItemDefault{ animalInfo.GetName()},
	m_animalInfo{ animalInfo }
{}

const AnimalInfo& MenuItemAnimal::GetAnimalInfo() const
{
	return m_animalInfo;
}

void MenuItemAnimal::OnSelect() {
	m_animalInfo.RotateQuality();
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
