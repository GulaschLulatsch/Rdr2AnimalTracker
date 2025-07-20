#pragma once

#include "AnimalInfo.h"
#include "MenuItemDefault.h"

#include "EMenuItemType.h"

#include <functional>

class MenuController;

class MenuItemAnimal : public MenuItemDefault
{
public:
	MenuItemAnimal(AnimalInfo& animalInfo);

	const AnimalInfo& GetAnimalInfo() const;

	EMenuItemType GetClass() const override;
	void OnDraw(float lineTop, float lineLeft, bool active) const override;
	void OnSelect() override;
	void OnRight(MenuController* controller) override;

private:
	AnimalInfo& m_animalInfo;
};