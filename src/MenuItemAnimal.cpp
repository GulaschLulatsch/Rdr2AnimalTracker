#include "MenuItemAnimal.h"

#include "AnimalInfo.h"
#include "EMenuItemType.h"
#include "MenuItemDefault.h"

#include <RDR2ScriptHook/natives.h>

MenuItemAnimal::MenuItemAnimal(
	AnimalInfo& animalInfo
) :
	MenuItemDefault{ animalInfo.GetName()},
	m_animalInfo{ animalInfo }
{}

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
	static char* blipTextureDict{ "BLIPS" };
	static char* blipPoorQuality{ "blip_animal_quality_01" };
	static char* blipGoodQuality{ "blip_animal_quality_02" };
	static char* blipPerfectQuality{ "blip_animal_quality_03" };
	static const ColorRgba colorPoor{ 230, 230, 230 };
	static const ColorRgba colorGood{ 80, 165, 215 };
	static const ColorRgba colorPerfect{ 255, 245, 145 };
	static const float blipWidth{ 32.f / 1920.f };
	static const float blipHeight{ 32.f / 1080.f };
	if (m_animalInfo.QualityMatches(PQ_HIGH)) {
		GRAPHICS::DRAW_SPRITE(blipTextureDict, blipPerfectQuality,
			lineLeft + GetItemWidth() - blipWidth, lineTop + GetItemHeight() - blipHeight,
			blipWidth, blipHeight, 0.f,
			colorPerfect.r, colorPerfect.g, colorPerfect.b, colorPerfect.a,
			false/*??*/
		);
	}
	if (m_animalInfo.QualityMatches(PQ_MEDIUM)) {
		GRAPHICS::DRAW_SPRITE(blipTextureDict, blipGoodQuality,
			lineLeft + GetItemWidth() - (2* blipWidth), lineTop + GetItemHeight() - blipHeight,
			blipWidth, blipHeight, 0.f,
			colorGood.r, colorGood.g, colorGood.b, colorGood.a,
			false/*??*/
		);
	}
	if (m_animalInfo.QualityMatches(PQ_LOW)) {
		GRAPHICS::DRAW_SPRITE(blipTextureDict, blipPoorQuality,
			lineLeft + GetItemWidth() - (3* blipWidth), lineTop + GetItemHeight() - blipHeight,
			blipWidth, blipHeight, 0.f,
			colorPoor.r, colorPoor.g, colorPoor.b, colorPoor.a,
			false/*??*/
		);
	}
}

void MenuItemAnimal::OnRight()
{ 
	OnSelect();
}
