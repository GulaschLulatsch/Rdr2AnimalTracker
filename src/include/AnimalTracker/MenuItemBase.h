#pragma once

#include "ColorRgba.h"
#include "IMenu.h"
#include "IMenuItem.h"

class MenuBase;

class MenuItemBase : public IMenuItem
{
public:
	virtual ~MenuItemBase() = default;

	virtual void OnDraw(float lineTop, float lineLeft, bool active) const override;

	float GetItemWidth() const override; 
	float GetItemHeight() const override;

	ColorRgba const& GetRectColor() const override;
	ColorRgba const& GetTextColor() const override;

	ColorRgba const& GetActiveRectColor() const override;
	ColorRgba const& GetActiveTextColor() const override;

	void SetMenu(IMenu* menu) override;
	IMenu* GetMenu() const override;

protected:
	MenuItemBase(
		float textLeftMargin,
		ColorRgba colorRect,
		ColorRgba colorText,
		ColorRgba colorRectActive = ColorRgba{},
		ColorRgba colorTextActive = ColorRgba{}
	);

	void WaitAndDraw(int ms);

private:

	float		m_textLeftMargin;
	ColorRgba	m_colorRect;
	ColorRgba	m_colorText;
	ColorRgba	m_colorRectActive;
	ColorRgba	m_colorTextActive;

	IMenu* m_menu{ nullptr };
};
