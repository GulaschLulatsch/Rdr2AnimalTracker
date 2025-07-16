#pragma once

#include <RDR2ScriptHook/enums.h>

class QualityFilter {
public:
	QualityFilter(int filter);

	QualityFilter(bool poor, bool good, bool perfect);

	bool Matches(ePedQuality quality) const;

	int GetBitMask() const;
	
	void Rotate();

	static constexpr const int POOR{ 0b001 };
	static constexpr const int GOOD{ 0b010 };
	static constexpr const int PERFECT{ 0b100 };

private:

	int m_filter;
};