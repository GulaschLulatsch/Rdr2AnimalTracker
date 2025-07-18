#include "MenuItemDefault.h"

#include "ColorRgba.h"
#include "IInfo.h"
#include "MenuItemBase.h"

#include <RDR2ScriptHook/enums.h>
#include <RDR2ScriptHook/natives.h>

#include <string>

const float MenuItemDefault::TEXT_LEFT_MARGIN{ 0.01f };

const ColorRgba MenuItemDefault::RECT_COLOR{ 181, 21, 2, 135 };
const ColorRgba MenuItemDefault::TEXT_COLOR{ 255, 255, 255, 255 };
const ColorRgba MenuItemDefault::RECT_COLOR_ACTIVE{ 235, 2, 2, 185 };
const ColorRgba MenuItemDefault::TEXT_COLOR_ACTIVE{ 255, 255, 255, 255 };

MenuItemDefault::MenuItemDefault(std::string const& caption) :
	MenuItemBase(
		TEXT_LEFT_MARGIN,
		RECT_COLOR,
		TEXT_COLOR,
		RECT_COLOR_ACTIVE,
		TEXT_COLOR_ACTIVE
	),
	m_caption(caption) 
{}

void MenuItemDefault::DrawQuality(const IInfo& info, float lineTop, float lineLeft) const
{
	static char* blipTextureDict{ "BLIPS" };
	static char* blipPoorQuality{ "blip_animal_quality_01" };
	static char* blipGoodQuality{ "blip_animal_quality_02" };
	static char* blipPerfectQuality{ "blip_animal_quality_03" };
	static const ColorRgba colorDisabled{ 100, 100, 100, 150 };
	static const ColorRgba colorPoor{ 230, 230, 230 };
	static const ColorRgba colorGood{ 80, 165, 215 };
	static const ColorRgba colorPerfect{ 255, 245, 145 };
	static const float blipWidth{ 32.f / 1920.f };
	static const float blipHeight{ 32.f / 1080.f };
	if (!info.IsQualitySet() || info.QualityMatches(PQ_HIGH)) {
		ColorRgba const& color = info.IsQualitySet() ? colorPerfect : colorDisabled;
		GRAPHICS::DRAW_SPRITE(blipTextureDict, blipPerfectQuality,
			lineLeft + GetItemWidth() - blipWidth, lineTop + GetItemHeight() - blipHeight,
			blipWidth, blipHeight, 0.f,
			color.r, color.g, color.b, color.a,
			false/*??*/
		);
	}
	if (!info.IsQualitySet() || info.QualityMatches(PQ_MEDIUM)) {
		ColorRgba const& color = info.IsQualitySet() ? colorGood : colorDisabled;
		GRAPHICS::DRAW_SPRITE(blipTextureDict, blipGoodQuality,
			lineLeft + GetItemWidth() - (2 * blipWidth), lineTop + GetItemHeight() - blipHeight,
			blipWidth, blipHeight, 0.f,
			color.r, color.g, color.b, color.a,
			false/*??*/
		);
	}
	if (!info.IsQualitySet() || info.QualityMatches(PQ_LOW)) {
		ColorRgba const& color = info.IsQualitySet() ? colorPoor : colorDisabled;
		GRAPHICS::DRAW_SPRITE(blipTextureDict, blipPoorQuality,
			lineLeft + GetItemWidth() - (3 * blipWidth), lineTop + GetItemHeight() - blipHeight,
			blipWidth, blipHeight, 0.f,
			color.r, color.g, color.b, color.a,
			false/*??*/
		);
	}
}

std::string MenuItemDefault::GetCaption() const {
	return m_caption;
}