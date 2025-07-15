#pragma once

#include "AnimalInfo.h"
#include "MenuItemDefault.h"

#include "EMenuItemType.h"

class MenuItemAnimal : public MenuItemDefault
{
public:
	MenuItemAnimal(
		AnimalInfo& animalInfo
	);

	EMenuItemType GetClass() const override;
	void OnDraw(float lineTop, float lineLeft, bool active) const override;
	void OnSelect() override;
	void OnRight() override;

private:
	AnimalInfo& m_animalInfo;
};